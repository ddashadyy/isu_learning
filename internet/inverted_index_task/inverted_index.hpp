#pragma once 

#include <list>
#include <string>
#include <vector>
#include <unordered_map>
#include "serialization.hpp"



class InvertedIndex final : IByteSerialize<InvertedIndex>
{
public:
    InvertedIndex() = default;
    InvertedIndex( const InvertedIndex& other );
    InvertedIndex( InvertedIndex&& other );
    ~InvertedIndex() = default;

    void indexDocument( const std::string& path );
    void indexCollection( const std::string& folder );
    std::list<int> executeQuery( const std::string& query );
    
    void serialize( const std::string& destination ) override;
    InvertedIndex& deserialize( const std::string& source ) override;

    static InvertedIndex& readFromDisk(const std::string& file_name);

private:
    std::list<int> get_intersection( const std::list<int>& l1, const std::list<int>& l2 ) const noexcept;
    std::list<int> get_union( const std::list<int>& l1, const std::list<int>& l2 ) const noexcept;

    std::string normalize( const std::string& term ) const noexcept;
    void add_word_to_index( const std::string& word, int doc_id );

    void log_document(const std::string& file_name, int doc_id) const noexcept;
    void log_top_table() const noexcept;
    void log_bottom_table() const noexcept;

    int count_character(const std::string& str, char ch) const noexcept;
    
    std::list<std::string> m_documents{};
    std::unordered_map<std::string, std::list<int>> m_index{};     
    // добавить хэш мапу для буфера и правильно ее сериализовать и десериализовать
};

