#include <gtest/gtest.h>

#include <tensor/AbstractTensorIndex.h>
#include <tensor/TensorIndex.h>

class AbstractTensorIndexShould : public ::testing::Test
{};


TEST_F(AbstractTensorIndexShould, concatenateAValidIndexWithAnother)
{
    // Given two valid indices
    const AbstractTensorIndex* index1 = new TensorIndex<2>{1, 2};
    const AbstractTensorIndex* index2 = new TensorIndex<3>{3, 4, 5};

    // When we concatenate the first index with the second
    const AbstractTensorIndex* result = (*index1) * (*index2);

    // Then the result should be the concatenation of both indices
    ASSERT_EQ(result->dim(), 5);
    ASSERT_TRUE(result->isValid());
    EXPECT_EQ(static_cast<const TensorIndex<5>&>(*result), TensorIndex<5>({1, 2, 3, 4, 5}));

    // Free the allocated indices
    delete index1;
    delete index2;
    delete result;
}

TEST_F(AbstractTensorIndexShould, concatenateAValidIndexWithAnInvalidIndex)
{
    // Given two valid indices
    const AbstractTensorIndex* index1 = new TensorIndex<2>{1, 2};
    const AbstractTensorIndex* index2 = new TensorIndex<3>;

    // When we concatenate the first index with the second
    const AbstractTensorIndex* result = (*index1) * (*index2);

    // Then the result should be the concatenation of both indices
    ASSERT_EQ(result->dim(), 5);
    EXPECT_FALSE(result->isValid());

    // Free the allocated indices
    delete index1;
    delete index2;
    delete result;
}

TEST_F(AbstractTensorIndexShould, createARangeOfItsValues)
{
    // Given a valid index
    const AbstractTensorIndex* index = new TensorIndex<5>{5, 1, 3, 4, 2};

    // When taking a range of its values
    const AbstractTensorIndex* result = index->range(1, 3);

    // Then the result should contain the values from this range
    ASSERT_EQ(result->dim(), 3);
    ASSERT_TRUE(result->isValid());
    EXPECT_EQ((*result)[0], (*index)[1]);
    EXPECT_EQ((*result)[1], (*index)[2]);
    EXPECT_EQ((*result)[2], (*index)[3]);

    // Free the allocated indices
    delete index;
    delete result;
}

TEST_F(AbstractTensorIndexShould, createARangeOfItsValuesInTheOverflowingCase)
{
    // Given a valid index
    const AbstractTensorIndex* index = new TensorIndex<5>{5, 1, 3, 4, 2};

    // When taking a range of its values
    const AbstractTensorIndex* result = index->range(1, -1);

    // Then the result should contain the values from this range
    ASSERT_EQ(result->dim(), 4);
    ASSERT_TRUE(result->isValid());
    EXPECT_EQ((*result)[0], (*index)[1]);
    EXPECT_EQ((*result)[1], (*index)[2]);
    EXPECT_EQ((*result)[2], (*index)[3]);
    EXPECT_EQ((*result)[3], (*index)[4]);

    // Free the allocated indices
    delete index;
    delete result;
}

TEST_F(AbstractTensorIndexShould, createAnInvalidRangeIfInvalid)
{
    // Given an invalid index
    const AbstractTensorIndex* index = new TensorIndex<5>;

    // When taking a range of its values
    const AbstractTensorIndex* result = index->range(1, -1);

    // Then the result should be invalid
    EXPECT_EQ(result->dim(), 4);
    EXPECT_FALSE(result->isValid());

    // Free the allocated indices
    delete index;
    delete result;
}
