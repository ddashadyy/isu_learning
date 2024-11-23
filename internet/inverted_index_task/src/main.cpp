#include <iostream>
#include <string>
#include <cstring>
#include <curl/curl.h>
#include <gumbo.h>
#include <memory>
#include "inverted_index.hpp"
#include "web_crawler.hpp"
#include <libstemmer.h>

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
	InvertedIndex ii("/home/gennadiy/stop_words.txt");
	// InvertedIndex ii;
	ii.indexCollection("/home/gennadiy/collection");

	// ii.indexCollection("/home/gennadiy/collection_html", "html");
	
	std::cout << "4 simple\n";
	std::cout << "was " << ii.executeQuery("was") << std::endl;
	std::cout << "Calpurnia's " << ii.executeQuery("Calpurnia's") << std::endl;
	std::cout << "Brutus " << ii.executeQuery("Brutus") << std::endl;
	std::cout << "Caesar " << ii.executeQuery("Caesar OR Romeo OR Hamlet") << std::endl;
	std::cout << std::endl;

	std::cout << "4 conjunctive\n";
	std::cout << "( Brutus AND Calpurnia ) AND ( Caesar AND Romeo AND Hamlet ) " << ii.executeQuery("( Brutus AND Calpurnia ) AND ( Caesar AND Romeo AND Hamlet ) ") << std::endl;
	std::cout << "Calpurnia's AND Brutus" << ii.executeQuery("Calpurnia's AND Brutus") << std::endl;
	std::cout << "Brutus AND who " << ii.executeQuery("Brutus AND who") << std::endl;
	std::cout << "Caesar AND when " << ii.executeQuery("Caesar and when") << std::endl;
	std::cout << std::endl;

	std::cout << "4 disjunctive\n";
	std::cout << "( Brutus OR Calpurnia ) OR ( Caesar OR Romeo OR Hamlet ) " << ii.executeQuery("( Brutus OR Calpurnia ) AND ( Caesar OR Romeo OR Hamlet ) ") << std::endl;
	std::cout << "Calpurnia's OR Brutus" << ii.executeQuery("Calpurnia's OR Brutus") << std::endl;
	std::cout << "Brutus OR then " << ii.executeQuery("Brutus OR then") << std::endl;
	std::cout << "Caesar OR them " << ii.executeQuery("Caesar OR them") << std::endl;
	std::cout << std::endl;

	std::list<int> l1 = {1,2,3,4};
	std::list<int> l2 = {};

	std::list<int> result;

	std::set_intersection(l1.begin(), l1.end(), l2.begin(), l2.end(), std::back_inserter(result));

	std::cout << result << std::endl;
    




    return EXIT_SUCCESS;
}


