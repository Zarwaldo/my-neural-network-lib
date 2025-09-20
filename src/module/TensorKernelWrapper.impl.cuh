#pragma once

#include <module/TensorKernelWrapper.cuh>

#include <module/TensorThreadDistributor.cuh>

#include <tensor/Tensor.h>

template <typename InputType>
struct CopyTensorsToManagedMemory
{};

template <typename ValueType, size_t Dimension>
struct CopyTensorsToManagedMemory<const Tensor<ValueType, Dimension>*>
{
    HOST static const RawTensor<ValueType, Dimension>* doIt(const Tensor<ValueType, Dimension>* const& tensor, void** const availableManagedMemory)
    {
        cudaMemcpy(*availableManagedMemory, &tensor->getRawTensor(), sizeof(RawTensor<ValueType, Dimension>), ::cudaMemcpyHostToDevice);
        RawTensor<ValueType, Dimension>* const result = static_cast<RawTensor<ValueType, Dimension>*>(*availableManagedMemory);
        *availableManagedMemory = result + 1;
        return result;
    }
};

template <typename ValueType, size_t Dimension>
struct CopyTensorsToManagedMemory<Tensor<ValueType, Dimension>*>
{
    HOST static RawTensor<ValueType, Dimension>* doIt(Tensor<ValueType, Dimension>* const& tensor, void** const availableManagedMemory)
    {
        return const_cast<RawTensor<ValueType, Dimension>*>(CopyTensorsToManagedMemory<const Tensor<ValueType, Dimension>*>::doIt(tensor, availableManagedMemory));
    }
};

template <typename ValueType, size_t OutputDimension, size_t... InputDimensions>
template <typename FunctionType, typename... ContextArgs>
HOST
void
TensorKernelWrapper<ValueType, OutputDimension, InputDimensions...>::call(FunctionType&& kernel, const RawTuple<Tensor<ValueType, OutputDimension>*, const Tensor<ValueType, InputDimensions>*...>& tensorsTuple, void* const managedMemory, const RawTuple<ContextArgs...>& contextArgs)
{
    static_assert(std::is_same_v<
        void,
        decltype(std::declval<FunctionType>()(
            std::declval<dim3>(),
            std::declval<dim3>(),
            std::declval<cudaStream_t>(),
            std::declval<const RawTuple<RawTensor<ValueType, OutputDimension>*, const RawTensor<ValueType, InputDimensions>*...>>(),
            std::declval<const RawTuple<ContextArgs...>>()
        ))
    >);

    void* availableManagedMemory = managedMemory;
    const RawTuple<RawTensor<ValueType, OutputDimension>*, const RawTensor<ValueType, InputDimensions>*...> managedTensorsTuple = tensorsTuple.template hostMap<CopyTensorsToManagedMemory>(&availableManagedMemory);

    const dim3 nbBlocks = TensorThreadDistributor<ValueType, OutputDimension>::getNbBlocks(*managedTensorsTuple.template get<0>());
    const dim3 nbThreads = TensorThreadDistributor<ValueType, OutputDimension>::getNbThreads();

    cudaStream_t stream;
    cudaStreamCreate(&stream);

    kernel(nbBlocks, nbThreads, stream, managedTensorsTuple, contextArgs);
    cudaStreamSynchronize(stream);

    cudaStreamDestroy(stream);
}
