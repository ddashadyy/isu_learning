// #pragma once 

// #include "inverted_index.hpp"
// #include <set>

// class WebCrawler
// {
// public:
//     WebCrawler(size_t maxDepth, std::set<std::string> visited) : m_max_depth(maxDepth), m_visited(visited) {}
//     ~WebCrawler() = default;
//     void crawl(const std::string& url, int depth, std::set<std::string>& visited);

//     std::set<std::string>& getVisited() { return m_visited; }

// private:
    
//     static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
//     void searchForLinks(GumboNode* node, const std::string& baseUrl, std::set<std::string>& links);

//     void indexHMTLByLink();

//     size_t m_max_depth;
//     std::set<std::string> m_visited;
// };

#include <iostream>
#include <string>
#include <set>
#include <curl/curl.h>
#include <gumbo.h>
#include "inverted_index.hpp"

class WebCrawler : public InvertedIndex
{
public:
    WebCrawler(const std::string& startUrl, int maxDepth) : startUrl(startUrl), maxDepth(maxDepth) {}
    ~WebCrawler() {}
    void crawl();

private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
    void searchForLinks(GumboNode* node, std::set<std::string>& links);
    std::string resolveUrl(const std::string& baseUrl, const std::string& relativeUrl);
    void crawlHelper(const std::string& url, int depth, std::set<std::string>& visited);

    std::string startUrl;
    int maxDepth;
};


