#pragma once

#include <module/TensorKernelWrapper.cuh>

#include <module/TensorThreadDistributor.cuh>

#include <tensor/Tensor.h>

template <typename InputType>
struct CopyTensorsToManagedMemory
{};

template <typename ScalarType, size_t Dimension>
struct CopyTensorsToManagedMemory<const Tensor<ScalarType, Dimension>*>
{
    HOST static const RawTensor<ScalarType, Dimension>* doIt(const Tensor<ScalarType, Dimension>* const& tensor, void** const availableManagedMemory)
    {
        cudaMemcpy(*availableManagedMemory, &tensor->getRawTensor(), sizeof(RawTensor<ScalarType, Dimension>), ::cudaMemcpyHostToDevice);
        RawTensor<ScalarType, Dimension>* const result = static_cast<RawTensor<ScalarType, Dimension>*>(*availableManagedMemory);
        *availableManagedMemory = result + 1;
        return result;
    }
};

template <typename ScalarType, size_t Dimension>
struct CopyTensorsToManagedMemory<Tensor<ScalarType, Dimension>*>
{
    HOST static RawTensor<ScalarType, Dimension>* doIt(Tensor<ScalarType, Dimension>* const& tensor, void** const availableManagedMemory)
    {
        return const_cast<RawTensor<ScalarType, Dimension>*>(CopyTensorsToManagedMemory<const Tensor<ScalarType, Dimension>*>::doIt(tensor, availableManagedMemory));
    }
};

template <typename ScalarType, size_t OutputDimension, size_t... InputDimensions>
template <typename FunctionType, typename... ContextArgs>
HOST
void
TensorKernelWrapper<ScalarType, OutputDimension, InputDimensions...>::call(FunctionType&& kernel, const RawTuple<Tensor<ScalarType, OutputDimension>*, const Tensor<ScalarType, InputDimensions>*...>& tensorsTuple, void* const managedMemory, const RawTuple<ContextArgs...>& contextArgs)
{
    static_assert(std::is_same_v<
        void,
        decltype(std::declval<FunctionType>()(
            std::declval<dim3>(),
            std::declval<dim3>(),
            std::declval<cudaStream_t>(),
            std::declval<const RawTuple<RawTensor<ScalarType, OutputDimension>*, const RawTensor<ScalarType, InputDimensions>*...>>(),
            std::declval<const RawTuple<ContextArgs...>>()
        ))
    >);

    void* availableManagedMemory = managedMemory;
    const RawTuple<RawTensor<ScalarType, OutputDimension>*, const RawTensor<ScalarType, InputDimensions>*...> managedTensorsTuple = tensorsTuple.template hostMap<CopyTensorsToManagedMemory>(&availableManagedMemory);

    const dim3 nbBlocks = TensorThreadDistributor<ScalarType, OutputDimension>::getNbBlocks(*managedTensorsTuple.template get<0>());
    const dim3 nbThreads = TensorThreadDistributor<ScalarType, OutputDimension>::getNbThreads();

    cudaStream_t stream;
    cudaStreamCreate(&stream);

    kernel(nbBlocks, nbThreads, stream, managedTensorsTuple, contextArgs);
    cudaStreamSynchronize(stream);

    cudaStreamDestroy(stream);
}
