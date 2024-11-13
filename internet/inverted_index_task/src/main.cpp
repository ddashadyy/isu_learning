#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <curl/curl.h>
#include <gumbo.h>
#include <regex>
#include "web_crawler.hpp"

std::ostream& operator << (std::ostream& os, const std::list<int>& lst)
{
    os.put('[');
    os.put(' ');
    for (auto it = lst.begin(); it != lst.end(); ++it)
        os << *it << " ";
    os.put(']');

    return os;
}

int main() 
{
    WebCrawler wc("https://github.com/", 2);
    wc.crawl();

    return EXIT_SUCCESS;
}