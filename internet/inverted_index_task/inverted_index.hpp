#pragma once 

#include <string>
#include <list>
#include <unordered_map>



class InvertedIndex
{
public:
    InvertedIndex() = default;
    ~InvertedIndex() = default;

    void indexDocument(const std::string& path);
    void indexCollection(const std::string& folder);
    std::list<int> executeQuery(const std::string& query);
    
private:
    std::list<int> getIntersection(const std::list<int>& l1, const std::list<int>& l2);
    std::list<int> getUnion(const std::list<int>& l1, const std::list<int>& l2);

    std::string normalize(const std::string& term);
    std::string to_lower_case(std::string& word);
    void add_word_too_index(const std::string& word, int docId);

    std::list<std::string> m_documents;
    std::unordered_map<std::string, std::list<int>> m_index;
};