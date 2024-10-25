#include "inverted_index.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <cctype>
#include <algorithm>




InvertedIndex::InvertedIndex(const InvertedIndex &other)
{
    m_documents = other.m_documents;
    m_index = other.m_index;
}

InvertedIndex::InvertedIndex(InvertedIndex &&other)
{
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

void InvertedIndex::indexCollection(const std::string& folder)
{
    namespace fs = std::filesystem;
    int counter = 0;

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
            log_document(key, counter);
            counter++;
        }
    }

    log_bottom_table();
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