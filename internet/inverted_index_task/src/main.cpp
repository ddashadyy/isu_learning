#include <iostream>
#include "inverted_index.hpp"
#include "web_crawler.hpp"
#include <vector>


template< typename T >
std::ostream& operator << (std::ostream& os, const std::vector<T>& con)
{
	os.put('['); os.put(' '); 
    for (const auto& element : con) os << element << " ";
	os.put(']');

    return os;
}


int main() 
{
	InvertedIndex ii;
	ii.indexCollection("/home/gennadiy/isu_learning/collection_html");

	std::cout << "King " << ii.executeQuery("King") << std::endl;
	std::cout << "Romeo " << ii.executeQuery("Romeo") << std::endl;
	std::cout << "Juliet " << ii.executeQuery("Juliet") << std::endl;
	std::cout << "Caesar " << ii.executeQuery("Caesar") << std::endl;
	std::cout << "SpiderMan " << ii.executeQuery("SpiderMan") << std::endl;
	std::cout << "Brutus Caesar Calpurnia " << ii.executeQuery("Brutus Caesar Calpurnia") << std::endl;

    return EXIT_SUCCESS;
}


