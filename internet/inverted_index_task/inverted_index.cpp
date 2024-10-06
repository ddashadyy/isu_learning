#include "inverted_index.hpp"
#include <iostream>
#include <fstream>

InvertedIndex::InvertedIndex()
{

}

void InvertedIndex::indexDocument(const std::string& path)
{

}

void InvertedIndex::indexCollection(const std::string& path)
{

}

std::list<int> InvertedIndex::executeQuery(const std::string& query)
{
    return std::list<int>();
}

std::list<int> InvertedIndex::getIntersection(const std::list<int>& l1, const std::list<int>& l2)
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

std::list<int> InvertedIndex::getUnion(const std::list<int>& l1, const std::list<int>& l2)
{
    std::list<int> result_of_union;

    auto it1 = l1.begin();
    auto it2 = l2.begin();

    while (it1 != l1.end() && it2 != l2.end()) 
    {
        if (*it1 == *it2) {
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

std::string InvertedIndex::normalize(const std::string& term)
{
    std::string normalized{};
    for (char c : term) 
        if (isalnum(c)) normalized.push_back(tolower(c));

    return normalized;
}
