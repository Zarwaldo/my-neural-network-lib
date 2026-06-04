#include <gtest/gtest.h>

#include <tensor/TensorMapKeyEnum.h>

// Define a sample KeyEnum
#define TENSOR_MAP_KEY_ENUM__VALUES_OF_KeyEnum FIRST_TENSOR, SECOND_TENSOR
DECLARE_TENSOR_MAP_KEY_ENUM(KeyEnum, )
IMPLEMENT_TENSOR_MAP_KEY_ENUM(KeyEnum)

class KeyEnumShould : public ::testing::Test
{};

TEST_F(KeyEnumShould, castFromEnumToSizeT)
{
    // Given a KeyEnum defined from the enum
    const KeyEnum key = FIRST_TENSOR;

    // When casting it to size_t
    const size_t result = static_cast<size_t>(key);

    // Then the result is the index equivalent to the KeyEnum's value
    EXPECT_EQ(result, 0);
}

TEST_F(KeyEnumShould, castFromSizeTToEnum)
{
    // Given a KeyEnum defined from a size_t
    const KeyEnum key = 1;

    // When casting it to the enum
    const KeyEnum::Values result = static_cast<KeyEnum::Values>(key);

    // Then the result is the index equivalent to the KeyEnum's value
    EXPECT_EQ(result, SECOND_TENSOR);
}

TEST_F(KeyEnumShould, compareIdenticalKeyEnums)
{
    // Given two identical KeyEnums
    const KeyEnum key1 = FIRST_TENSOR;
    const KeyEnum key2 = FIRST_TENSOR;

    // When comparing both KeyEnums
    const bool areEqual = (key1 == key2);
    const bool areDifferent = (key1 != key2);

    // Then the KeyEnums are computed as equal0
    EXPECT_TRUE(areEqual);
    EXPECT_FALSE(areDifferent);
}

TEST_F(KeyEnumShould, compareDifferentKeyEnums)
{
    // Given two different KeyEnums
    const KeyEnum key1 = FIRST_TENSOR;
    const KeyEnum key2 = SECOND_TENSOR;

    // When comparing both KeyEnums
    const bool areEqual = (key1 == key2);
    const bool areDifferent = (key1 != key2);

    // Then the KeyEnums are computed as equal0
    EXPECT_FALSE(areEqual);
    EXPECT_TRUE(areDifferent);
}

TEST_F(KeyEnumShould, provideItsNumberOfValues)
{
    // Given a KeyEnum class

    // When getting its number of values
    const size_t result = KeyEnum::NbValues;

    // Then the result is indeed the number of values in the base enum
    EXPECT_EQ(result, 2);
}

TEST_F(KeyEnumShould, iterateOnPossibleValues)
{
    // Given a KeyEnum class and its iterator provider
    const KeyEnum::IteratorProvider& iteratorProvider = KeyEnum::values();

    // When starting to iterate on values
    KeyEnum::Iterator it = iteratorProvider.begin();
    // Then the iterator gives the first value of the KeyEnum
    EXPECT_EQ(*it, KeyEnum{FIRST_TENSOR});

    // When continuing to iterate on values
    ++it;
    // Then the iterator gives the second value of the KeyEnum
    EXPECT_EQ(*it, KeyEnum{SECOND_TENSOR});

    // When ending to iterate on values
    ++it;
    // Then the iterator is equal to the end iterator
    EXPECT_TRUE(it == iteratorProvider.end());
}

TEST_F(KeyEnumShould, provideATupleContainingItsValues)
{
    // Given a KeyEnum class

    // When getting the tuple of possible values
    const RawTuple<KeyEnum, KeyEnum> result = KeyEnum::getValuesTuple();

    // Then the result indeed contains the possible values of the KeyEnum class
    EXPECT_EQ(result.get<0>(), KeyEnum{FIRST_TENSOR});
    EXPECT_EQ(result.get<1>(), KeyEnum{SECOND_TENSOR});
}
