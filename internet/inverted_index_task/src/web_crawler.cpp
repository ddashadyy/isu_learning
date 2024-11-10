#include "web_crawler.hpp"
#include <regex>
#include <iostream>

void WebCrawler::crawl(const std::string& url, int depth)
{
    if (depth > m_max_depth || m_visited.count(url) > 0) return;

    m_visited.insert(url);
    std::string pageContent;

    CURL* curl = curl_easy_init();
    if (curl) 
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &pageContent);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) 
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        
        curl_easy_cleanup(curl);
    }

    GumboOutput* output = gumbo_parse(pageContent.c_str());
    std::set<std::string> links;
    
    // Извлечение базового URL
    std::regex urlRegex(R"(^(https?://[^/]+))");
    std::smatch match;
    std::string baseUrl = url;
    
    if (std::regex_search(url, match, urlRegex)) 
        baseUrl = match[1].str(); 
    

    searchForLinks(output->root, baseUrl);
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    for (const auto& link : m_visited)
    {
        std::cout << link << std::endl;
        indexHTMLByLink(link);
        crawl(link, depth + 1);
    }
}

size_t WebCrawler::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp)
{
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

void WebCrawler::searchForLinks(GumboNode *node, const std::string &baseUrl)
{
    if (node->type != GUMBO_NODE_ELEMENT) return;

    if (node->v.element.tag == GUMBO_TAG_A) 
    {
        GumboAttribute* href = gumbo_get_attribute(&node->v.element.attributes, "href");
        if (href) 
        {
            std::string link = href->value;

            // Проверка на фрагменты
            if (link.empty() || link[0] == '#') return; 
            
            // Преобразование относительных ссылок в абсолютные
            if (link[0] == '/') 
                link = baseUrl + link; // Добавляем базовый URL
            
            else if (link.find("http://") == std::string::npos && link.find("https://") == std::string::npos) 
                link = baseUrl + "/" + link;
            
            m_visited.insert(link);
        }
    }

    for (unsigned int i = 0; i < node->v.element.children.length; ++i) 
        searchForLinks(static_cast<GumboNode*>(node->v.element.children.data[i]), baseUrl);
}