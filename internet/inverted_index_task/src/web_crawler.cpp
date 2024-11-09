#include "web_crawler.hpp"
#include <iostream>

void WebCrawler::crawl(const std::string& url, int startDepth)
{
    if (startDepth > m_depth) return;

    std::string htmlContent = fetchURL(url);
    if (htmlContent.empty()) return;    

    std::unordered_set<std::string> links = extractLinks(htmlContent);
    for (const auto& link : links)
    {
        std::cout << link << std::endl;
        m_inverted_index.indexHTMLByLink(link);
    }
        

    for (const auto& link : links)
        crawl(link, startDepth + 1);
}

InvertedIndex &WebCrawler::getInvertedIndex()
{
    return m_inverted_index;
}

std::string WebCrawler::fetchURL(const std::string &url)
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) 
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return readBuffer;
}

size_t WebCrawler::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::unordered_set<std::string> WebCrawler::extractLinks(const std::string& html)
{
    std::unordered_set<std::string> links;
    GumboOutput* output = gumbo_parse(html.c_str());

    searchForLinks(output->root, links);
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    return links;
}

void WebCrawler::searchForLinks(GumboNode* node, std::unordered_set<std::string>& links)
{
    // Проверяем, является ли узел элементом
    if (node->type == GUMBO_NODE_ELEMENT) {
        // Проверяем, является ли это тегом <a>
        if (node->v.element.tag == GUMBO_TAG_A) {
            // Получаем атрибут href
            for (size_t i = 0; i < node->v.element.attributes.length; ++i) {
                GumboAttribute* attr = static_cast<GumboAttribute*>(node->v.element.attributes.data[i]);
                if (attr && attr->name == "href") {
                    links.insert(attr->value); // Добавляем ссылку в набор
                }
            }
        }
    }

    // Рекурсивно проходим по всем дочерним узлам
    if (node->type == GUMBO_NODE_ELEMENT && node->v.element.children.length > 0) {
        for (size_t i = 0; i < node->v.element.children.length; ++i) {
            searchForLinks(static_cast<GumboNode*>(node->v.element.children.data[i]), links);
        }
    }
}
