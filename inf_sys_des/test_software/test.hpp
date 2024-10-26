#pragma once

#include <vector>
#include <stdexcept>
#include <limits>
#include <gtest/gtest.h>

struct Sequence 
{
public:
    static int evaluateLocalMaximum(const std::vector<int>& sequence);  
};


