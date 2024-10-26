#include <vector>
#include <stdexcept>
#include <limits>
#include <gtest/gtest.h>
#include "test.hpp"
#include "parametrized_test.hpp"

TEST(SequenceTest, TestWithTwoLocalMaxima) {
    EXPECT_EQ(Sequence::evaluateLocalMaximum({1, 2, 1, 3, 1, 4, 1}), 2);
}

TEST(SequenceTest, TestWithOneLocalMaximum) {
    EXPECT_EQ(Sequence::evaluateLocalMaximum({1, 2, 1, 0, 3}), 0);
}

TEST(SequenceTest, TestWithNoLocalMaximum) {
    EXPECT_EQ(Sequence::evaluateLocalMaximum({1, 1, 1, 1}), 0);
}

TEST(SequenceTest, TestWithMultipleLocalMaxima) {
    EXPECT_EQ(Sequence::evaluateLocalMaximum({1, 3, 2, 4, 3, 5, 4}), 2);
}

TEST(SequenceTest, TestWithInsufficientElements) {
    EXPECT_THROW(Sequence::evaluateLocalMaximum({1}), std::invalid_argument);
    EXPECT_THROW(Sequence::evaluateLocalMaximum({1, 2}), std::invalid_argument);
}

TEST(SequenceTest, TestWithSameValues) {
    EXPECT_EQ(Sequence::evaluateLocalMaximum({5, 5, 5, 5}), 0);
}

TEST(SequenceTest, TestWithRandomValues) {
    EXPECT_EQ(Sequence::evaluateLocalMaximum({10, 20, 10, 30, 20, 40}), 2);
}

TEST(SequenceTest, DISABLED_TestWithLargeInput) {
    std::vector<int> input(1000);
    for (int i = 0; i < 1000; ++i) 
        input[i] = (i % 100 == 0) ? i + 1 : i;
    
    EXPECT_EQ(Sequence::evaluateLocalMaximum(input), 100);
}

// Определяем параметризованный тест
TEST_P(ParametrizedSequence, CompareElements) {
    // Здесь могла бы быть какая нибудь логика но она нам не нужна :)
}

// Инициализируем параметризованный тест
INSTANTIATE_TEST_SUITE_P(TestSeq, ParametrizedSequence,
    ::testing::Values(
        std::make_tuple(std::vector<int>{1, 2, 3}, 2),
        std::make_tuple(std::vector<int>{1, 2}, 3),
        std::make_tuple(std::vector<int>{1, 2, 1, 1, 2, 1, 2, 1}, 2)
    )
);

int main(int argc, char** argv) 
{
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS(); 
}