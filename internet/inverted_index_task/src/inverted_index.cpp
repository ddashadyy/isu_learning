#include <cmath>
#include <cctype>
#include <vector>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <filesystem>
#include <curl/curl.h>
#include <libstemmer.h>
#include "inverted_index.hpp"

InvertedIndex::InvertedIndex(const InvertedIndex& other)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    m_documents = other.m_documents;
    m_index = other.m_index;
}

void InvertedIndex::indexDocument(const std::string& path)
{
    auto find_it = std::find(m_documents.begin(), m_documents.end(), path);
    if (find_it != m_documents.end()) return;

    auto document_type = get_mime_type_of_document(path);

    auto make_file = [](const std::string& path, const MimeType& mime_type) {
        std::ifstream file;
        if (mime_type == MimeType::TEXT || mime_type == MimeType::HTML)
        {
            file.open(path);
            if (!file.is_open()) 
            {
                std::cerr << "Could not open the file\n";
            }
        }
        return file;
    };

    auto normalize_and_write_to_index = [&](const std::vector<std::string>& words, int doc_id) {
        std::for_each(words.begin(), words.end(), [&](const std::string& word) {
            std::string normalized_word = normalize(word); 
            if (!normalized_word.empty()) 
                add_word_to_index(normalized_word, doc_id);
            }
        );
    };

    auto index_hmtl = [&](const char* buffer) {
        GumboOutput* output = gumbo_parse(buffer);
        std::string extracted_text;
        
        extract_text(output->root, extracted_text);
        gumbo_destroy_output(&kGumboDefaultOptions, output);

        return extracted_text;
    };

    int doc_id = 0;

    if (document_type == MimeType::TEXT)
    {
        auto file = make_file(path, document_type);
        doc_id = m_documents.size();
        m_documents.push_back(path);

        std::string line{};
        while (std::getline(file, line)) 
        {
            std::istringstream iss(line);
            std::string word{};

            while (iss >> word) 
            {
                word = normalize(word);
                if (!word.empty()) 
                    add_word_to_index(word, doc_id);   
            }
        }
        file.close();
    }

    else if (document_type == MimeType::HTML)
    {
        auto html_file = make_file(path, document_type);

        doc_id = m_documents.size();
        m_documents.push_back(path);

        std::stringstream buffer;
        buffer << html_file.rdbuf();

        html_file.close();
    
        std::string extracted_text = index_hmtl(buffer.str().c_str());

        auto splitted_string = splitString(extracted_text);
        normalize_and_write_to_index(splitted_string, doc_id);
    }

    else if (document_type == MimeType::WEB)
    {
        doc_id = m_documents.size();
        m_documents.push_back(path);

        int doc_id = m_documents.size();
        m_documents.push_back(path);

        std::string read_buffer = connect_by_url(path);
        if (read_buffer.empty()) return;

        std::string extracted_text = index_hmtl(read_buffer.c_str());

        auto splitted_string = splitString(extracted_text);
        normalize_and_write_to_index(splitted_string, doc_id);
    }

    log_document(path, doc_id);
}

void InvertedIndex::indexCollection(const std::string& folder)
{
    namespace fs = std::filesystem;

    log_top_table();

    const fs::path dir(folder);
    if (!fs::exists(dir)) return; 

    for (const auto& entry : fs::directory_iterator(dir)) 
    {
        // recursive traversal
        if (fs::is_directory(entry)) indexCollection(entry.path().string());
        if (fs::is_regular_file(entry.status())) 
        {
            const std::string key = entry.path().string();
            indexDocument(key);
        }
    }

    for (auto& [word, term] : m_index)
    {
        double documents_size = static_cast<double>(m_documents.size()); 
        double document_frequency = static_cast<double>(term.getDocumentFrequency());

        double idf = log10(documents_size / document_frequency);
        term.computeTfIdf(idf);
    }

    log_bottom_table();
    
}

std::list<DocumentRelevance> InvertedIndex::executeQuery(const std::string& query)
{
    std::list <DocumentRelevance> answer;
    auto splited_query = splitString(query);

    for (const auto& word : splited_query)   
    {
        auto normalized_word = normalize(word);

        if (m_index.find(normalized_word) != m_index.end())
        {
            auto& term = m_index.at(normalized_word);   
            auto& list = term.getList();
            
            for (const auto& term_doc : list)
            {
                DocumentRelevance doc_rel(term_doc.getDocId());
                answer.push_back(doc_rel);
            }

            intersect(answer, term);
        }

        
    }
    
    answer.sort([](const DocumentRelevance& lhs, const DocumentRelevance& rhs) {
        return lhs.getRelevance() > rhs.getRelevance();
    });

    answer.remove_if([](const DocumentRelevance& dr) {
        return dr.getRelevance() < std::numeric_limits<double>::epsilon();
    });

    return answer;
}

std::list<DocumentRelevance> InvertedIndex::executeQuery(const std::string& query, size_t n)
{
    auto list_doc_rel = executeQuery(query);
    std::list<DocumentRelevance> limited_results;

    size_t count = 0;
    for (const auto& doc_rel : list_doc_rel)
    {
        if (count < n)
        {
            limited_results.push_back(doc_rel);
            ++count;
        }
        else break; 
    }

    return limited_results; 
}

void InvertedIndex::intersect(std::list<DocumentRelevance>& answer, Term& term)
{  
    auto& term_document = term.getList();

    auto start_term_document = term_document.begin();
    auto start_doc_relevance = answer.begin();
    
    while (start_term_document != term_document.end() && start_doc_relevance != answer.end())
    {
        size_t rev_doc_id = start_doc_relevance->getDocId();
        size_t term_doc_id = start_term_document->getDocId();

        if (rev_doc_id == term_doc_id)
        {
            double tf_idf = start_term_document->getTfIdf();
            start_doc_relevance->updateRelevance(tf_idf);

            ++start_term_document;
            ++start_doc_relevance;
        }
        
        else if (rev_doc_id > term_doc_id) ++start_term_document;
        else if (rev_doc_id < term_doc_id) ++start_doc_relevance;        
    }
}

void InvertedIndex::extract_text(GumboNode* node, std::string& output)
{  
    if (node->type == GUMBO_NODE_ELEMENT) 
    {
        GumboTag tag = node->v.element.tag;

        if (tag == GUMBO_TAG_STYLE || tag == GUMBO_TAG_SCRIPT) 
            return; 
        
        for (unsigned int i = 0; i < node->v.element.children.length; ++i)  
        {
            extract_text(static_cast<GumboNode*>(node->v.element.children.data[i]), output); 
        }
    } 

    else if (node->type == GUMBO_NODE_TEXT)  
    { 
        output.append(node->v.text.text); 
        output.append(" "); 
    } 
}

size_t InvertedIndex::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp)
{
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

std::vector<std::string> InvertedIndex::splitString(const std::string& line) noexcept
{
    std::vector<std::string> words;
    std::istringstream stream(line);
    std::string word;

    while (stream >> word) {
        words.push_back(word);
    }

    return words;
}

// void InvertedIndex::serialize(const std::string& destination)
// {
//     std::ofstream out(destination, std::ios::binary);
    
//     // Сериализация list m_documents
//     size_t doc_count = m_documents.size();
//     out.write(reinterpret_cast<const char*>(&doc_count), sizeof(doc_count));
    
//     for (const auto& docs : m_documents) 
//     {
//         size_t length = docs.size();
//         out.write(reinterpret_cast<const char*>(&length), sizeof(length));
//         out.write(docs.c_str(), length);
//     }
    
//     // Сериализация hash_map m_index
//     size_t index_size = m_index.size();
//     out.write(reinterpret_cast<const char*>(&index_size), sizeof(index_size));
    
//     for (const auto& [key, value] : m_index) 
//     {
//         size_t key_length = key.size();
//         out.write(reinterpret_cast<const char*>(&key_length), sizeof(key_length));
//         out.write(key.c_str(), key_length);
        
//         size_t value_size = value.size();
//         out.write(reinterpret_cast<const char*>(&value_size), sizeof(value_size));
        
//         for (const auto& val : value) 
//         {
//             out.write(reinterpret_cast<const char*>(&val), sizeof(val));
//         }
//     }
    
//     out.close();
// }

// InvertedIndex& InvertedIndex::deserialize(const std::string& source)
// {
//     std::ifstream in(source, std::ios::binary);
//     if (!in) 
//     {
//         std::cerr << "Failed to open file: " << source << std::endl;
//         return *this; 
//     }

//     // десериализация list documents
//     size_t doc_count;
//     in.read(reinterpret_cast<char*>(&doc_count), sizeof(doc_count));
    
//     m_documents.clear();
//     for (size_t i = 0; i < doc_count; ++i) 
//     {
//         size_t length;
//         in.read(reinterpret_cast<char*>(&length), sizeof(length));
        
//         std::string doc(length, '0'); 
//         in.read(&doc[0], length);
//         m_documents.push_back(doc);
//     }

//     // десериализация hash_map index
//     size_t index_size;
//     in.read(reinterpret_cast<char*>(&index_size), sizeof(index_size));
    
//     m_index.clear();
//     for (size_t i = 0; i < index_size; ++i) 
//     {
//         size_t key_length;
//         in.read(reinterpret_cast<char*>(&key_length), sizeof(key_length));
        
//         std::string key(key_length, '0');
//         in.read(&key[0], key_length);
        
//         size_t value_size;
//         in.read(reinterpret_cast<char*>(&value_size), sizeof(value_size));

//         std::list<int> values;
//         for (size_t j = 0; j < value_size; ++j) 
//         {
//             int val;
//             in.read(reinterpret_cast<char*>(&val), sizeof(val));
//             values.push_back(val);
//         }
        
//         m_index[key] = values; 
//     }

//     in.close();

//     return *this; 
// }

// InvertedIndex& InvertedIndex::readFromDisk(const std::string& file_name)
// {
//     static InvertedIndex invertedIndex{};

//     std::ifstream construct_file;
//     construct_file.open(file_name);

//     char c;
//     if (!construct_file.is_open()) 
//         return invertedIndex;
    
//     else if (!(construct_file >> c))
//         return invertedIndex;
    
//     return invertedIndex.deserialize(file_name);
// }

MimeType InvertedIndex::get_mime_type_of_document(const std::string& file_name) const noexcept
{
    if (file_name.starts_with("http://") || file_name.starts_with("https://")) return MimeType::WEB;
    else if (file_name.ends_with(".htm") || file_name.ends_with(".html")) return MimeType::HTML;
    else if (file_name.ends_with(".txt")) return MimeType::TEXT;

    return MimeType::OTHER;
}

std::string InvertedIndex::connect_by_url(const std::string& url)
{
    CURL* curl;
    CURLcode res;
    std::string read_buffer;

    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if(res != CURLE_OK) 
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }
    
    return read_buffer;
}

std::string InvertedIndex::normalize(const std::string& term) const 
{
    auto cleanWord = [](const std::string& word) 
    {
        std::string cleaned;
        std::copy_if(word.begin(), word.end(), std::back_inserter(cleaned),
                     [](char c) { return isalnum(c); }); 
        std::transform(cleaned.begin(), cleaned.end(), cleaned.begin(), 
                       [](unsigned char c) { return std::tolower(c); }); 
        return cleaned;
    };

    const char* algorithm = "english"; 
    const char* charenc = nullptr; 

    sb_stemmer* stemmer = sb_stemmer_new(algorithm, charenc);
    if (stemmer == nullptr) 
        throw std::runtime_error("Error creating stemmer");
        
    std::string cleanedWord = cleanWord(term); 

    const char* word = cleanedWord.c_str();
    size_t wordLength = strlen(word);

    // Стэмминг слова
    const void* stemmedWord = sb_stemmer_stem(stemmer, reinterpret_cast<unsigned char*>(const_cast<char*>(word)), wordLength);
    size_t stemmedLength = sb_stemmer_length(stemmer);

    std::string result(reinterpret_cast<const char*>(stemmedWord), stemmedLength);
    
    sb_stemmer_delete(stemmer);

    return result;
}

void InvertedIndex::add_word_to_index(const std::string& word, int doc_id)
{
    if (!word.empty())
    {   
        Term term(doc_id);
        term.addDocument(doc_id);

        m_index.insert(std::make_pair(word, term));

        m_count_tokens++;
    }
}

void InvertedIndex::log_document(const std::string& file_name, int doc_id) const noexcept
{
    std::cout << std::left 
              << "| " << std::setw(6) << doc_id 
              << " | " << std::setw(80) << file_name
              << " | " << std::setw(9) << m_index.size() << " |" << std::endl;
}

void InvertedIndex::log_top_table() const noexcept
{
    std::cout << "+--------+----------------------------------------------------------------------------------+-----------+" << std::endl;
    std::cout << "| docID  | file                                                                             | indexsize |" << std::endl;
    std::cout << "+--------+----------------------------------------------------------------------------------+-----------+" << std::endl;
}
 
void InvertedIndex::log_bottom_table() const noexcept
{
    std::cout << "+--------+----------------------------------------------------------------------------------+-----------+" << std::endl;
}
