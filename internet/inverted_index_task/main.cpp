#include <iostream>
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
    InvertedIndex ii;
    
    ii.indexCollection("collection");

    std::cout << "Brutus " << ii.executeQuery("Brutus") << std::endl;
    std::cout << "Caesar " << ii.executeQuery("Caesar") << std::endl;
    std::cout << "Calpurnia " << ii.executeQuery("Calpurnia") << std::endl;
    std::cout << "Brutus AND Brutus " << ii.executeQuery("Brutus AND Brutus") << std::endl;
    std::cout << "Brutus AND Caesar " << ii.executeQuery("Brutus AND Caesar") << std::endl;
    std::cout << "Brutus AND Caesar AND Calpurnia " << ii.executeQuery("Brutus AND Caesar AND Calpurnia") << std::endl;
    std::cout << "Brutus OR Brutus " << ii.executeQuery("Brutus OR Brutus") << std::endl;
    std::cout << "Brutus OR Caesar " << ii.executeQuery("Brutus OR Caesar") << std::endl;
    std::cout << "Brutus OR Caesar OR Calpurnia " << ii.executeQuery("Brutus OR Caesar OR Calpurnia") << std::endl;
    std::cout << "SpiderMan " << ii.executeQuery("SpiderMan") << std::endl;
    std::cout << "Brutus AND SpiderMan " << ii.executeQuery("Brutus AND SpiderMan") << std::endl;
    std::cout << "Caesar OR SpiderMan " << ii.executeQuery("Caesar OR SpiderMan") << std::endl;

    return 0;
}