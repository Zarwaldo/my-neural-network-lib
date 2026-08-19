#pragma once

#include <CommonMacros.h>

#include <helpers/RawTuple.h>

template <typename ScalarType, size_t Dimension>
class Tensor;

template <typename ScalarType, size_t OutputDimension, size_t... InputDimensions>
class TensorKernelWrapper
{
public:
    template <typename FunctionType, typename... ContextArgs>
    HOST
    static inline void call(FunctionType&& kernel, const RawTuple<Tensor<ScalarType, OutputDimension>*, const Tensor<ScalarType, InputDimensions>*...>& tensorsTuple, void* const managedMemory, const RawTuple<ContextArgs...>& contextArgs = makeRawTuple());
};

#include <module/TensorKernelWrapper.impl.cuh>
