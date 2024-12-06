#include "web_crawler.hpp"

void WebCrawler::crawl()
{
    std::set<std::string> visited;
    crawlHelper(startUrl, maxDepth, visited);
}

size_t WebCrawler::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp)
{
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

void WebCrawler::searchForLinks(GumboNode* node, std::set<std::string>& links)
{
    if (node->type != GUMBO_NODE_ELEMENT) return;

    if (node->v.element.tag == GUMBO_TAG_A) 
    {
        GumboAttribute* href = gumbo_get_attribute(&node->v.element.attributes, "href");
        if (href)
            links.insert(href->value);
    }

    for (unsigned int i = 0; i < node->v.element.children.length; ++i) 
        searchForLinks(static_cast<GumboNode*>(node->v.element.children.data[i]), links);
}

std::string WebCrawler::resolveUrl(const std::string& baseUrl, const std::string& relativeUrl)
{
    if (relativeUrl.empty()) return "";

    if (relativeUrl[0] == '/') 
        // Относительная ссылка начинается с /
        return baseUrl.substr(0, baseUrl.find("/", 8)) + relativeUrl;
    
    if (relativeUrl.substr(0, 4) == "http") 
        // Это абсолютный URL
        return relativeUrl;
    
    return baseUrl + relativeUrl;
}

void WebCrawler::crawlHelper(const std::string& url, int depth, std::set<std::string>& visited)
{
    if (depth == 0 || visited.count(url) > 0) return;
    
    visited.insert(url);
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
    searchForLinks(output->root, links);
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    for (const auto& link : links) 
    {
        std::string absoluteLink = resolveUrl(url, link); 

        indexDocument(absoluteLink);
        crawlHelper(absoluteLink, depth - 1, visited); 
    }
}
