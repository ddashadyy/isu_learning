// #include <iostream>
// #include <list>
// #include "inverted_index.hpp"
// #include "web_crawler.hpp"
// #include "curl/curl.h"


// std::ostream& operator << (std::ostream& os, const std::list<int> lst)
// {
//     os.put('[');
//     os.put(' ');
//     for (auto it = lst.begin(); it != lst.end(); ++it)
//         os << *it << " ";
//     os.put(']');

//     return os;
// }





// int main()
// {
//     // InvertedIndex ii;

//     // ii.indexCollection("/home/gennadiy/collection_links.txt", "web_page");

//     curl_global_init(CURL_GLOBAL_DEFAULT);

//     WebCrawler wc(3);
//     wc.crawl("https://raspmath.isu.ru/schedule/23");

//     // ii.indexCollection("/home/collection_html");

//     // std::cout << "Brutus " << ii.executeQuery("Brutus") << std::endl;
//     // std::cout << "Caesar " << ii.executeQuery("Caesar") << std::endl;
//     // std::cout << "Calpurnia " << ii.executeQuery("Calpurnia") << std::endl;
//     // std::cout << "Brutus AND Brutus " << ii.executeQuery("Brutus AND Brutus") << std::endl;
//     // std::cout << "Brutus AND Caesar " << ii.executeQuery("Brutus AND Caesar") << std::endl;
//     // std::cout << "Brutus AND Caesar AND Calpurnia " << ii.executeQuery("Brutus AND Caesar AND Calpurnia") << std::endl;
//     // std::cout << "Brutus OR Brutus " << ii.executeQuery("Brutus OR Brutus") << std::endl;
//     // std::cout << "Brutus OR Caesar " << ii.executeQuery("Brutus OR Caesar") << std::endl;
//     // std::cout << "Brutus OR Caesar OR Calpurnia " << ii.executeQuery("Brutus OR Caesar OR Calpurnia") << std::endl;
//     // std::cout << "SpiderMan " << ii.executeQuery("SpiderMan") << std::endl;
//     // std::cout << "Brutus AND SpiderMan " << ii.executeQuery("Brutus AND SpiderMan") << std::endl;
//     // std::cout << "Caesar OR SpiderMan " << ii.executeQuery("Caesar OR SpiderMan") << std::endl;

//     // ii.indexHTMLByLink("https://dota2protracker.com/hero/Invoker");

    



//     return EXIT_SUCCESS;
// }



#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <curl/curl.h>
#include <gumbo.h>
#include <regex>
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

int main() {
    // curl_global_init(CURL_GLOBAL_DEFAULT);

    // std::string startUrl = "https://raspmath.isu.ru/schedule/23"; // Замените на нужный URL
    // int maxDepth = 2; // Задайте максимальную глубину индексации
    // std::set<std::string> visited;

    // crawl(startUrl, maxDepth, visited);

    // curl_global_cleanup();

    // std::set<std::string> visited;
    // WebCrawler wc(3, visited);
    // wc.crawl("https://raspmath.isu.ru/schedule/23", 2, wc.getVisited());


    // WebCrawler wc("https://raspmath.isu.ru/schedule/23", 2);
    // wc.crawl();



    InvertedIndex ii;
    ii.indexHTMLByLink("https://raspmath.isu.ru/schedule/23");

    std::cout << "scheduleimei@mail.ru " << ii.executeQuery("scheduleimei@mail.ru") << std::endl;

    return 0;
}