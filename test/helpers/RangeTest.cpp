#include <gtest/gtest.h>

#include <helpers/RangeMacro.h>
#include <helpers/StringMacro.h>


class RANGEShould : public ::testing::Test
{};

TEST_F(RANGEShould, createARangeOfSize1) {
    // Given a desired range size of 1
#define RANGEShould__createARangeOfSize0__SIZE 1

    // When we create a range of the desired size
    const std::string result = STRING(RANGE(RANGEShould__createARangeOfSize0__SIZE));

    // Then we only get 0
    EXPECT_EQ(result, "0");
}

TEST_F(RANGEShould, createARangeOfSize2) {
    // Given a desired range size of 2
#define RANGEShould__createARangeOfSize2__SIZE 2

    // When we create a range of the desired size
    const std::string result = STRING(RANGE(RANGEShould__createARangeOfSize2__SIZE));

    // Then we get the 2 first integers
    EXPECT_EQ(result, "0, 1");
}

TEST_F(RANGEShould, createARangeOfSize3) {
    // Given a desired range size of 3
#define RANGEShould__createARangeOfSize3__SIZE 3

    // When we create a range of the desired size
    const std::string result = STRING(RANGE(RANGEShould__createARangeOfSize3__SIZE));

    // Then we get the 3 first integers
    EXPECT_EQ(result, "0, 1, 2");
}

TEST_F(RANGEShould, createARangeOfSize16) {
    // Given a desired range size of 16
#define RANGEShould__createARangeOfSize16__SIZE 16

    // When we create a range of the desired size
    const std::string result = STRING(RANGE(RANGEShould__createARangeOfSize16__SIZE));

    // Then we get the 16 first integers
    EXPECT_EQ(result,
        "0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15"
    );
}
