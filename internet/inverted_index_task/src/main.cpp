#include <iostream>
#include "inverted_index.hpp"
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>

template< typename T >
std::ostream& operator << (std::ostream& os, const std::vector<T>& con)
{
	os.put('['); os.put(' '); 
    for (const auto& element : con) os << element << " ";
	os.put(']');

    return os;
}

using json = nlohmann::json;

int main() 
{
	InvertedIndex ii;

	auto start = std::chrono::high_resolution_clock::now();
	ii.indexCollection("/home/gennadiy/collection");
	auto end = std::chrono::high_resolution_clock::now();

	ii.serialize("serialized.json");

	std::chrono::duration<double> time = end - start;
	std::cout << time.count() << std::endl;

	ii.printResult("Juliet");


    return EXIT_SUCCESS;
}


