#include <gtest/gtest.h>

#include <helpers/ForEachMacro.h>
#include <helpers/StringMacro.h>


class FOR_EACHShould : public ::testing::Test
{};

#define FOR_EACHShould__SAMPLE_INSTRUCTION(element, vectorName, pushMethodName) vectorName.pushMethodName(element)

TEST_F(FOR_EACHShould, generateInstructionsInCaseThereIs1Argument) {
    // Given 1 argument and a vector to fill
#define FOR_EACHShould__generateInstructionsInCaseThereIs1Argument__ARGUMENTS "_1"
    std::vector<std::string> vector;

    // When computing the number of arguments and storing it in a variable
    (FOR_EACH(FOR_EACHShould__SAMPLE_INSTRUCTION, FORWARD(vector, push_back), FOR_EACHShould__generateInstructionsInCaseThereIs1Argument__ARGUMENTS));

    // Then the resulting vector should be filled with the iterated arguments
    const std::vector<std::string> expected{FOR_EACHShould__generateInstructionsInCaseThereIs1Argument__ARGUMENTS};
    EXPECT_EQ(vector, expected);
}

TEST_F(FOR_EACHShould, generateInstructionsInCaseThereIs2Arguments) {
    // Given 2 arguments and a vector to fill
#define FOR_EACHShould__generateInstructionsInCaseThereIs2Arguments__ARGUMENTS "_1", "_2"
    std::vector<std::string> vector;

    // When computing the number of arguments and storing it in a variable
    (FOR_EACH(FOR_EACHShould__SAMPLE_INSTRUCTION, FORWARD(vector, push_back), FOR_EACHShould__generateInstructionsInCaseThereIs2Arguments__ARGUMENTS));

    // Then the resulting vector should be filled with the iterated arguments
    const std::vector<std::string> expected{FOR_EACHShould__generateInstructionsInCaseThereIs2Arguments__ARGUMENTS};
    EXPECT_EQ(vector, expected);
}

TEST_F(FOR_EACHShould, generateInstructionsInCaseThereIs3Arguments) {
    // Given 3 arguments and a vector to fill
#define FOR_EACHShould__generateInstructionsInCaseThereIs3Arguments__ARGUMENTS "_1", "_2", "_3"
    std::vector<std::string> vector;

    // When computing the number of arguments and storing it in a variable
    (FOR_EACH(FOR_EACHShould__SAMPLE_INSTRUCTION, FORWARD(vector, push_back), FOR_EACHShould__generateInstructionsInCaseThereIs3Arguments__ARGUMENTS));

    // Then the resulting vector should be filled with the iterated arguments
    const std::vector<std::string> expected{FOR_EACHShould__generateInstructionsInCaseThereIs3Arguments__ARGUMENTS};
    EXPECT_EQ(vector, expected);
}

TEST_F(FOR_EACHShould, generateInstructionsInCaseThereIs16Arguments) {
    // Given 16 arguments and a vector to fill
#define FOR_EACHShould__generateInstructionsInCaseThereIs16Arguments__ARGUMENTS                           \
    "_1", "_2", "_3", "_4", "_5", "_6", "_7", "_8", "_9", "_10", "_11", "_12", "_13", "_14", "_15", "_16"
    std::vector<std::string> vector;

    // When computing the number of arguments and storing it in a variable
    (FOR_EACH(FOR_EACHShould__SAMPLE_INSTRUCTION, FORWARD(vector, push_back), FOR_EACHShould__generateInstructionsInCaseThereIs16Arguments__ARGUMENTS));

    // Then the resulting vector should be filled with the iterated arguments
    const std::vector<std::string> expected{FOR_EACHShould__generateInstructionsInCaseThereIs16Arguments__ARGUMENTS};
    EXPECT_EQ(vector, expected);
}


class FOR_EACH_SEPShould : public ::testing::Test
{};

#define FOR_EACH_SEPShould__SAMPLE_INSTRUCTION(element, ...) _##element
#define FOR_EACH_SEPShould__SAMPLE_SEPARATOR §

TEST_F(FOR_EACH_SEPShould, generateSeparatedStringsInCaseThereIs1Argument) {
    // Given 1 argument
#define FOR_EACH_SEPShould__generateSeparatedStringsInCaseThereIs1Argument__ARGUMENTS 1

    // When iterating an instruction on the arguments with a separator
    const std::string result = STRING(FOR_EACH_SEP(FOR_EACH_SEPShould__SAMPLE_INSTRUCTION, FORWARD(FOR_EACH_SEPShould__SAMPLE_SEPARATOR), FORWARD(), FOR_EACH_SEPShould__generateSeparatedStringsInCaseThereIs1Argument__ARGUMENTS));

    // Then the resulting string should be the arguments applied to the instruction, separated by the separator
    EXPECT_EQ(result, "_1");
}

TEST_F(FOR_EACH_SEPShould, generateSeparatedStringsInCaseThereIs2Arguments) {
    // Given 2 arguments
#define FOR_EACH_SEPShould__generateSeparatedStringsInCaseThereIs2Arguments__ARGUMENTS 1, 2

    // When iterating an instruction on the arguments with a separator
    const std::string result = STRING(FOR_EACH_SEP(FOR_EACH_SEPShould__SAMPLE_INSTRUCTION, FORWARD(FOR_EACH_SEPShould__SAMPLE_SEPARATOR), FORWARD(), FOR_EACH_SEPShould__generateSeparatedStringsInCaseThereIs2Arguments__ARGUMENTS));

    // Then the resulting string should be the arguments applied to the instruction, separated by the separator
    EXPECT_EQ(result, "_1 § _2");
}

TEST_F(FOR_EACH_SEPShould, generateSeparatedStringsInCaseThereIs3Arguments) {
    // Given 3 arguments
#define FOR_EACH_SEPShould__generateSeparatedStringsInCaseThereIs3Arguments__ARGUMENTS 1, 2, 3

    // When iterating an instruction on the arguments with a separator
    const std::string result = STRING(FOR_EACH_SEP(FOR_EACH_SEPShould__SAMPLE_INSTRUCTION, FORWARD(FOR_EACH_SEPShould__SAMPLE_SEPARATOR), FORWARD(), FOR_EACH_SEPShould__generateSeparatedStringsInCaseThereIs3Arguments__ARGUMENTS));

    // Then the resulting string should be the arguments applied to the instruction, separated by the separator
    EXPECT_EQ(result, "_1 § _2 § _3");
}

TEST_F(FOR_EACH_SEPShould, generateSeparatedStringsInCaseThereIs16Arguments) {
    // Given 16 arguments
#define FOR_EACH_SEPShould__generateSeparatedStringsInCaseThereIs16Arguments__ARGUMENTS \
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16

    // When iterating an instruction on the arguments with a separator
    const std::string result = STRING(FOR_EACH_SEP(FOR_EACH_SEPShould__SAMPLE_INSTRUCTION, FORWARD(FOR_EACH_SEPShould__SAMPLE_SEPARATOR), FORWARD(), FOR_EACH_SEPShould__generateSeparatedStringsInCaseThereIs16Arguments__ARGUMENTS));

    // Then the resulting string should be the arguments applied to the instruction, separated by the separator
    EXPECT_EQ(result, "_1 § _2 § _3 § _4 § _5 § _6 § _7 § _8 § _9 § _10 § _11 § _12 § _13 § _14 § _15 § _16");
}


class JOINShould : public ::testing::Test
{};

#define JOINShould__SAMPLE_SEPARATOR §

TEST_F(JOINShould, joinArgumentsInCaseThereIs1) {
    // Given 1 argument
#define JOINShould__joinArgumentsInCaseThereIs1__ARGUMENTS _1

    // When joining the arguments with a separator
    const std::string result = STRING(JOIN(JOINShould__SAMPLE_SEPARATOR, JOINShould__joinArgumentsInCaseThereIs1__ARGUMENTS));

    // Then the resulting string is the base argument
    EXPECT_EQ(result, "_1");
}

TEST_F(JOINShould, joinArgumentsInCaseThereIs2) {
    // Given 2 arguments
#define JOINShould__joinArgumentsInCaseThereIs2__ARGUMENTS _1, _2

    // When joining the arguments with a separator
    const std::string result = STRING(JOIN(JOINShould__SAMPLE_SEPARATOR, JOINShould__joinArgumentsInCaseThereIs2__ARGUMENTS));

    // Then the resulting string is the base arguments joined with the separator
    EXPECT_EQ(result, "_1 § _2");
}

TEST_F(JOINShould, joinArgumentsInCaseThereIs3) {
    // Given 3 arguments
#define JOINShould__joinArgumentsInCaseThereIs3__ARGUMENTS _1, _2, _3

    // When joining the arguments with a separator
    const std::string result = STRING(JOIN(JOINShould__SAMPLE_SEPARATOR, JOINShould__joinArgumentsInCaseThereIs3__ARGUMENTS));

    // Then the resulting string is the base arguments joined with the separator
    EXPECT_EQ(result, "_1 § _2 § _3");
}

TEST_F(JOINShould, joinArgumentsInCaseThereIs16) {
    // Given 16 arguments
#define JOINShould__joinArgumentsInCaseThereIs16__ARGUMENTS               \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16

    // When joining the arguments with a separator
    const std::string result = STRING(JOIN(JOINShould__SAMPLE_SEPARATOR, JOINShould__joinArgumentsInCaseThereIs16__ARGUMENTS));

    // Then the resulting string is the base arguments joined with the separator
    EXPECT_EQ(result, "_1 § _2 § _3 § _4 § _5 § _6 § _7 § _8 § _9 § _10 § _11 § _12 § _13 § _14 § _15 § _16");
}


class REPEATShould : public ::testing::Test
{};

#define REPEATShould__SAMPLE_STRING string

TEST_F(REPEATShould, repeatAString1Times) {
    // Given a number of repetitions to make
#define REPEATShould__repeatAString1Times__NB_REPETITIONS 1

    // When repeating a sample string this number of times
    const std::string result = STRING(REPEAT(REPEATShould__SAMPLE_STRING, REPEATShould__repeatAString1Times__NB_REPETITIONS));

    // Then the resulting string is the sample string repeated this number of times
    EXPECT_EQ(result, "string");
}

TEST_F(REPEATShould, repeatAString2Times) {
    // Given a number of repetitions to make
#define REPEATShould__repeatAString2Times__NB_REPETITIONS 2

    // When repeating a sample string this number of times
    const std::string result = STRING(REPEAT(REPEATShould__SAMPLE_STRING, REPEATShould__repeatAString2Times__NB_REPETITIONS));

    // Then the resulting string is the sample string repeated this number of times
    EXPECT_EQ(result, "string , string");
}

TEST_F(REPEATShould, repeatAString3Times) {
    // Given a number of repetitions to make
#define REPEATShould__repeatAString3Times__NB_REPETITIONS 3

    // When repeating a sample string this number of times
    const std::string result = STRING(REPEAT(REPEATShould__SAMPLE_STRING, REPEATShould__repeatAString3Times__NB_REPETITIONS));

    // Then the resulting string is the sample string repeated this number of times
    EXPECT_EQ(result, "string , string , string");
}

TEST_F(REPEATShould, repeatAString16Times) {
    // Given a number of repetitions to make
#define REPEATShould__repeatAString16Times__NB_REPETITIONS 16

    // When repeating a sample string this number of times
    const std::string result = STRING(REPEAT(REPEATShould__SAMPLE_STRING, REPEATShould__repeatAString16Times__NB_REPETITIONS));

    // Then the resulting string is the sample string repeated this number of times
    EXPECT_EQ(result, "string , string , string , string , string , string , string , string , string , string , string , string , string , string , string , string");
}
