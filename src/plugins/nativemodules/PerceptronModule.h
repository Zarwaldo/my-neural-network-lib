#pragma once

#include <module/ModuleWrapper.h>

#include <helpers/RawTuple.h>

#include <tensor/TensorIndex.h>

template <typename ValueType, size_t InputDimension, size_t OutputDimension>
class PerceptronModuleImpl;

DECLARE_MODULE_WRAPPER(
    PerceptronModule,
    ValueType,
    PerceptronModuleImpl,
    PACK(RawTuple<const TensorIndex<InputDimension + OutputDimension>&, const TensorIndex<OutputDimension>&>),
    PACK(typename, size_t, size_t),
    PACK(ValueType, InputDimension, OutputDimension)
)
