#pragma once 

#include <list>
#include <stack>
#include <string>
#include <vector>
#include <gumbo.h>
#include <curl/curl.h>
#include <unordered_map>
#include <unordered_set>
#include "serialization.hpp"

enum class MimeType 
{
    TEXT,
    HTML,
    WEB
};

class InvertedIndex : IByteSerialize<InvertedIndex>
{
public:
    InvertedIndex() { curl_global_init(CURL_GLOBAL_DEFAULT); }
    InvertedIndex(const std::string& stop_words_file_path);
    InvertedIndex( const InvertedIndex& other );
    InvertedIndex( InvertedIndex&& other );
    virtual ~InvertedIndex() { curl_global_cleanup(); };

    void indexDocument( const std::string& path );
    // void indexHTML( const std::string& html );
    // void indexHTMLByLink( const std::string& url );

    void indexCollection( const std::string& folder );

    std::list<int> executeQuery( const std::string& query );
    
    void serialize( const std::string& destination ) override;
    InvertedIndex& deserialize( const std::string& source ) override;

    static InvertedIndex& readFromDisk(const std::string& file_name);

private:
    std::list<int> get_intersection( const std::list<int>& l1, const std::list<int>& l2 ) const noexcept;
    std::list<int> get_union( const std::list<int>& l1, const std::list<int>& l2 ) const noexcept;
    MimeType get_mime_type_of_document( const std::string& file_name ) const noexcept;

    void proces_logical_operators( std::stack<std::string>& operators, std::stack<std::list<int>>& operands ) noexcept;

    std::string normalize( const std::string& term ) const;
    void add_word_to_index( const std::string& word, int doc_id );

    void log_document( const std::string& file_name, int doc_id ) const noexcept;
    void log_top_table() const noexcept;
    void log_bottom_table() const noexcept;

    void extract_text( GumboNode* node, std::string& output );
    static size_t WriteCallback( void* contents, size_t size, size_t nmemb, std::string* userp );
    std::vector<std::string> splitString( std::string& line ) noexcept;
    
    std::list<std::string> m_documents{};
    std::unordered_map<std::string, std::list<int>> m_index{};     
    std::unordered_set<std::string> m_stop_words{};
};



