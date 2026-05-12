#include <gtest/gtest.h>

#include <tensor/TensorMap.h>

class TensorMapShould : public ::testing::Test
{
protected:
    Tensor<float, 2>* firstTensor;
    Tensor<float, 3>* secondTensor;

    void SetUp() override
    {
        firstTensor = Tensor<float, 2>::create({2, 2});
        secondTensor = Tensor<float, 3>::create({3, 3, 3});
    }

    void TearDown() override
    {
        delete firstTensor;
        delete secondTensor;
    }
};

#define TENSOR_MAP_KEY_ENUM__VALUES_OF_KeyEnum FIRST_TENSOR, SECOND_TENSOR
DEFINE_TENSOR_MAP_KEY_ENUM(KeyEnum, )
IMPLEMENT_TENSOR_MAP_KEY_ENUM(KeyEnum)

TEST_F(TensorMapShould, getItsFirstTensorBeingBuiltFromInitializerList)
{
    // Given a tensor map, containing both tensors, built from an initializer list
    const TensorMap<float, KeyEnum> map({firstTensor, secondTensor});

    // When we get its first tensor
    const Tensor<float, 2>& result = dynamic_cast<const Tensor<float, 2>&>(map.get(FIRST_TENSOR));

    // Then the result is our first tensor
    EXPECT_EQ(&result, firstTensor);
}

TEST_F(TensorMapShould, getItsSecondTensorBeingBuiltFromInitializerList)
{
    // Given a tensor map, containing both tensors, built from an initializer list
    const TensorMap<float, KeyEnum> map({firstTensor, secondTensor});

    // When we get its second tensor
    const Tensor<float, 3>& result = dynamic_cast<const Tensor<float, 3>&>(map.get(SECOND_TENSOR));

    // Then the result is our second tensor
    EXPECT_EQ(&result, secondTensor);
}

TEST_F(TensorMapShould, getItsFirstTensorBeingBuiltFromArray)
{
    // Given a tensor map, containing both tensors, built from an array of pointers
    AbstractTensor<float>* const tensors[] = {firstTensor, secondTensor};
    const TensorMap<float, KeyEnum> map(tensors);

    // When we get its first tensor
    const Tensor<float, 2>& result = dynamic_cast<const Tensor<float, 2>&>(map.get(FIRST_TENSOR));

    // Then the result is our first tensor
    EXPECT_EQ(&result, firstTensor);
}

TEST_F(TensorMapShould, getItsSecondTensorBeingBuiltFromArray)
{
    // Given a tensor map, containing both tensors, built from an array of pointers
    AbstractTensor<float>* const tensors[] = {firstTensor, secondTensor};
    const TensorMap<float, KeyEnum> map(tensors);

    // When we get its second tensor
    const Tensor<float, 3>& result = dynamic_cast<const Tensor<float, 3>&>(map.get(SECOND_TENSOR));

    // Then the result is our second tensor
    EXPECT_EQ(&result, secondTensor);
}

TEST_F(TensorMapShould, getItsFirstTensorFromSizeT)
{
    // Given a tensor map, containing both tensors
    const TensorMap<float, KeyEnum> map({firstTensor, secondTensor});

    // When we get its tensor of index 0
    const AbstractTensor<float>& result = map.get(0);

    // Then the result is our first tensor
    EXPECT_EQ(&result, &map.get(FIRST_TENSOR));
}

TEST_F(TensorMapShould, getItsSecondTensorFromSizeT)
{
    // Given a tensor map, containing both tensors
    const TensorMap<float, KeyEnum> map({firstTensor, secondTensor});

    // When we get its tensor of index 1
    const AbstractTensor<float>& result = map.get(1);

    // Then the result is our second tensor
    EXPECT_EQ(&result, &map.get(SECOND_TENSOR));
}

TEST_F(TensorMapShould, iterateOnItsTensors)
{
    // Given a tensor map, containing both tensors
    const TensorMap<float, KeyEnum> map({firstTensor, secondTensor});

    // When we start iterating on the map
    AbstractTensorMap<float>::IteratorConst it = map.begin();

    // Then the iterator returns the first tensor
    EXPECT_NE(it, map.end());
    EXPECT_EQ(&(*it), &map.get(FIRST_TENSOR));

    // When we continue iterating on the map
    ++it;

    // Then the iterator returns the second tensor
    EXPECT_NE(it, map.end());
    EXPECT_EQ(&(*it), &map.get(SECOND_TENSOR));

    // When we end iterating on the map
    ++it;

    // Then the iterator is the end iterator
    EXPECT_EQ(it, map.end());
}

TEST_F(TensorMapShould, copyItself)
{
    // Given a tensor map, containing both tensors
    const TensorMap<float, KeyEnum> map({firstTensor, secondTensor});

    // When we create a copy of this tensor map
    const TensorMap<float, KeyEnum> copy(map);

    // Then this copy should contain the same tensors
    const Tensor<float, 2>& tensor1 = dynamic_cast<const Tensor<float, 2>&>(copy.get(FIRST_TENSOR));
    EXPECT_EQ(&tensor1, firstTensor);
    const Tensor<float, 3>& tensor2 = dynamic_cast<const Tensor<float, 3>&>(copy.get(SECOND_TENSOR));
    EXPECT_EQ(&tensor2, secondTensor);

}
