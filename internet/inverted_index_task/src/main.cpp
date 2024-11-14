#include <iostream>
#include <string>
#include <curl/curl.h>
#include <gumbo.h>
#include <memory>
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
    // WebCrawler wc("https://github.com/", 2);
    // wc.crawl();
    try
	{
		auto deserializedInvertedIndexPtr = std::make_unique<InvertedIndex>(InvertedIndex::readFromDisk("/home/gennadiy/third_course/internet/inverted_index_task/serialized"));

		deserializedInvertedIndexPtr->indexCollection("/home/collection");
		deserializedInvertedIndexPtr->serialize("/home/gennadiy/third_course/internet/inverted_index_task/serialized");

        std::cout << "Brutus OR Calpurnia " << deserializedInvertedIndexPtr->executeQuery("Brutus OR Calpurnia") << std::endl;
        std::cout << "Caesar OR Romeo OR Hamlet " << deserializedInvertedIndexPtr->executeQuery("Caesar OR Romeo OR Hamlet") << std::endl;
        std::cout << "( Brutus OR Calpurnia ) AND ( Caesar OR Romeo OR Hamlet ) " << deserializedInvertedIndexPtr->executeQuery("( Brutus OR Calpurnia ) AND ( Caesar OR Romeo OR Hamlet )") << std::endl;

	}
	catch (const std::ios_base::failure& e)
	{
		std::cerr << e.what() << '\n';
	}

    return EXIT_SUCCESS;
}