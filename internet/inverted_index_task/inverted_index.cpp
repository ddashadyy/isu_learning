#include "inverted_index.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <cctype>
#include <algorithm>

void InvertedIndex::indexDocument(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) 
    {
        std::cerr << "Could not open the file: " << path << std::endl;
        return;
    }
    
    std::string line{};
    m_documents.push_back(path); 
    int doc_id = m_documents.size(); 

    while (std::getline(file, line)) 
    {
        std::istringstream iss(line);
        std::string word{};

        while (iss >> word) 
        {
            word = normalize(word);
            if (!word.empty()) 
                add_word_too_index(word, doc_id);   
        }
    }
}

void InvertedIndex::indexCollection(const std::string& folder)
{
    namespace fs = std::filesystem;

    fs::path dir(folder);
    if (!fs::exists(dir) || !fs::is_directory(dir)) return; 

    for (const auto& entry : fs::directory_iterator(dir)) 
    {
        if (fs::is_regular_file(entry.status())) 
        {
            std::string key = entry.path().string();
            std::cout << key << std::endl;

            indexDocument(key);       
        }
    }
    std::cout << "\n";
}

std::list<int> InvertedIndex::executeQuery(const std::string& query)
{
    std::list<int> executed_query;
    std::string temp{};

    std::istringstream stream(query);
    std::string word{};

    while (stream >> word)
    {
        word = to_lower_case(word);

        if (temp.empty() || temp == "or") 
            executed_query = getUnion(executed_query, m_index[word]);
        else if (temp == "and")
            executed_query = getIntersection(executed_query, m_index[word]);

        temp = word;
    }

    return executed_query;
}

std::list<int> InvertedIndex::getIntersection(const std::list<int>& l1, const std::list<int>& l2) const
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

std::list<int> InvertedIndex::getUnion(const std::list<int>& l1, const std::list<int>& l2) const
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

std::string InvertedIndex::normalize(const std::string& term) const
{
    std::string normalized{};
    for (unsigned char c : term) 
        if (std::isalpha(c)) normalized.push_back(std::tolower(c));

    return normalized;
}

std::string InvertedIndex::to_lower_case(std::string& word) const
{
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    return word;
}

void InvertedIndex::add_word_too_index(const std::string &word, int docId)
{
    if (!word.empty()) 
    {
        auto& postings = m_index[word];
        if (postings.empty() || postings.back() != docId)
            postings.push_back(docId);
        
    }
}
