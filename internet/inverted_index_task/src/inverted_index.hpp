#pragma once 

#include <list>
#include <stack>
#include <memory>
#include <string>
#include <vector>
#include <gumbo.h>
#include <curl/curl.h>
#include <unordered_map>
#include <unordered_set>
#include "serialization.hpp"
#include "term.hpp"
#include "document_relevance.hpp"

enum class MimeType 
{
    TEXT,
    HTML,
    WEB,
    OTHER
};

class InvertedIndex // : IByteSerialize<InvertedIndex>
{
public:
    InvertedIndex() { curl_global_init(CURL_GLOBAL_DEFAULT); }
    InvertedIndex( const InvertedIndex& other );
    virtual ~InvertedIndex() { curl_global_cleanup(); };

    void indexDocument( const std::string& path );
    void indexCollection( const std::string& folder );

    std::vector<DocumentRelevance> executeQuery( const std::string& query );
    std::vector<DocumentRelevance> executeQuery( const std::string& query, size_t n );
    void intersect( std::vector<DocumentRelevance>& answer, Term& term );
    void printResult(const std::string& query);
    
    // void serialize( const std::string& destination ) override;
    // InvertedIndex& deserialize( const std::string& source ) override;

    // static InvertedIndex& readFromDisk(const std::string& file_name);

private:
    MimeType get_mime_type_of_document( const std::string& file_name ) const noexcept;

    std::string normalize( const std::string& term ) const;
    void add_word_to_index( const std::string& word, int doc_id );

    void log_document( const std::string& file_name, int doc_id ) const noexcept;
    void log_top_table() const noexcept;
    void log_bottom_table() const noexcept;

    void extract_text( GumboNode* node, std::string& output );
    static size_t WriteCallback( void* contents, size_t size, size_t nmemb, std::string* userp );
    std::vector<std::string> splitString( const std::string& line ) noexcept;
    
    std::list<std::string> m_documents{};
    std::unordered_map<std::string, Term> m_index{};  

protected:
    std::string connect_by_url( const std::string& url );
};



