#include "inverted_index.hpp"
#include <ios>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <cctype>
#include <algorithm>
#include <vector>
#include <regex>
#include <curl/curl.h>


InvertedIndex::InvertedIndex(const InvertedIndex &other)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    m_documents = other.m_documents;
    m_index = other.m_index;
}

InvertedIndex::InvertedIndex(InvertedIndex &&other)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    m_documents = std::move(other.m_documents);
    m_index = std::move(other.m_index);
}

void InvertedIndex::indexDocument(const std::string& path)
{
    // Исключаем повторную индексацию 
    auto find_it = std::find(m_documents.begin(), m_documents.end(), path);
    if (find_it != m_documents.end()) return;

    std::ifstream file(path);
    if (!file.is_open()) 
    {
        throw std::ios_base::failure("Could not open the file");
        return;
    }

    int doc_id = m_documents.size(); 
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

void InvertedIndex::indexHTML(const std::string& html)
{
    // Исключаем повторную индексацию 
    auto find_it = std::find(m_documents.begin(), m_documents.end(), html);
    if (find_it != m_documents.end()) return;


    std::ifstream html_file(html);
    if (!html_file.is_open())
    {
        throw std::ios_base::failure("Could not open the file");
    }

    int doc_id = m_documents.size();
    m_documents.push_back(html);

    std::stringstream buffer;
    buffer << html_file.rdbuf();

    html_file.close();
    
    std::string raw_html = buffer.str();
    
    GumboOutput* output = gumbo_parse(raw_html.c_str());
    std::string extracted_text;
    
    extract_text(output->root, extracted_text);
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    auto splitted_string = splitString(extracted_text);

    for (auto& word : splitted_string)
    {
        std::string normalized_word = normalize(word);
        if (!normalized_word.empty()) 
            add_word_to_index(normalized_word, doc_id);
    }
    
}

void InvertedIndex::indexHTMLByLink(const std::string& url)
{
    // Исключаем повторную индексацию 
    auto find_it = std::find(m_documents.begin(), m_documents.end(), url);
    if (find_it != m_documents.end()) return;

    int doc_id = m_documents.size();
    m_documents.push_back(url);

    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if(res != CURLE_OK) 
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return;
        }
    }

    GumboOutput* output = gumbo_parse(readBuffer.c_str());
    if (!output) 
    {
        std::cerr << "Failed to parse HTML." << std::endl;
        return;
    }

    std::string extracted_text;

    extract_text(output->root, extracted_text);
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    std::cout << extracted_text << std::endl;

    log_document(url, doc_id);

    auto splitted_string = splitString(extracted_text);
    
    for (auto& word : splitted_string)
    {
        std::string normalized_word = normalize(word);
        // std::cout << normalized_word << std::endl;
        if (!normalized_word.empty()) 
            add_word_to_index(normalized_word, doc_id);
    }
}

void InvertedIndex::extract_text(GumboNode* node, std::string& output)
{  
    if (node->type == GUMBO_NODE_TEXT) 
    {
        output.append(node->v.text.text);
        output.append(" ");
    }
    else if (node->type == GUMBO_NODE_ELEMENT) 
        for (unsigned int i = 0; i < node->v.element.children.length; ++i) 
            extract_text(static_cast<GumboNode*>(node->v.element.children.data[i]), output);
}

// Функция для обработки данных, полученных через curl
size_t InvertedIndex::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp)
{
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

std::vector<std::string> InvertedIndex::splitString(std::string& line) noexcept
{
    std::regex regex("[^a-zA-Z]+");
    
    std::sregex_token_iterator iter(line.begin(), line.end(), regex, -1);
    std::sregex_token_iterator end;
    
    std::vector<std::string> result;
    for (; iter != end; ++iter) 
    {
        if (!iter->str().empty()) 
        { 
            result.push_back(*iter);
            
        }
    }
    
    return result;
}

void InvertedIndex::indexCollection(const std::string& folder, const std::string& index_type)
{
    if (index_type == "web_page")
    {
        int counter = 0;
        log_top_table();

        // Исключаем повторную индексацию 
        auto find_it = std::find(m_documents.begin(), m_documents.end(), folder);
        if (find_it != m_documents.end()) return;

        std::ifstream file(folder);
        if (!file.is_open()) 
        {
            throw std::ios_base::failure("Could not open the file");
            return;
        }

        int doc_id = m_documents.size(); 
        m_documents.push_back(folder); 

        std::string line{};
        while (std::getline(file, line)) 
        {
            std::istringstream iss(line);
            indexHTMLByLink(iss.str());

            log_document(iss.str(), counter);
        }
        log_bottom_table();
    }
                
    else 
    {
        namespace fs = std::filesystem;

        int counter = 0;
        log_top_table();

        const fs::path dir(folder);
        if (!fs::exists(dir)) return; 

        for (const auto& entry : fs::directory_iterator(dir)) 
        {
            // recursive traversal
            if (fs::is_directory(entry)) indexCollection(entry.path().string(), index_type);

            if (fs::is_regular_file(entry.status())) 
            {
                const std::string key = entry.path().string();
                
                if (index_type == "document")
                    indexDocument(key);
                else if (index_type == "html")
                    indexHTML(key); 

                log_document(key, counter);
                counter++;
            }
        }

        log_bottom_table();
    }

}

std::list<int> InvertedIndex::executeQuery(const std::string& query) 
{
    std::list<int> executed_query;
    std::string temp{};

    std::istringstream stream(query);
    std::string word{};

    while (stream >> word)
    {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);

        if (temp.empty() || temp == "or") 
            executed_query = get_union(executed_query, m_index[word]);
        else if (temp == "and")
            executed_query = get_intersection(executed_query, m_index[word]);

        temp = word;
    }

    return executed_query;
}

void InvertedIndex::serialize(const std::string& destination)
{
    std::ofstream out(destination, std::ios::binary);
    
    // Сериализация list m_documents
    size_t doc_count = m_documents.size();
    out.write(reinterpret_cast<const char*>(&doc_count), sizeof(doc_count));
    
    for (const auto& docs : m_documents) 
    {
        size_t length = docs.size();
        out.write(reinterpret_cast<const char*>(&length), sizeof(length));
        out.write(docs.c_str(), length);
    }
    
    // Сериализация hash_map m_index
    size_t index_size = m_index.size();
    out.write(reinterpret_cast<const char*>(&index_size), sizeof(index_size));
    
    for (const auto& [key, value] : m_index) 
    {
        size_t key_length = key.size();
        out.write(reinterpret_cast<const char*>(&key_length), sizeof(key_length));
        out.write(key.c_str(), key_length);
        
        size_t value_size = value.size();
        out.write(reinterpret_cast<const char*>(&value_size), sizeof(value_size));
        
        for (const auto& val : value) 
        {
            out.write(reinterpret_cast<const char*>(&val), sizeof(val));
        }
    }
    
    out.close();
}

InvertedIndex& InvertedIndex::deserialize(const std::string& source)
{
    std::ifstream in(source, std::ios::binary);
    if (!in) 
    {
        std::cerr << "Failed to open file: " << source << std::endl;
        return *this; 
    }

    // десериализация list documents
    size_t doc_count;
    in.read(reinterpret_cast<char*>(&doc_count), sizeof(doc_count));
    
    m_documents.clear();
    for (size_t i = 0; i < doc_count; ++i) 
    {
        size_t length;
        in.read(reinterpret_cast<char*>(&length), sizeof(length));
        
        std::string doc(length, '0'); 
        in.read(&doc[0], length);
        m_documents.push_back(doc);
    }

    // десериализация hash_map index
    size_t index_size;
    in.read(reinterpret_cast<char*>(&index_size), sizeof(index_size));
    
    m_index.clear();
    for (size_t i = 0; i < index_size; ++i) 
    {
        size_t key_length;
        in.read(reinterpret_cast<char*>(&key_length), sizeof(key_length));
        
        std::string key(key_length, '0');
        in.read(&key[0], key_length);
        
        size_t value_size;
        in.read(reinterpret_cast<char*>(&value_size), sizeof(value_size));

        std::list<int> values;
        for (size_t j = 0; j < value_size; ++j) 
        {
            int val;
            in.read(reinterpret_cast<char*>(&val), sizeof(val));
            values.push_back(val);
        }
        
        m_index[key] = values; 
    }

    in.close();

    return *this; 
}

InvertedIndex& InvertedIndex::readFromDisk(const std::string& file_name)
{
    static InvertedIndex invertedIndex{};

    std::ifstream construct_file;
    construct_file.open(file_name);

    char c;
    if (!construct_file.is_open()) 
        return invertedIndex;
    
    else if (!(construct_file >> c))
        return invertedIndex;
    
    return invertedIndex.deserialize(file_name);
}

std::list<int> InvertedIndex::get_intersection(const std::list<int>& l1, const std::list<int>& l2) const noexcept
{
    std::list<int> result_of_intersection;

    auto it1 = l1.begin();
    auto it2 = l2.begin();

    while (it1 != l1.end() && it2 != l2.end())
    {
        if (*it1 == *it2)
        {
            result_of_intersection.push_back(*it1);
            ++it1;
            ++it2;
        }
        else if (*it1 < *it2) ++it1;
        else ++it2;
    }

    return result_of_intersection;
}

std::list<int> InvertedIndex::get_union(const std::list<int>& l1, const std::list<int>& l2) const noexcept
{
    std::list<int> result_of_union;

    auto it1 = l1.begin();
    auto it2 = l2.begin();

    while (it1 != l1.end() && it2 != l2.end()) 
    {
        if (*it1 == *it2) 
        {
            result_of_union.push_back(*it1);
            ++it1;
            ++it2;
        } 
        else if (*it1 < *it2) 
        {
            result_of_union.push_back(*it1);
            ++it1;
        }

        else 
        {
            result_of_union.push_back(*it2);
            ++it2;
        }
    }

    while (it1 != l1.end()) 
    {
        result_of_union.push_back(*it1);
        ++it1;
    }

    while (it2 != l2.end()) 
    {
        result_of_union.push_back(*it2);
        ++it2;
    }

    return result_of_union;
}

std::string InvertedIndex::normalize(const std::string& term) const noexcept
{
    char ap = 39;
    if (count_character(term, ap) == 2)
        return term.substr(term.find_first_of(ap) + 1, term.find_last_of(ap) - 2);

    else
    {
	    std::string normalized{};

        for (const auto c : term)
        {
            if (!std::isalpha(c)) break;
            else normalized.push_back(std::tolower(c));
        }
        return normalized;
    }

    // std::string normalized_word;
    // std::remove_copy_if(term.begin(), term.end(), std::back_inserter(normalized_word),
    //                     [](unsigned char c) { return std::ispunct(c); });
    // std::transform(normalized_word.begin(), normalized_word.end(), normalized_word.begin(),
    //                [](unsigned char c) { return std::tolower(c); });
    // return normalized_word;

}

void InvertedIndex::add_word_to_index(const std::string &word, int doc_id)
{
    if (!word.empty()) 
    {
        auto& postings = m_index[word];
        if (postings.empty() || postings.back() != doc_id)
            postings.push_back(doc_id);   
    }
}

void InvertedIndex::log_document(const std::string& file_name, int doc_id) const noexcept
{
    std::cout << std::left 
              << "| " << std::setw(6) << doc_id 
              << " | " << std::setw(60) << file_name
              << " | " << std::setw(9) << m_index.size() << " |" << std::endl;
}

void InvertedIndex::log_top_table() const noexcept
{
    std::cout << "+--------+--------------------------------------------------------------+-----------+" << std::endl;
    std::cout << "| docID  | file                                                         | indexsize |" << std::endl;
    std::cout << "+--------+--------------------------------------------------------------+-----------+" << std::endl;
}
 
void InvertedIndex::log_bottom_table() const noexcept
{
    std::cout << "+--------+--------------------------------------------------------------+-----------+" << std::endl;
}

int InvertedIndex::count_character(const std::string &str, char ch) const noexcept
{
    int count = 0;
    for (char c : str) {
        if (c == ch) {
            count++;
        }
    }
    return count;
}
