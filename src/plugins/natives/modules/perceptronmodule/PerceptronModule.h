#pragma once

#include <module/ModuleWrapper.h>

#include <helpers/RawTuple.h>

#include <tensor/TensorIndex.h>

template <typename ScalarType, size_t InputDimension, size_t OutputDimension>
class PerceptronModuleImpl;

DECLARE_MODULE_WRAPPER(
    PerceptronModule,
    ScalarType,
    PerceptronModuleImpl,
    PACK(RawTuple<const TensorIndex<InputDimension + OutputDimension>&, const TensorIndex<OutputDimension>&>),
    PACK(typename, size_t, size_t),
    PACK(ScalarType, InputDimension, OutputDimension)
)
