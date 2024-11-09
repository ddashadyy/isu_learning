#include <iostream>
#include <list>
#include "inverted_index.hpp"
#include "web_crawler.hpp"

std::ostream& operator << (std::ostream& os, const std::list<int> lst)
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
    WebCrawler wc(3);
    wc.crawl("https://github.com/madilkhan002/C-Multi-Threaded-Web-Crawler/blob/master/multheaded_web_crawler.cpp", 0);

    return EXIT_SUCCESS;
}





