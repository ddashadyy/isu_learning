#pragma once

#include <gtest/gtest.h>
#include <tuple>
#include <vector>

struct ParametrizedSequence : public ::testing::TestWithParam<std::tuple<std::vector<int>, int>> {};


