#include <gtest/gtest.h>

#include <helpers/GetFromIndexMacro.h>
#include <helpers/StringMacro.h>


class GET_FROM_INDEXShould : public ::testing::Test
{};

#define GET_FROM_INDEXShould__SAMPLE_LIST                                 \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16

TEST_F(GET_FROM_INDEXShould, getTheFirstElementFromAList) {
    // Given a macro list and an index to choose an element from it
#define GET_FROM_INDEXShould__getTheFirstElementFromAList__INDEX 0

    // When we get the element from the list
    const std::string result = STRING(GET_FROM_INDEX(GET_FROM_INDEXShould__getTheFirstElementFromAList__INDEX, GET_FROM_INDEXShould__SAMPLE_LIST));

    // Then the obtained element is the expected one
    EXPECT_EQ(result, "_1");
}

TEST_F(GET_FROM_INDEXShould, getTheSecondElementFromAList) {
    // Given a macro list and an index to choose an element from it
#define GET_FROM_INDEXShould__getTheSecondElementFromAList__INDEX 1

    // When we get the element from the list
    const std::string result = STRING(GET_FROM_INDEX(GET_FROM_INDEXShould__getTheSecondElementFromAList__INDEX, GET_FROM_INDEXShould__SAMPLE_LIST));

    // Then the obtained element is the expected one
    EXPECT_EQ(result, "_2");
}

TEST_F(GET_FROM_INDEXShould, getTheThirdElementFromAList) {
    // Given a macro list and an index to choose an element from it
#define GET_FROM_INDEXShould__getTheThirdElementFromAList__INDEX 2

    // When we get the element from the list
    const std::string result = STRING(GET_FROM_INDEX(GET_FROM_INDEXShould__getTheThirdElementFromAList__INDEX, GET_FROM_INDEXShould__SAMPLE_LIST));

    // Then the obtained element is the expected one
    EXPECT_EQ(result, "_3");
}

TEST_F(GET_FROM_INDEXShould, getThe16thElementFromAList) {
    // Given a macro list and an index to choose an element from it
#define GET_FROM_INDEXShould__getThe16thElementFromAList__INDEX 15

    // When we get the element from the list
    const std::string result = STRING(GET_FROM_INDEX(GET_FROM_INDEXShould__getThe16thElementFromAList__INDEX, GET_FROM_INDEXShould__SAMPLE_LIST));

    // Then the obtained element is the expected one
    EXPECT_EQ(result, "_16");
}
