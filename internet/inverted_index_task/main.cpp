#include <iostream>
#include <memory>
#include <chrono>
#include "inverted_index.hpp"
#include "html_parser.hpp"

std::ostream& operator << (std::ostream& os, const std::list<int> lst)
{
    os.put('[');
    os.put(' ');
    for (auto it = lst.begin(); it != lst.end(); ++it)
        os << *it << " ";
    os.put(']');

    return os;
}


void show_duration_of_indexing_collection(std::unique_ptr<InvertedIndex>& ptr)
{
    const auto start{std::chrono::steady_clock::now()};
    ptr->indexCollection("collection");
    const auto end{std::chrono::steady_clock::now()};

    const std::chrono::duration<double> elapsed_seconds{end - start};
    std::cout << elapsed_seconds.count() << std::endl;
}


#define INVERTED_INDEX
// #define INVERTED_INDEX_TESTS 

#define BOOST_PARSER

int main()
{
#ifdef BOOST_PARSER
    // // Sample HTML string
    // std::string html = "<html><head><title>Example HTML</title></head><body><h1>Hello, world!</h1></body></html>";

    // // Parse the HTML string
    // TagInfo tag_info;
    // html_tag parser;
    // std::string::const_iterator first = html.begin(), last = html.end();
    // bool success = x3::phrase_parse(first, last, parser, x3::space, tag_info);

    // // Check if parsing was successful
    // if (success && first == last) {
    //     // Print the tag information
    //     std::cout << "Tag name: " << tag_info.tag_name << std::endl;
    //     std::cout << "Attributes: ";
    //     for (const auto& attribute : tag_info.attributes) {
    //         std::cout << attribute << " ";
    //     }
    //     std::cout << std::endl;
    //     std::cout << "Content: " << tag_info.content << std::endl;
    // } else {
    //     std::cout << "Parsing failed." << std::endl;
    // }


#endif



#ifdef INVERTED_INDEX
    try
    {
        auto deserializedInvertedIndexPtr = std::make_unique<InvertedIndex>(InvertedIndex::readFromDisk("serialized"));

        deserializedInvertedIndexPtr->indexCollection("collection");
        deserializedInvertedIndexPtr->indexCollection("collection");
        deserializedInvertedIndexPtr->serialize("serialized");

        std::cout << "Brutus " << deserializedInvertedIndexPtr->executeQuery("Brutus") << std::endl;
        std::cout << "Caesar " << deserializedInvertedIndexPtr->executeQuery("Caesar") << std::endl;
        std::cout << "Calpurnia " << deserializedInvertedIndexPtr->executeQuery("Calpurnia") << std::endl;
        std::cout << "Brutus AND Brutus " << deserializedInvertedIndexPtr->executeQuery("Brutus AND Brutus") << std::endl;
        std::cout << "Brutus AND Caesar " << deserializedInvertedIndexPtr->executeQuery("Brutus AND Caesar") << std::endl;
        std::cout << "Brutus AND Caesar AND Calpurnia " << deserializedInvertedIndexPtr->executeQuery("Brutus AND Caesar AND Calpurnia") << std::endl;
        std::cout << "Brutus OR Brutus " << deserializedInvertedIndexPtr->executeQuery("Brutus OR Brutus") << std::endl;
        std::cout << "Brutus OR Caesar " << deserializedInvertedIndexPtr->executeQuery("Brutus OR Caesar") << std::endl;
        std::cout << "Brutus OR Caesar OR Calpurnia " << deserializedInvertedIndexPtr->executeQuery("Brutus OR Caesar OR Calpurnia") << std::endl;
        std::cout << "SpiderMan " << deserializedInvertedIndexPtr->executeQuery("SpiderMan") << std::endl;
        std::cout << "Brutus AND SpiderMan " << deserializedInvertedIndexPtr->executeQuery("Brutus AND SpiderMan") << std::endl;
        std::cout << "Caesar OR SpiderMan " << deserializedInvertedIndexPtr->executeQuery("Caesar OR SpiderMan") << std::endl;
    }
    catch(const std::ios_base::failure& e)
    {
        std::cerr << e.what() << '\n';
    }

#endif // INVERTED_INDEX

#ifdef INVERTED_INDEX_TESTS
    auto invertedIndexTestsPtr = std::make_unique<InvertedIndex>(InvertedIndex::readFromDisk("serialized"));
    show_duration_of_indexing_collection(invertedIndexTestsPtr);
#endif // INVERTED_INDEX_TESTS

    return EXIT_SUCCESS;
}