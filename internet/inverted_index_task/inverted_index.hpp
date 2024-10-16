#pragma once 

#include <string>
#include <list>
#include <unordered_map>
#include "serialization.hpp"


class InvertedIndex : IByteSerialize<InvertedIndex>
{
public:
    InvertedIndex() = default;
    InvertedIndex( InvertedIndex&& other );
    ~InvertedIndex() = default;

    void indexDocument( const std::string& path );
    void indexCollection( const std::string& folder );
    std::list<int> executeQuery( const std::string& query );
    
    void serialize( const std::string& destination ) override final;
    InvertedIndex& deserialize( const std::string& source ) override final;

private:
    std::list<int> get_intersection( const std::list<int>& l1, const std::list<int>& l2 ) const;
    std::list<int> get_union( const std::list<int>& l1, const std::list<int>& l2 ) const;

    std::string normalize( const std::string& term ) const;
    std::string to_lower_case( std::string& word ) const;
    void add_word_too_index( const std::string& word, int docId );

    std::list<std::string> m_documents;
    std::unordered_map<std::string, std::list<int>> m_index;
};