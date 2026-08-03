#include <gtest/gtest.h>

#include <module/TensorThreadDistributor.cuh>

#include <tensor/Tensor.h>
#include <tensor/TensorIndex.h>

class TensorThreadDistributorStaticallyShould : public ::testing::Test
{};

TEST_F(TensorThreadDistributorStaticallyShould, returnACorrectNumberOfThreadsDimension0)
{
    // When we ask the number of threads per block provided by TensorThreadDistributor in dimension 0
    const dim3 result = TensorThreadDistributor<float, 0>::getNbThreads();

    // Then the returned amount of threads should be of only 1 thread
    EXPECT_EQ(result.x, 1);
    EXPECT_EQ(result.y, 1);
    EXPECT_EQ(result.z, 1);
}

TEST_F(TensorThreadDistributorStaticallyShould, returnACorrectNumberOfThreadsDimension1)
{
    // Given a device with a certain supported amount of threads per block
    int device;
    cudaGetDevice(&device);

    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, device);

    const int maxThreadsPerBlock = prop.maxThreadsPerBlock;

    // When we ask the number of threads per block provided by TensorThreadDistributor in dimension 1
    const dim3 result = TensorThreadDistributor<float, 1>::getNbThreads();

    // Then the returned amount of threads should be supported and have less dimensions
    EXPECT_LE(result.x * result.y * result.z, maxThreadsPerBlock);
    EXPECT_EQ(result.y, 1);
    EXPECT_EQ(result.z, 1);
}

TEST_F(TensorThreadDistributorStaticallyShould, returnACorrectNumberOfThreadsDimension2)
{
    // Given a device with a certain supported amount of threads per block
    int device;
    cudaGetDevice(&device);

    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, device);

    const int maxThreadsPerBlock = prop.maxThreadsPerBlock;

    // When we ask the number of threads per block provided by TensorThreadDistributor in dimension 2
    const dim3 result = TensorThreadDistributor<float, 2>::getNbThreads();

    // Then the returned amount of threads should be supported and have less dimensions
    EXPECT_LE(result.x * result.y * result.z, maxThreadsPerBlock);
    EXPECT_EQ(result.z, 1);
}

TEST_F(TensorThreadDistributorStaticallyShould, returnACorrectNumberOfThreadsDimension3)
{
    // Given a device with a certain supported amount of threads per block
    int device;
    cudaGetDevice(&device);

    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, device);

    const int maxThreadsPerBlock = prop.maxThreadsPerBlock;

    // When we ask the number of threads per block provided by TensorThreadDistributor in dimension 3
    const dim3 result = TensorThreadDistributor<float, 3>::getNbThreads();

    // Then the returned amount of threads should be supported
    EXPECT_LE(result.x * result.y * result.z, maxThreadsPerBlock);
}

class TensorThreadDistributorShould : public ::testing::Test
{
protected:
    Tensor<bool, 5>* ownerTensor;

    void SetUp() override
    {
        ownerTensor = Tensor<bool, 5>::create({16, 16, 16, 2, 2});
        for (const TensorIndex<5>& index : *ownerTensor)
        {
            (*ownerTensor)[index] = false;
        }
    }

    void TearDown() override
    {
        delete ownerTensor;
    }
};

namespace
{
    GLOBAL void fillTensorWithTrueKernel(RawTensor<bool, 5>* rawTensor)
    {
        TensorThreadDistributor<bool, 5> distributor(*rawTensor);
        RawTensorIndex<5> index = distributor.askNewIndex(RawTensorIndex<5>());

        while (index.isValid())
        {
            __syncthreads();
            if (index < rawTensor->sizes())
                (*rawTensor)[index] = true;

            index = distributor.askNewIndex(index);
        }
    }

    GLOBAL void fillTensorWithTrueKernel2(RawTensor<bool, 5>* rawTensor)
    {
        TensorThreadDistributor<bool, 5> distributor(*rawTensor);
        distributor.iterate([rawTensor](const RawTensorIndex<5>& index) {
            if (index < rawTensor->sizes())
                (*rawTensor)[index] = true;
        });
    }
}

TEST_F(TensorThreadDistributorShould, applyFunctionOnAllTensorIndices)
{
    // Given a tensor allocated on the managed memory space, and an ownership-less copy written on managed memory space
    RawTensor<bool, 5>* managedTensor;
    cudaMallocManaged(&managedTensor, sizeof(RawTensor<bool, 5>));
    cudaMemcpy(managedTensor, &ownerTensor->getRawTensor(), sizeof(RawTensor<bool, 5>), ::cudaMemcpyHostToDevice);

    // When we run fillTensorWithTrueKernel which uses TensorThreadDistributor
    const dim3 nbBlocks = TensorThreadDistributor<bool, 5>::getNbBlocks(*managedTensor);
    const dim3 nbThreads = TensorThreadDistributor<bool, 5>::getNbThreads();

    fillTensorWithTrueKernel<<<nbBlocks, nbThreads>>>(managedTensor);
    cudaDeviceSynchronize();

    // Then all values of the tensor should be set to true
    for (const RawTensorIndex<5>& index : *managedTensor)
    {
        EXPECT_EQ((*managedTensor)[index], true);
    }

    // Free the tensor copy on managed memory
    cudaFree(managedTensor);
}

TEST_F(TensorThreadDistributorShould, applyFunctionOnAllTensorIndicesWithIterateMethod)
{
    // Given a tensor allocated on the managed memory space, and an ownership-less copy written on managed memory space
    RawTensor<bool, 5>* managedTensor;
    cudaMallocManaged(&managedTensor, sizeof(RawTensor<bool, 5>));
    cudaMemcpy(managedTensor, &ownerTensor->getRawTensor(), sizeof(RawTensor<bool, 5>), ::cudaMemcpyHostToDevice);

    // When we run fillTensorWithTrueKernel2 which uses TensorThreadDistributor::iterate
    const dim3 nbBlocks = TensorThreadDistributor<bool, 5>::getNbBlocks(*managedTensor);
    const dim3 nbThreads = TensorThreadDistributor<bool, 5>::getNbThreads();

    fillTensorWithTrueKernel2<<<nbBlocks, nbThreads>>>(managedTensor);
    cudaDeviceSynchronize();

    // Then all values of the tensor should be set to true
    for (const RawTensorIndex<5>& index : *managedTensor)
    {
        EXPECT_EQ((*managedTensor)[index], true);
    }

    // Free the tensor copy on managed memory
    cudaFree(managedTensor);
}
