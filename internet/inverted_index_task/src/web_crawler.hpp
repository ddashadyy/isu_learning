#pragma once

#include <string>
#include <set>
#include <curl/curl.h>
#include <pugixml.hpp>

class WebCrawler 
{
public:
    WebCrawler(int maxDepth);
    void crawl(const std::string& url, int depth);

private:
    std::string fetchURL(const std::string& url);
    std::set<std::string> extractLinks(const std::string& hmtl);
    void indexHTML(const std::string& html);

    int m_depth;    

};
