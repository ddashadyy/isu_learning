#pragma once 

#include "inverted_index.hpp"
#include <set>

class WebCrawler : public InvertedIndex
{
public:
    WebCrawler(size_t maxDepth) : m_max_depth(maxDepth) {}
    ~WebCrawler() = default;
    void crawl(const std::string& url, int depth = 0);

private:
    
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
    void searchForLinks(GumboNode* node, const std::string& baseUrl);


    size_t m_max_depth;
    std::set<std::string> m_visited;
};
