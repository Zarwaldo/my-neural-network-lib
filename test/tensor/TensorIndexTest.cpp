#include <gtest/gtest.h>

#include <tensor/TensorIndex.h>

class TensorIndexShould : public ::testing::Test
{};

TEST_F(TensorIndexShould, tellItIsInValidWithDefaultConstructor)
{
    // Given an invalid index
    const TensorIndex<3> index;

    // When we call the isValid method
    const bool result = index.isValid();

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellItIsInvalidWithBooleanConstructor)
{
    // Given an invalid index
    const TensorIndex<3> index(false);

    // When we call the isValid method
    const bool result = index.isValid();

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, beValidAndEqualZeroByDefaultWithBooleanConstructor)
{
    // Given a valid index
    const TensorIndex<3> index(true);

    // When looking at its value
    // Then the said index should be valid and have its default coefficients equal to zero
    ASSERT_TRUE(index.isValid());
    EXPECT_EQ(index[0], 0);
    EXPECT_EQ(index[1], 0);
    EXPECT_EQ(index[2], 0);
}

TEST_F(TensorIndexShould, tellItIsValidWithArrayConstructor)
{
    // Given a valid index
    const size_t values[] = {0, 1, 2};
    const TensorIndex<3> index(values);

    // When we call the isValid method
    const bool result = index.isValid();

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellItIsValidWithInitializerListConstructorWithCorrectSize)
{
    // Given a valid index
    const TensorIndex<3> index({0, 1, 2});

    // When we call the isValid method
    const bool result = index.isValid();

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellItIsInvalidWithInitializerListConstructorWithIncorrectSize)
{
    // Given an incorrectly instantiated index
    const TensorIndex<3> index({0, 1, 2, 3});

    // When we call the isValid method
    const bool result = index.isValid();

    // Then the result should be true
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellItIsValidWithCopyConstructorCalledOnValidIndex)
{
    // Given a copy of a valid index
    const TensorIndex<3> baseIndex({0, 1, 2});
    const TensorIndex<3> index(baseIndex);

    // When we call the isValid method
    const bool result = index.isValid();

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellItIsInvalidWithCopyConstructorCalledOnInvalidIndex)
{
    // Given a copy of an invalid index
    const TensorIndex<3> baseIndex;
    const TensorIndex<3> index(baseIndex);

    // When we call the isValid method
    const bool result = index.isValid();

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellItIsValidWithSetValidCalledOnInvalidIndex)
{
    // Given an invalid index on which setValid is called
    TensorIndex<3> index;
    index.setValid(true);

    // When we call the isValid method
    const bool result = index.isValid();

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellItIsInvalidWithSetValidCalledOnValidIndex)
{
    // Given a valid index on which setValid is called
    TensorIndex<3> index({0, 1, 2});
    index.setValid(false);

    // When we call the isValid method
    const bool result = index.isValid();

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellAnInvalidIndexIsEqualToAnotherInvalidIndex)
{
    // Given two invalid indices
    const TensorIndex<3> index1;
    const TensorIndex<3> index2;

    // When we ask whether these indices are equal
    const bool result = index1 == index2;

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellAValidIndexIsNotEqualToAnInvalidIndex)
{
    // Given a valid index and an invalid index
    const TensorIndex<3> index1({0, 1, 2});
    const TensorIndex<3> index2;

    // When we ask whether these indices are equal
    const bool result = index1 == index2;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellValidIndicesWithDifferentCoordinatesAreNotEqual)
{
    // Given two valid indices with different coordinates
    const TensorIndex<3> index1({0, 1, 2});
    const TensorIndex<3> index2({0, 1, 3});

    // When we ask whether these indices are equal
    const bool result = index1 == index2;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellValidIndicesWithEqualCoordinatesAreEqual)
{
    // Given two valid indices with equal coordinates
    const TensorIndex<3> index1({0, 1, 2});
    const TensorIndex<3> index2({0, 1, 2});

    // When we ask whether these indices are equal
    const bool result = index1 == index2;

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellAnInvalidIndexIsDifferentFromAnotherInvalidIndex)
{
    // Given two invalid indices
    const TensorIndex<3> index1;
    const TensorIndex<3> index2;

    // When we ask whether these indices are different
    const bool result = index1 != index2;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellAValidIndexIsNotDifferentFromAnInvalidIndex)
{
    // Given a valid index and an invalid index
    const TensorIndex<3> index1({0, 1, 2});
    const TensorIndex<3> index2;

    // When we ask whether these indices are different
    const bool result = index1 != index2;

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellValidIndicesWithDifferentCoordinatesAreDifferent)
{
    // Given two valid indices with different coordinates
    const TensorIndex<3> index1({0, 1, 2});
    const TensorIndex<3> index2({0, 1, 3});

    // When we ask whether these indices are different
    const bool result = index1 != index2;

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellValidIndicesWithEqualCoordinatesAreNotDifferent)
{
    // Given two valid indices with equal coordinates
    const TensorIndex<3> index1({0, 1, 2});
    const TensorIndex<3> index2({0, 1, 2});

    // When we ask whether these indices are different
    const bool result = index1 != index2;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsGreaterOrEqualToAnotherIndex0)
{
    // Given two valid indices
    const TensorIndex<2> index1({0, 0});
    const TensorIndex<2> index2({0, 1});

    // When we ask whether index2 is greater or equal to index1
    const bool result = index2 >= index1;

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsGreaterOrEqualToAnotherIndex1)
{
    // Given two valid indices
    const TensorIndex<2> index1({0, 1});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index2 is greater or equal to index1
    const bool result = index2 >= index1;

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsGreaterOrEqualToAnotherIndex2)
{
    // Given two valid indices
    const TensorIndex<2> index1({1, 1});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index2 is greater or equal to index1
    const bool result = index2 >= index1;

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsGreaterOrEqualToAnotherIndex3)
{
    // Given two valid indices
    const TensorIndex<2> index1({0, 0});
    const TensorIndex<2> index2({0, 1});

    // When we ask whether index1 is greater or equal to index2
    const bool result = index1 >= index2;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsGreaterOrEqualToAnotherIndex4)
{
    // Given two valid indices
    const TensorIndex<2> index1({0, 1});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index1 is greater or equal to index2
    const bool result = index1 >= index2;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsGreaterOrEqualToAnotherIndex5)
{
    // Given two valid indices
    const TensorIndex<2> index1({1, 1});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index1 is greater or equal to index2
    const bool result = index1 >= index2;

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsLesserOrEqualToAnotherIndex0)
{
    // Given two valid indices
    const TensorIndex<2> index1({0, 0});
    const TensorIndex<2> index2({0, 1});

    // When we ask whether index2 is lesser or equal to index1
    const bool result = index2 <= index1;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsLesserOrEqualToAnotherIndex1)
{
    // Given two valid indices
    const TensorIndex<2> index1({0, 1});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index2 is lesser or equal to index1
    const bool result = index2 <= index1;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsLesserOrEqualToAnotherIndex2)
{
    // Given two valid indices
    const TensorIndex<2> index1({1, 1});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index2 is lesser or equal to index1
    const bool result = index2 <= index1;

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsLesserOrEqualToAnotherIndex3)
{
    // Given two valid indices
    const TensorIndex<2> index1({0, 0});
    const TensorIndex<2> index2({0, 1});

    // When we ask whether index1 is lesser or equal to index2
    const bool result = index1 <= index2;

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsLesserOrEqualToAnotherIndex4)
{
    // Given two valid indices
    const TensorIndex<2> index1({0, 1});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index1 is lesser or equal to index2
    const bool result = index1 <= index2;

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsLesserOrEqualToAnotherIndex5)
{
    // Given two valid indices
    const TensorIndex<2> index1({1, 1});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index1 is lesser or equal to index2
    const bool result = index1 <= index2;

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsGreaterThanAnotherIndex0)
{
    // Given two valid indices
    const TensorIndex<2> index1({0, 0});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index2 is greater than index1
    const bool result = index2 > index1;

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsGreaterThanAnotherIndex1)
{
    // Given two valid indices
    const TensorIndex<2> index1({0, 1});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index2 is greater than index1
    const bool result = index2 > index1;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsGreaterThanAnotherIndex2)
{
    // Given two valid indices
    const TensorIndex<2> index1({1, 1});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index2 is greater than index1
    const bool result = index2 > index1;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsGreaterThanAnotherIndex3)
{
    // Given two valid indices
    const TensorIndex<2> index1({0, 0});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index1 is greater than index2
    const bool result = index1 > index2;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsGreaterThanAnotherIndex4)
{
    // Given two valid indices
    const TensorIndex<2> index1({0, 1});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index1 is greater than index2
    const bool result = index1 > index2;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsGreaterThanAnotherIndex5)
{
    // Given two valid indices
    const TensorIndex<2> index1({1, 1});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index1 is greater than index2
    const bool result = index1 > index2;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsLesserThanAnotherIndex0)
{
    // Given two valid indices
    const TensorIndex<2> index1({0, 0});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index2 is lesser than index1
    const bool result = index2 < index1;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsLesserThanAnotherIndex1)
{
    // Given two valid indices
    const TensorIndex<2> index1({0, 1});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index2 is lesser than index1
    const bool result = index2 < index1;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsLesserThanAnotherIndex2)
{
    // Given two valid indices
    const TensorIndex<2> index1({1, 1});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index2 is lesser than index1
    const bool result = index2 < index1;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsLesserThanAnotherIndex3)
{
    // Given two valid indices
    const TensorIndex<2> index1({0, 0});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index1 is lesser than index2
    const bool result = index1 < index2;

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsLesserThanAnotherIndex4)
{
    // Given two valid indices
    const TensorIndex<2> index1({0, 1});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index1 is lesser than index2
    const bool result = index1 < index2;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, tellWhetherAnIndexIsLesserThanAnotherIndex5)
{
    // Given two valid indices
    const TensorIndex<2> index1({1, 1});
    const TensorIndex<2> index2({1, 1});

    // When we ask whether index1 is lesser than index2
    const bool result = index1 < index2;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorIndexShould, addUpAnIndexWithAnInvalidIndex)
{
    // Given a valid index and an invalid index
    const TensorIndex<3> index1({0, 1, 2});
    const TensorIndex<3> index2;

    // When we add up these two indices
    const TensorIndex<3> result = index1 + index2;

    // Then the result should be invalid
    EXPECT_FALSE(result.isValid());
}

TEST_F(TensorIndexShould, addUpTwoIndices0)
{
    // Given two valid indices
    const TensorIndex<3> index1({0, 1, 2});
    const TensorIndex<3> index2({3, 4, 5});

    // When we add up these two indices
    const TensorIndex<3> result = index1 + index2;

    // Then the result should be their sum
    EXPECT_EQ(result, TensorIndex<3>({3, 5, 7}));
}

TEST_F(TensorIndexShould, addUpTwoIndices1)
{
    // Given two valid indices
    const TensorIndex<3> index1({3, 4, 5});
    const TensorIndex<3> index2({0, 1, 2});

    // When we add up these two indices
    const TensorIndex<3> result = index1 + index2;

    // Then the result should be their sum
    EXPECT_EQ(result, TensorIndex<3>({3, 5, 7}));
}

TEST_F(TensorIndexShould, incrementAnIndexWithAnInvalidIndex)
{
    // Given a valid index and an invalid index
    TensorIndex<3> index1({0, 1, 2});
    const TensorIndex<3> index2;

    // When we increment the first index with the second
    index1 += index2;

    // Then the first index should become invalid
    EXPECT_FALSE(index1.isValid());
}

TEST_F(TensorIndexShould, incrementAValidIndexWithAnother0)
{
    // Given two valid indices
    TensorIndex<3> index1({0, 1, 2});
    const TensorIndex<3> index2({3, 4, 5});

    // When we increment the first index with the second
    index1 += index2;

    // Then the first index should become their sum
    EXPECT_EQ(index1, TensorIndex<3>({3, 5, 7}));
}

TEST_F(TensorIndexShould, incrementAValidIndexWithAnother1)
{
    // Given two valid indices
    TensorIndex<3> index1({3, 4, 5});
    const TensorIndex<3> index2({0, 1, 2});

    // When we increment the first index with the second
    index1 += index2;

    // Then the first index should become their sum
    EXPECT_EQ(index1, TensorIndex<3>({3, 5, 7}));
}

TEST_F(TensorIndexShould, substractTwoIndicesWithOneInvalid)
{
    // Given a valid index and an invalid index
    const TensorIndex<3> index1({0, 1, 2});
    const TensorIndex<3> index2;

    // When we substract these two indices
    const TensorIndex<3> result = index1 - index2;

    // Then the result should be invalid
    EXPECT_FALSE(result.isValid());
}

TEST_F(TensorIndexShould, substractTwoIndicesWithNegativeResult)
{
    // Given two valid indices, the second one having a coordinate greater than the first's corresponding one
    const TensorIndex<3> index1({3, 4, 2});
    const TensorIndex<3> index2({0, 1, 5});

    // When we substract these two indices
    const TensorIndex<3> result = index1 - index2;

    // Then the result should be invalid
    EXPECT_FALSE(result.isValid());
}

TEST_F(TensorIndexShould, substractTwoIndices)
{
    // Given two valid indices
    const TensorIndex<3> index1({3, 4, 5});
    const TensorIndex<3> index2({0, 1, 2});

    // When we substract these two indices
    const TensorIndex<3> result = index1 - index2;

    // Then the result should be their difference
    EXPECT_EQ(result, TensorIndex<3>({3, 3, 3}));
}

TEST_F(TensorIndexShould, substractAnIndexWithAnInvalidIndex)
{
    // Given a valid index and an invalid index
    TensorIndex<3> index1({0, 1, 2});
    const TensorIndex<3> index2;

    // When we substract the first index with the second
    index1 -= index2;

    // Then the first index should become invalid
    EXPECT_FALSE(index1.isValid());
}

TEST_F(TensorIndexShould, substractAValidIndexWithAnotherWithNegativeResult)
{
    // Given two valid indices, the second one having a coordinate greater than the first's corresponding one
    TensorIndex<3> index1({3, 4, 2});
    const TensorIndex<3> index2({0, 1, 5});

    // When we substract the first index with the second
    index1 -= index2;

    // Then the first index should become invalid
    EXPECT_FALSE(index1.isValid());
}

TEST_F(TensorIndexShould, substractAValidIndexWithAnother)
{
    // Given two valid indices
    TensorIndex<3> index1({3, 4, 5});
    const TensorIndex<3> index2({0, 1, 2});

    // When we substract the first index with the second
    index1 -= index2;

    // Then the first index should become their difference
    EXPECT_EQ(index1, TensorIndex<3>({3, 3, 3}));
}

TEST_F(TensorIndexShould, concatenateAValidIndexWithAnother)
{
    // Given two valid indices
    const TensorIndex<2> index1({1, 2});
    const TensorIndex<3> index2({3, 4, 5});

    // When we concatenate the first index with the second
    const TensorIndex<5> result = index1 * index2;

    // Then the result should be the concatenation of both indices
    EXPECT_EQ(result, TensorIndex<5>({1, 2, 3, 4, 5}));
}

TEST_F(TensorIndexShould, concatenateAValidIndexWithAnInvalidIndex)
{
    // Given a valid index and an invalid one
    const TensorIndex<2> index1({1, 2});
    const TensorIndex<3> index2;

    // When we concatenate the first index with the second
    const TensorIndex<5> result = index1 * index2;

    // Then the result should be an invalid index
    EXPECT_FALSE(result.isValid());
}

TEST_F(TensorIndexShould, copyAValidIndexIntoAnInvalidIndex)
{
    // Given an invalid index and a valid index
    TensorIndex<3> index1;
    const TensorIndex<3> index2({0, 1, 2});

    // When we copy the second into the first
    index1 = index2;

    // Then the first should become the same as the second
    EXPECT_TRUE(index1 == index2);
}

TEST_F(TensorIndexShould, copyAnInvalidIndexIntoAValidIndex)
{
    // Given an invalid index and a valid index
    TensorIndex<3> index1({0, 1, 2});
    const TensorIndex<3> index2;

    // When we copy the second into the first
    index1 = index2;

    // Then the first should become the same as the second
    EXPECT_TRUE(index1 == index2);
}

TEST_F(TensorIndexShould, copyAValidIndexIntoAValidIndex)
{
    // Given an invalid index and a valid index
    TensorIndex<3> index1({0, 1, 2});
    const TensorIndex<3> index2({3, 4, 5});

    // When we copy the second into the first
    index1 = index2;

    // Then the first should become the same as the second
    EXPECT_TRUE(index1 == index2);
}

TEST_F(TensorIndexShould, computeTheDotProductOfAnInvalidIndexWithAnother)
{
    // Given an invalid index and a valid index
    const TensorIndex<3> index1;
    const TensorIndex<3> index2({0, 1, 2});

    // When we compute their dot product
    const size_t result = index1.dot(index2);

    // Then the result should be the size_t equivalent of -1
    EXPECT_EQ(result, static_cast<size_t>(-1));
}

TEST_F(TensorIndexShould, computeTheDotProductOfAZeroIndexWithAnother)
{
    // Given a zero index and a valid index
    const TensorIndex<3> index1({0, 0, 0});
    const TensorIndex<3> index2({1, 2, 3});

    // When we compute their dot product
    const size_t result = index1.dot(index2);

    // Then the result should be 0
    EXPECT_EQ(result, 0);
}

TEST_F(TensorIndexShould, computeTheDotProductOfTwoIndices0)
{
    // Given two valid indices
    const TensorIndex<3> index1({1, 2, 3});
    const TensorIndex<3> index2({4, 5, 6});

    // When we compute their dot product
    const size_t result = index1.dot(index2);

    // Then the result should be 32
    EXPECT_EQ(result, 32);
}

TEST_F(TensorIndexShould, computeTheDotProductOfTwoIndices1)
{
    // Given two valid indices
    const TensorIndex<3> index1({5, 2, 3});
    const TensorIndex<3> index2({4, 6, 1});

    // When we compute their dot product
    const size_t result = index1.dot(index2);

    // Then the result should be 35
    EXPECT_EQ(result, 35);
}

TEST_F(TensorIndexShould, computeTheNumberOfIndicesInferiorToAnInvalidIndex)
{
    // Given an invalid index
    const TensorIndex<3> index;

    // When we compute the number of indices inferior to it
    const size_t result = index.nbInferiorIndices();

    // Then the result should be the size_t equivalent of -1
    EXPECT_EQ(result, static_cast<size_t>(-1));
}

TEST_F(TensorIndexShould, computeTheNumberOfIndicesInferiorToAZeroIndex)
{
    // Given a zero index
    const TensorIndex<3> index({0, 0, 0});

    // When we compute the number of indices inferior to it
    const size_t result = index.nbInferiorIndices();

    // Then the result should be 0
    EXPECT_EQ(result, 0);
}

TEST_F(TensorIndexShould, computeTheNumberOfIndicesInferiorToAValidIndex0)
{
    // Given a valid index
    const TensorIndex<3> index({2, 3, 4});

    // When we compute the number of indices inferior to it
    const size_t result = index.nbInferiorIndices();

    // Then the result should be 24
    EXPECT_EQ(result, 24);
}

TEST_F(TensorIndexShould, computeTheNumberOfIndicesInferiorToAValidIndex1)
{
    // Given a valid index
    const TensorIndex<3> index({5, 1, 3});

    // When we compute the number of indices inferior to it
    const size_t result = index.nbInferiorIndices();

    // Then the result should be 15
    EXPECT_EQ(result, 15);
}

TEST_F(TensorIndexShould, giveAccessToItsFirstCoordinate)
{
    // Given a valid index
    const TensorIndex<3> index({5, 1, 3});

    // When we get its first coordinate
    const size_t& result = index[0];

    // Then the result should be its first coordinate
    EXPECT_EQ(result, 5);
}

TEST_F(TensorIndexShould, giveAccessToItsSecondCoordinate)
{
    // Given a valid index
    const TensorIndex<3> index({5, 1, 3});

    // When we get its second coordinate
    const size_t& result = index[1];

    // Then the result should be its second coordinate
    EXPECT_EQ(result, 1);
}

TEST_F(TensorIndexShould, giveAccessToItsThirdCoordinate)
{
    // Given a valid index
    const TensorIndex<3> index({5, 1, 3});

    // When we get its third coordinate
    const size_t& result = index[2];

    // Then the result should be its third coordinate
    EXPECT_EQ(result, 3);
}

TEST_F(TensorIndexShould, createARangeOfItsValues)
{
    // Given a valid index
    const TensorIndex<5> index({5, 1, 3, 4, 2});

    // When taking a range of its values
    const TensorIndex<3> result = index.range<1, 3>();

    // Then the result should contain the values from this range
    ASSERT_TRUE(result.isValid());
    EXPECT_EQ(result[0], index[1]);
    EXPECT_EQ(result[1], index[2]);
    EXPECT_EQ(result[2], index[3]);
}

TEST_F(TensorIndexShould, createARangeOfItsValuesInTheOverflowingCase)
{
    // Given a valid index
    const TensorIndex<5> index({5, 1, 3, 4, 2});

    // When taking a range of its values
    const TensorIndex<4> result = index.range<1, -1>();

    // Then the result should contain the values from this range
    ASSERT_TRUE(result.isValid());
    EXPECT_EQ(result[0], index[1]);
    EXPECT_EQ(result[1], index[2]);
    EXPECT_EQ(result[2], index[3]);
    EXPECT_EQ(result[3], index[4]);
}

TEST_F(TensorIndexShould, createAnInvalidRangeIfInvalid)
{
    // Given an invalid index
    const TensorIndex<5> index;

    // When taking a range of its values
    const TensorIndex<4> result = index.range<1, -1>();

    // Then the result should be invalid
    EXPECT_FALSE(result.isValid());
}

TEST_F(TensorIndexShould, iterateOnItsCoordinates)
{
    // Given a valid index
    const TensorIndex<3> index({5, 1, 3});

    // When we iterate on the index and get the iterated values
    size_t nbReadValues = 0;
    size_t iteratedValues[3];
    for (const size_t* it = index.begin(); it != index.end(); ++it)
    {
        iteratedValues[nbReadValues] = *it;
        nbReadValues++;
    }

    // Then these values should be its coordinates
    EXPECT_EQ(nbReadValues, 3);
    EXPECT_EQ(iteratedValues[0], 5);
    EXPECT_EQ(iteratedValues[1], 1);
    EXPECT_EQ(iteratedValues[2], 3);
}
