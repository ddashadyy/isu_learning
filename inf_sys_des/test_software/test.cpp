#include "test.hpp"

int Sequence::evaluateLocalMaximum(const std::vector<int>& sequence)
{
    if (sequence.size() < 3) 
        throw std::invalid_argument("Sequence must contain at least three elements.");
    
    std::vector<int> localMaximaIndices;

    for (size_t i = 1; i < sequence.size() - 1; ++i) 
        if (sequence[i] > sequence[i - 1] && sequence[i] > sequence[i + 1]) 
            localMaximaIndices.push_back(i);
        
    if (localMaximaIndices.size() < 2) 
        return 0;
    
    int minDistance = std::numeric_limits<int>::max();
    for (size_t i = 1; i < localMaximaIndices.size(); ++i) 
    {
        int distance = localMaximaIndices[i] - localMaximaIndices[i - 1];
        if (distance < minDistance) minDistance = distance;
    }

    return minDistance;
}