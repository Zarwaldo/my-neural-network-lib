#include <gtest/gtest.h>

#include <tensor/Tensor.h>

class TensorShould : public ::testing::Test
{
protected:
    Tensor<float, 3>* tensor;

    void SetUp() override
    {
        tensor = Tensor<float, 3>::create({4, 3, 2});
    }

    void TearDown() override
    {
        delete tensor;
    }

    void initializeTensor()
    {
        const TensorIndex<3>& sizes = tensor->sizes();
        for (const TensorIndex<3>& index : *tensor)
        {
            // Give a distinct value for each stored float
            (*tensor)[index] = ((index[0] * sizes[1]) + index[1]) * sizes[2] + index[2];
        }
    }
};

TEST_F(TensorShould, giveItsNumberOfElements) {
    // Given a 4x3x2 float tensor

    // When we ask its number of elements
    const size_t result = tensor->nbElements();

    // Then the result should be 24
    EXPECT_EQ(result, 24);
}

TEST_F(TensorShould, giveItsMemorySize) {
    // Given a 4x3x2 float tensor

    // When we ask its memory size
    const size_t result = tensor->memorySize();

    // Then the result should be 96 bytes
    EXPECT_EQ(result, 96);
}

TEST_F(TensorShould, giveItsSizes) {
    // Given a 4x3x2 float tensor

    // When we ask its sizes
    const TensorIndex<3>& sizes = tensor->sizes();

    // Then the result should be 4x3x2
    EXPECT_EQ(sizes, TensorIndex<3>({4, 3, 2}));
}

TEST_F(TensorShould, giveAccessToItsInternalValues) {
    // Given an initialized 4x3x2 float tensor
    initializeTensor();

    // When reading values in the tensor
    const float valueAt000 = (*tensor)[TensorIndex<3>{0, 0, 0}];
    const float valueAt211 = (*tensor)[TensorIndex<3>{2, 1, 1}];
    const float valueAt321 = (*tensor)[TensorIndex<3>{3, 2, 1}];

    // Then the read values should be the ones with which the tensor was initialized
    EXPECT_EQ(valueAt000, 0.0f);
    EXPECT_EQ(valueAt211, 15.0f);
    EXPECT_EQ(valueAt321, 23.0f);
}

TEST_F(TensorShould, createItsSubtensors) {
    // Given an initialized 4x3x2 float tensor
    initializeTensor();

    // When we extract one of its subtensors
    const Tensor<float, 3> subtensor = tensor->subtensor({1, 1, 0}, {3, 2, 2});

    // Then this subtensor should be of size 2X1x2
    EXPECT_EQ(subtensor.sizes(), TensorIndex<3>({2, 1, 2}));
    // Its value at (0, 0, 0) should be the base tensor's value at (1, 1, 0), which is 8
    const float& value000 = subtensor[TensorIndex<3>{0, 0, 0}];
    EXPECT_EQ(value000, 8.0f);
    // Its value at (0, 0, 1) should be the base tensor's value at (1, 1, 1), which is 9
    const float& value001 = subtensor[TensorIndex<3>{0, 0, 1}];
    EXPECT_EQ(value001, 9.0f);
    // Its value at (1, 0, 0) should be the base tensor's value at (2, 1, 0), which is 14
    const float& value100 = subtensor[TensorIndex<3>{1, 0, 0}];
    EXPECT_EQ(value100, 14.0f);
    // Its value at (1, 0, 1) should be the base tensor's value at (2, 1, 1), which is 15
    const float& value101 = subtensor[TensorIndex<3>{1, 0, 1}];
    EXPECT_EQ(value101, 15.0f);
}

TEST_F(TensorShould, dereferenceUnitTensors) {
    // Given an initialized tensor and a unit subtensor of the base tensor, containing the element of index {2, 1, 0}
    initializeTensor();
    const Tensor<float, 3> subtensor = tensor->subtensor({2, 1, 0}, {3, 2, 1});

    // When we dereference this subtensor
    const float& value = *subtensor;

    // Then the result should be the value at index {2, 1, 0} in the base tensor, which is 14
    EXPECT_EQ(value, 14.0f);
}

TEST_F(TensorShould, dereferenceIterators) {
    // Given a tensor and a tensor index
    TensorIndex<3> index = {2, 1, 1};

    // When we create an iterator of this index and dereference it
    RawTensor<float, 3>::Iterator it = tensor->iterator(index);
    const TensorIndex<3>& result = *it;

    // Then the resulting index should be the same as the first one
    EXPECT_EQ(result, index);
}

TEST_F(TensorShould, incrementIterators0) {
    // Given a tensor and one of its iterators
    TensorIndex<3> index = {0, 0, 0};
    RawTensor<float, 3>::Iterator it = tensor->iterator(index);

    // When we increment this iterator
    RawTensor<float, 3>::Iterator& result = ++it;

    // Then the returned iterator is the same as the first one and the iterator was incremented
    EXPECT_EQ(&result, &it);
    RawTensorIndex<3> expectedIndex = {0, 0, 1};
    EXPECT_EQ(*it, expectedIndex);
}

TEST_F(TensorShould, incrementIterators1) {
    // Given a tensor and one of its iterators
    TensorIndex<3> index = {0, 0, 1};
    RawTensor<float, 3>::Iterator it = tensor->iterator(index);

    // When we increment this iterator
    RawTensor<float, 3>::Iterator& result = ++it;

    // Then the returned iterator is the same as the first one and the iterator was incremented
    EXPECT_EQ(&result, &it);
    RawTensorIndex<3> expectedIndex = {0, 1, 0};
    EXPECT_EQ(*it, expectedIndex);
}

TEST_F(TensorShould, incrementIterators2) {
    // Given a tensor and one of its iterators
    TensorIndex<3> index = {0, 1, 1};
    RawTensor<float, 3>::Iterator it = tensor->iterator(index);

    // When we increment this iterator
    RawTensor<float, 3>::Iterator& result = ++it;

    // Then the returned iterator is the same as the first one and the iterator was incremented
    EXPECT_EQ(&result, &it);
    RawTensorIndex<3> expectedIndex = {0, 2, 0};
    EXPECT_EQ(*it, expectedIndex);
}

TEST_F(TensorShould, incrementIterators3) {
    // Given a tensor and one of its iterators
    TensorIndex<3> index = {0, 2, 1};
    RawTensor<float, 3>::Iterator it = tensor->iterator(index);

    // When we increment this iterator
    RawTensor<float, 3>::Iterator& result = ++it;

    // Then the returned iterator is the same as the first one and the iterator was incremented
    EXPECT_EQ(&result, &it);
    RawTensorIndex<3> expectedIndex = {1, 0, 0};
    EXPECT_EQ(*it, expectedIndex);
}

TEST_F(TensorShould, incrementIterators4) {
    // Given a tensor and one of its iterators
    TensorIndex<3> index = {3, 2, 1};
    RawTensor<float, 3>::Iterator it = tensor->iterator(index);

    // When we increment this iterator
    RawTensor<float, 3>::Iterator& result = ++it;

    // Then the returned iterator is the same as the first one and the iterator was incremented
    EXPECT_EQ(&result, &it);
    RawTensorIndex<3> expectedIndex = RawTensorIndex<3>();
    EXPECT_EQ(*it, expectedIndex);
}

TEST_F(TensorShould, tellEqualIteratorsAreEqual) {
    // Given a tensor and two equal iterators
    TensorIndex<3> index = {0, 2, 1};
    RawTensor<float, 3>::Iterator firstIt = tensor->iterator(index);
    RawTensor<float, 3>::Iterator secondIt = tensor->iterator(index);

    // When we check whether firstIt and secondIt are equal
    const bool result = firstIt == secondIt;

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorShould, tellDifferentIteratorsAreNotEqual) {
    // Given a tensor and two different iterators
    TensorIndex<3> firstIndex = {0, 2, 1};
    RawTensor<float, 3>::Iterator firstIt = tensor->iterator(firstIndex);
    TensorIndex<3> secondIndex = {3, 2, 1};
    RawTensor<float, 3>::Iterator secondIt = tensor->iterator(secondIndex);

    // When we check whether firstIt and secondIt are equal
    const bool result = firstIt == secondIt;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorShould, tellEqualIteratorsAreDifferent) {
    // Given a tensor and two equal iterators
    TensorIndex<3> index = {0, 2, 1};
    RawTensor<float, 3>::Iterator firstIt = tensor->iterator(index);
    RawTensor<float, 3>::Iterator secondIt = tensor->iterator(index);

    // When we check whether firstIt and secondIt are different
    const bool result = firstIt != secondIt;

    // Then the result should be false
    EXPECT_FALSE(result);
}

TEST_F(TensorShould, tellDifferentIteratorsAreDifferent) {
    // Given a tensor and two different iterators
    TensorIndex<3> firstIndex = {0, 2, 1};
    RawTensor<float, 3>::Iterator firstIt = tensor->iterator(firstIndex);
    TensorIndex<3> secondIndex = {3, 2, 1};
    RawTensor<float, 3>::Iterator secondIt = tensor->iterator(secondIndex);

    // When we check whether firstIt and secondIt are different
    const bool result = firstIt != secondIt;

    // Then the result should be true
    EXPECT_TRUE(result);
}

TEST_F(TensorShould, iterateOnAllItsIndices) {
    // Given a tensor and a list of indices
    std::vector<TensorIndex<3>> iteratedIndices;

    // When we record all the indices we can iterate on
    for (const TensorIndex<3>& index: *tensor)
        iteratedIndices.emplace_back(index);

    // Then the list should contain all possible indices in order
    const std::vector<TensorIndex<3>> expectedIndices = {
        {0, 0, 0},
        {0, 0, 1},
        {0, 1, 0},
        {0, 1, 1},
        {0, 2, 0},
        {0, 2, 1},
        {1, 0, 0},
        {1, 0, 1},
        {1, 1, 0},
        {1, 1, 1},
        {1, 2, 0},
        {1, 2, 1},
        {2, 0, 0},
        {2, 0, 1},
        {2, 1, 0},
        {2, 1, 1},
        {2, 2, 0},
        {2, 2, 1},
        {3, 0, 0},
        {3, 0, 1},
        {3, 1, 0},
        {3, 1, 1},
        {3, 2, 0},
        {3, 2, 1}
    };
    EXPECT_EQ(iteratedIndices.size(), expectedIndices.size());
    for (int i = 0; i < iteratedIndices.size(); i++)
        EXPECT_EQ(iteratedIndices[i], expectedIndices[i]);
}

class TensorMemoryManagementShould : public ::testing::Test
{};

TEST_F(TensorMemoryManagementShould, freeATensorsDataOnlyOnceWithCopyOfNestedLifespan) {
    // Given a tensor and a copy of it
    Tensor<float, 3>* tensor = Tensor<float, 3>::create({4, 3, 2});
    Tensor<float, 3>* copyTensor = new Tensor<float, 3>(*tensor);

    // When the copy is destroyed before the base tensor
    delete copyTensor;
    delete tensor;

    // Then the memory is correctly released
}

TEST_F(TensorMemoryManagementShould, freeATensorsDataOnlyOnceWithCopyOfShiftedLifespan) {
    // Given a tensor and a copy of it
    Tensor<float, 3>* tensor = Tensor<float, 3>::create({4, 3, 2});
    Tensor<float, 3>* copyTensor = new Tensor<float, 3>(*tensor);

    // When the copy is destroyed after the base tensor
    delete tensor;
    // The memory is already released here
    delete copyTensor;

    // Then the memory is correctly released
}

TEST_F(TensorMemoryManagementShould, freeATensorsDataOnlyOnceWithMovedTensorOfNestedLifespan) {
    // Given a tensor and a moved version of it
    Tensor<float, 3>* tensor = Tensor<float, 3>::create({4, 3, 2});
    Tensor<float, 3>* movedTensor = new Tensor<float, 3>(std::move(*tensor)); // Data ownership is transfered to the moved tensor

    // When the moved tensor is destroyed before the base tensor
    delete movedTensor;
    // The memory is already released here
    delete tensor;

    // Then the memory is correctly released
}

TEST_F(TensorMemoryManagementShould, freeATensorsDataOnlyOnceWithMovedTensorOfShiftedLifespan) {
    // Given a tensor and a moved version of it
    Tensor<float, 3>* tensor = Tensor<float, 3>::create({4, 3, 2});
    Tensor<float, 3>* movedTensor = new Tensor<float, 3>(std::move(*tensor)); // Data ownership is transfered to the moved tensor

    // When the copy is destroyed after the base tensor
    delete tensor;
    delete movedTensor;

    // Then the memory is correctly released
}
