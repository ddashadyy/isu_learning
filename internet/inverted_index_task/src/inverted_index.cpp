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

InvertedIndex::InvertedIndex(const std::string& stop_words_file_path)
{
    std::ifstream stopWordsFile(stop_words_file_path);
    if (!stopWordsFile.is_open()) 
    {
        std::cerr << "Ошибка открытия файла со стоп-словами: " << stop_words_file_path << std::endl;
        return;
    }

    std::string word;
    while (stopWordsFile >> word) 
    {
        word = normalize(word);
        m_stop_words.insert(word);
    }
    stopWordsFile.close();
}

InvertedIndex::InvertedIndex(const InvertedIndex& other)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    m_documents = other.m_documents;
    m_index = other.m_index;
}

InvertedIndex::InvertedIndex(InvertedIndex&& other)
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
            if (m_stop_words.find(word) == m_stop_words.end())
                if (!word.empty()) 
                    add_word_to_index(word, doc_id);   
        }
    }
    file.close();
    
    log_document(path, doc_id);
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
        word = normalize(word);
        if (m_stop_words.find(word) == m_stop_words.end())
            if (!word.empty()) 
                add_word_to_index(word, doc_id);
    }

    log_document(html, doc_id);
    
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

    auto splitted_string = splitString(extracted_text);
    
    for (auto& word : splitted_string)
    {
        word = normalize(word);
        if (m_stop_words.find(word) == m_stop_words.end())
            if (!word.empty()) 
                add_word_to_index(word, doc_id);
    }
    log_document(url, doc_id);
}

void InvertedIndex::indexCollection(const std::string& folder, const std::string& index_type)
{
    if (index_type == "web_page")
    {
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

        m_documents.push_back(folder); 

        std::string line{};
        while (std::getline(file, line)) 
        {
            std::istringstream iss(line);
            indexHTMLByLink(iss.str());

            // log_document(iss.str(), counter);
        }
        log_bottom_table();
    }
                
    else 
    {
        namespace fs = std::filesystem;

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

                // log_document(key, counter);
            }
        }

        log_bottom_table();
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

// Функция для обработки данных, полученных через curl
size_t InvertedIndex::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp)
{
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

std::vector<std::string> InvertedIndex::splitString(std::string& line) noexcept
{
    std::vector<std::string> words;
    std::istringstream stream(line);
    std::string word;

    while (stream >> word) {
        words.push_back(word);
    }

    return words;
}

std::list<int> InvertedIndex::executeQuery(const std::string& query) 
{
    std::list<int> result;

    std::stack<std::string> operators;
    std::stack<std::list<int>> operands;

    std::istringstream ss(query);
    std::string token{};
    while (std::getline(ss, token, ' ')) 
    {
        token = normalize(token);

        if (token == "(") 
            operators.push(token);

        else if (token == ")") 
        {
            while (!operators.empty() && operators.top() != "(") 
                processLogicalOperators(operators, operands);
            operators.pop(); 
        } 
        else if (token == "and" || token == "or") 
            operators.push(token);
        else 
            if (m_stop_words.find(token) == m_stop_words.end())
                operands.push(m_index.at(token));
            else 
                operands.push(std::list<int>{});
    }

    while (!operators.empty()) 
        processLogicalOperators(operators, operands);
    
    result = operands.top();
    operands.pop();

    return result;
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

void InvertedIndex::processLogicalOperators(std::stack<std::string>& operators, std::stack<std::list<int>>& operands) noexcept
{
    std::string op = operators.top();
    operators.pop();

    auto second_operand = operands.top(); operands.pop();
    auto first_operand = operands.top(); operands.pop();
    
    if (op == "and")
        operands.push(get_intersection(first_operand, second_operand));
    else if (op == "or")
        operands.push(get_union(first_operand, second_operand));

}

std::string InvertedIndex::normalize(const std::string& term) const 
{
    auto cleanWord = [](const std::string& word) 
    {
        std::string cleaned;
        std::copy_if(word.begin(), word.end(), std::back_inserter(cleaned),
                     [](char c) { return c != '!' && c != '?' && c != '.' && c != ','; }); 
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

