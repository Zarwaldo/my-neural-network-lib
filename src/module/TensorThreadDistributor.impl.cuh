#pragma once

#include <module/TensorThreadDistributor.cuh>

#include <tensor/Tensor.h>

template <typename ValueType, size_t Dimension>
DEVICE
TensorThreadDistributor<ValueType, Dimension>::TensorThreadDistributor(RawTensor<ValueType, Dimension>& rawTensor)
    : m_rawTensor(rawTensor)
{}

template <typename ValueType, size_t Dimension>
DEVICE
TensorThreadDistributor<ValueType, Dimension>::~TensorThreadDistributor()
{}

template <typename ValueType, size_t Dimension>
HOST
dim3
TensorThreadDistributor<ValueType, Dimension>::getNbThreads()
{
    int device;
    cudaGetDevice(&device);

    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, device);

    const int maxThreadsPerBlock = prop.maxThreadsPerBlock;
    const unsigned int blocksDimension = (Dimension < 3) ? Dimension : 3;

    int logb = 0;
    for (int i = maxThreadsPerBlock; i > 1; i >>= 1)
    {
        logb++;
    }

    unsigned int non1value = (blocksDimension > 0) ? (1 << (logb / blocksDimension)) : 0;

    return dim3{
        (blocksDimension > 0) ? non1value : 1,
        (blocksDimension > 1) ? non1value : 1,
        (blocksDimension > 2) ? non1value : 1
    };
}

template <typename ValueType, size_t Dimension>
HOST
dim3
TensorThreadDistributor<ValueType, Dimension>::getNbBlocks(const RawTensor<ValueType, Dimension>& rawTensor)
{
    const dim3 nbThreads = getNbThreads();
    const TensorIndex<Dimension>& tensorSize = rawTensor.sizes();

    if constexpr (Dimension == 0)
    {
        return {
            1,
            1,
            1
        };
    }
    if constexpr (Dimension == 1)
    {
        return {
            (static_cast<int>(tensorSize[0]) + nbThreads.x - 1) / nbThreads.x,
            1,
            1
        };
    }
    if constexpr (Dimension == 2)
    {
        return {
            (static_cast<int>(tensorSize[0]) + nbThreads.x - 1) / nbThreads.x,
            (static_cast<int>(tensorSize[1]) + nbThreads.y - 1) / nbThreads.y,
            1
        };
    }
    if constexpr (Dimension >= 3)
    {
        return {
            (static_cast<int>(tensorSize[0]) + nbThreads.x - 1) / nbThreads.x,
            (static_cast<int>(tensorSize[1]) + nbThreads.y - 1) / nbThreads.y,
            (static_cast<int>(tensorSize[2]) + nbThreads.z - 1) / nbThreads.z
        };
    }
}

template <typename ValueType, size_t Dimension>
DEVICE
RawTensorIndex<Dimension>
TensorThreadDistributor<ValueType, Dimension>::askNewIndex(const RawTensorIndex<Dimension>& previous) const
{
    if (!previous.isValid())
    {
        size_t array[Dimension];
        for (size_t dim = 0; dim < Dimension; dim++)
            array[dim] = 0;

        if constexpr (Dimension >= 1)
            array[0] = blockIdx.x * blockDim.x + threadIdx.x;
        if constexpr (Dimension >= 2)
            array[1] = blockIdx.y * blockDim.y + threadIdx.y;
        if constexpr (Dimension >= 3)
            array[2] = blockIdx.z * blockDim.z + threadIdx.z;

        return RawTensorIndex<Dimension>(array);
    }

    bool isMax = true;
    for (size_t dim = 3; dim < Dimension; ++dim)
        if (previous[dim] < m_rawTensor.sizes()[dim] - 1)
        {
            isMax = false;
            break;
        }

    __syncthreads();

    if (isMax)
        return RawTensorIndex<Dimension>();

    return *(++m_rawTensor.iterator(previous));
}

template <typename ValueType, size_t Dimension>
template <typename FunctionType>
DEVICE
std::enable_if_t<
    std::is_same_v<
        void,
        decltype( // This method returns void and takes a function as a parameter
            std::declval<FunctionType>()(
                std::declval<const RawTensorIndex<Dimension>&>() // The parameter function takes a tensor index as parameter
            )
        )
    >,
    void
>
TensorThreadDistributor<ValueType, Dimension>::iterate<FunctionType>(const FunctionType&& function)
{
    RawTensorIndex<Dimension> index = askNewIndex(RawTensorIndex<Dimension>());

    while (index.isValid())
    {
        __syncthreads();
        function(index);

        index = askNewIndex(index);
    }
}
