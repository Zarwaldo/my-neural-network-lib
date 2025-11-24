#include <gtest/gtest.h>

#include <helpers/StringMacro.h>
#include <helpers/TernaryMacro.h>


class TERNARYShould : public ::testing::Test
{};

#define TERNARYShould__SAMPLE_TRUE_CASE TRUE
#define TERNARYShould__SAMPLE_FALSE_CASE FALSE

TEST_F(TERNARYShould, expandToTheFalseCaseInCaseTheConditionIsEmpty) {
    // Given an empty condition
#define TERNARYShould__expandToTheFalseCaseInCaseTheConditionIsEmpty__CONDITION

    // When computing the number of arguments and storing it in a variable
    const std::string result = STRING(TERNARY(TERNARYShould__expandToTheFalseCaseInCaseTheConditionIsEmpty__CONDITION, TERNARYShould__SAMPLE_TRUE_CASE, TERNARYShould__SAMPLE_FALSE_CASE));

    // Then the resulting string should correspond to the one passed as false case
    EXPECT_EQ(result, STRING(TERNARYShould__SAMPLE_FALSE_CASE));
}

TEST_F(TERNARYShould, expandToTheFalseCaseInCaseTheConditionIsEmptyWithForward) {
    // Given an empty condition using a FORWARD
#define TERNARYShould__expandToTheFalseCaseInCaseTheConditionIsEmptyWithForward__CONDITION FORWARD()

    // When computing the number of arguments and storing it in a variable
    const std::string result = STRING(TERNARY(TERNARYShould__expandToTheFalseCaseInCaseTheConditionIsEmptyWithForward__CONDITION, TERNARYShould__SAMPLE_TRUE_CASE, TERNARYShould__SAMPLE_FALSE_CASE));

    // Then the resulting string should correspond to the one passed as false case
    EXPECT_EQ(result, STRING(TERNARYShould__SAMPLE_FALSE_CASE));
}

TEST_F(TERNARYShould, expandToTheTrueCaseInCaseTheConditionIsNonEmpty) {
    // Given a non-empty condition
#define TERNARYShould__expandToTheTrueCaseInCaseTheConditionIsNonEmpty__CONDITION a

    // When computing the number of arguments and storing it in a variable
    const std::string result = STRING(TERNARY(TERNARYShould__expandToTheTrueCaseInCaseTheConditionIsNonEmpty__CONDITION, TERNARYShould__SAMPLE_TRUE_CASE, TERNARYShould__SAMPLE_FALSE_CASE));

    // Then the resulting string should correspond to the one passed as true case
    EXPECT_EQ(result, STRING(TERNARYShould__SAMPLE_TRUE_CASE));
}

TEST_F(TERNARYShould, expandToTheTrueCaseInCaseTheConditionIsNonEmptyWithSeveralParameters) {
    // Given a non-empty condition with several parameters
#define TERNARYShould__expandToTheTrueCaseInCaseTheConditionIsNonEmptyWithSeveralParameters__CONDITION FORWARD(a, b)

    // When computing the number of arguments and storing it in a variable
    const std::string result = STRING(TERNARY(TERNARYShould__expandToTheTrueCaseInCaseTheConditionIsNonEmptyWithSeveralParameters__CONDITION, TERNARYShould__SAMPLE_TRUE_CASE, TERNARYShould__SAMPLE_FALSE_CASE));

    // Then the resulting string should correspond to the one passed as true case
    EXPECT_EQ(result, STRING(TERNARYShould__SAMPLE_TRUE_CASE));
}

TEST_F(TERNARYShould, expandToTheTrueCaseInCaseTheConditionIsNonEmptyWithAComma) {
    // Given a non-empty condition with only a comma
#define TERNARYShould__expandToTheTrueCaseInCaseTheConditionIsNonEmptyWithAComma__CONDITION FORWARD(,)

    // When computing the number of arguments and storing it in a variable
    const std::string result = STRING(TERNARY(TERNARYShould__expandToTheTrueCaseInCaseTheConditionIsNonEmptyWithAComma__CONDITION, TERNARYShould__SAMPLE_TRUE_CASE, TERNARYShould__SAMPLE_FALSE_CASE));

    // Then the resulting string should correspond to the one passed as true case
    EXPECT_EQ(result, STRING(TERNARYShould__SAMPLE_TRUE_CASE));
}
