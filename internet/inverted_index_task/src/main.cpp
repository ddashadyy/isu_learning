#include <iostream>
#include "inverted_index.hpp"
#include "web_crawler.hpp"


template< typename T >
std::ostream& operator << (std::ostream& os, const std::list<T>& lst)
{
	os.put('['); os.put(' '); 
    for (const auto& element : lst) os << element << " ";
	os.put(']');

    return os;
}


int main() 
{
	InvertedIndex ii;
	ii.indexCollection("/home/gennadiy/collection_html");
	// ii.indexCollection("/home/gennadiy/collection_test");
	std::cout << "King " <<  ii.executeQuery("King") << std::endl;
	



    return EXIT_SUCCESS;
}


