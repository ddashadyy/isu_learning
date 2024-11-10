#include <iostream>
#include <list>
#include "inverted_index.hpp"
#include "web_crawler.hpp"
#include "curl/curl.h"


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
    // InvertedIndex ii;

    // ii.indexCollection("/home/gennadiy/collection_links.txt", "web_page");

    curl_global_init(CURL_GLOBAL_DEFAULT);

    WebCrawler wc(3);
    wc.crawl("https://raspmath.isu.ru/schedule/23");

    // ii.indexCollection("/home/collection_html");

    // std::cout << "Brutus " << ii.executeQuery("Brutus") << std::endl;
    // std::cout << "Caesar " << ii.executeQuery("Caesar") << std::endl;
    // std::cout << "Calpurnia " << ii.executeQuery("Calpurnia") << std::endl;
    // std::cout << "Brutus AND Brutus " << ii.executeQuery("Brutus AND Brutus") << std::endl;
    // std::cout << "Brutus AND Caesar " << ii.executeQuery("Brutus AND Caesar") << std::endl;
    // std::cout << "Brutus AND Caesar AND Calpurnia " << ii.executeQuery("Brutus AND Caesar AND Calpurnia") << std::endl;
    // std::cout << "Brutus OR Brutus " << ii.executeQuery("Brutus OR Brutus") << std::endl;
    // std::cout << "Brutus OR Caesar " << ii.executeQuery("Brutus OR Caesar") << std::endl;
    // std::cout << "Brutus OR Caesar OR Calpurnia " << ii.executeQuery("Brutus OR Caesar OR Calpurnia") << std::endl;
    // std::cout << "SpiderMan " << ii.executeQuery("SpiderMan") << std::endl;
    // std::cout << "Brutus AND SpiderMan " << ii.executeQuery("Brutus AND SpiderMan") << std::endl;
    // std::cout << "Caesar OR SpiderMan " << ii.executeQuery("Caesar OR SpiderMan") << std::endl;

    // ii.indexHTMLByLink("https://dota2protracker.com/hero/Invoker");

    



    return EXIT_SUCCESS;
}



// #include <iostream>
// #include <string>
// #include <vector>
// #include <set>
// #include <curl/curl.h>
// #include <gumbo.h>
// #include <regex>

// size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
//     size_t totalSize = size * nmemb;
//     userp->append((char*)contents, totalSize);
//     return totalSize;
// }

// void searchForLinks(GumboNode* node, const std::string& baseUrl, std::set<std::string>& links) {
//     if (node->type != GUMBO_NODE_ELEMENT) {
//         return;
//     }

//     if (node->v.element.tag == GUMBO_TAG_A) {
//         GumboAttribute* href = gumbo_get_attribute(&node->v.element.attributes, "href");
//         if (href) {
//             std::string link = href->value;

//             // Проверка на фрагменты
//             if (link.empty() || link[0] == '#') {
//                 return; // Игнорируем фрагменты
//             }

//             // Преобразование относительных ссылок в абсолютные
//             if (link[0] == '/') {
//                 link = baseUrl + link; // Добавляем базовый URL
//             } else if (link.find("http://") == std::string::npos && link.find("https://") == std::string::npos) {
//                 // Если это не абсолютная ссылка, добавляем базовый URL
//                 link = baseUrl + "/" + link;
//             }

//             links.insert(link);
//         }
//     }

//     for (unsigned int i = 0; i < node->v.element.children.length; ++i) {
//         searchForLinks(static_cast<GumboNode*>(node->v.element.children.data[i]), baseUrl, links);
//     }
// }

// void crawl(const std::string& url, int depth, std::set<std::string>& visited) {
//     if (depth == 0 || visited.count(url) > 0) {
//         return;
//     }

//     visited.insert(url);
//     std::string pageContent;

//     CURL* curl = curl_easy_init();
//     if (curl) {
//         curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
//         curl_easy_setopt(curl, CURLOPT_WRITEDATA, &pageContent);
//         curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        
//         CURLcode res = curl_easy_perform(curl);
//         if (res != CURLE_OK) {
//             std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
//         }
//         curl_easy_cleanup(curl);
//     }

//     GumboOutput* output = gumbo_parse(pageContent.c_str());
//     std::set<std::string> links;
    
//     // Извлечение базового URL
//     std::regex urlRegex(R"(^(https?://[^/]+))");
//     std::smatch match;
//     std::string baseUrl = url;
    
//     if (std::regex_search(url, match, urlRegex)) {
//         baseUrl = match[1].str(); // Получаем базовый URL
//     }

//     searchForLinks(output->root, baseUrl, links);
//     gumbo_destroy_output(&kGumboDefaultOptions, output);

//     // Печать ссылок
//     std::cout << "Links found on " << url << ":" << std::endl;
//     for (const auto& link : links) {
//         std::cout << link << std::endl;
//         crawl(link, depth - 1, visited); // Рекурсивный вызов для следующего уровня
//     }
// }

// int main() {
//     curl_global_init(CURL_GLOBAL_DEFAULT);

//     std::string startUrl = "https://raspmath.isu.ru/schedule/23"; // Замените на нужный URL
//     int maxDepth = 2; // Задайте максимальную глубину индексации
//     std::set<std::string> visited;

//     crawl(startUrl, maxDepth, visited);

//     curl_global_cleanup();

//     return 0;
// }