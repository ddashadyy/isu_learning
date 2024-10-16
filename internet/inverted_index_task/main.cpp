#include <iostream>
#include <memory>
#include "inverted_index.hpp"

std::ostream& operator << (std::ostream& os, const std::list<int> lst)
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
    auto invertedIndexPtr = std::make_unique<InvertedIndex>();

    invertedIndexPtr->indexCollection("collection");
    invertedIndexPtr->serialize();

    auto deserializedInvertedIndexPtr = std::make_unique<InvertedIndex>(std::move(invertedIndexPtr->deserialize("serialized.bin")));
    
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

    return 0;
}