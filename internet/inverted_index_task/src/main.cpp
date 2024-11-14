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


// Функция для удаления неалфавитных символов
// std::string cleanWord(const std::string& word) {
//     std::string cleaned;
//     for (char c : word) {
//         if (std::isalpha(c)) { // Проверяем, является ли символ буквой
//             cleaned += std::tolower(c); // Приводим к нижнему регистру
//         }
//     }
//     return cleaned;
// }

int main() 
{
    // // WebCrawler wc("https://github.com/", 2);
    // // wc.crawl();

    // try
	// {
	// 	auto deserializedInvertedIndexPtr = std::make_unique<InvertedIndex>(InvertedIndex::readFromDisk("/home/gennadiy/third_course/internet/inverted_index_task/serialized"));

	// 	deserializedInvertedIndexPtr->indexCollection("/home/gennadiy/collection");
	// 	deserializedInvertedIndexPtr->serialize("/home/gennadiy/third_course/internet/inverted_index_task/serialized");
		
	// 	// deserializedInvertedIndexPtr->indexDocument("/home/gennadiy/collection/The_Life_and_Death_of_Julies_Caesar.txt");

	// 	std::cout << "Calpurnia's " << deserializedInvertedIndexPtr->executeQuery("Calpurnia's") << std::endl;
	// 	std::cout << "Brutus's " << deserializedInvertedIndexPtr->executeQuery("Brutus's") << std::endl;
	// 	std::cout << "Caesar's " << deserializedInvertedIndexPtr->executeQuery("Caesar's") << std::endl;
	// 	std::cout << "Romeo's " << deserializedInvertedIndexPtr->executeQuery("Romeo's") << std::endl;
	// 	std::cout << "Hamlet's " << deserializedInvertedIndexPtr->executeQuery("Hamlet's") << std::endl;

    //     std::cout << "Brutus OR Calpurnia " << deserializedInvertedIndexPtr->executeQuery("Brutus OR Calpurnia") << std::endl;
    //     std::cout << "Caesar OR Romeo OR Hamlet " << deserializedInvertedIndexPtr->executeQuery("Caesar OR Romeo OR Hamlet") << std::endl;
    //     std::cout << "( Brutus OR Calpurnia ) AND ( Caesar OR Romeo OR Hamlet ) " << deserializedInvertedIndexPtr->executeQuery("( Brutus OR Calpurnia ) AND ( Caesar OR Romeo OR Hamlet )") << std::endl;

	// }
	// catch (const std::ios_base::failure& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// }

	//  /home/gennadiy/stop_words.txt

	InvertedIndex ii("/home/gennadiy/stop_words.txt");
	// InvertedIndex ii;
	ii.indexCollection("/home/gennadiy/collection");

	// 4 простых, в том числе запрос, содержащий стоп-слово, и запросы с различными формами одного слова
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

    




    return EXIT_SUCCESS;
}


