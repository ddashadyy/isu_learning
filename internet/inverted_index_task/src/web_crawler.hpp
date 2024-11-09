#pragma once

#include <string>
#include <unordered_set>
#include <curl/curl.h>
#include <gumbo.h>

#include "inverted_index.hpp"

class WebCrawler 
{
public:
    WebCrawler(int maxDepth) : m_depth(maxDepth) {}
    void crawl(const std::string& url, int startDepth);

    InvertedIndex& getInvertedIndex();

private:
    std::string fetchURL(const std::string& url);
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

    std::unordered_set<std::string> extractLinks(const std::string& html);
    void searchForLinks(GumboNode* node, std::unordered_set<std::string>& links);


    void indexHTML(const std::string& html);

    int m_depth;    
    InvertedIndex m_inverted_index;
};
