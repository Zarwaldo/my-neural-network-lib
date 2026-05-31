#pragma once

#include <module/ModuleMacros.h>

#include <plugins/nativemodules/TensorMapKeyEnums.h>

DECLARE_MODULE(
    PerceptronModuleImpl,
    ValueType,
    TensorSingleton,
    PerceptronParamsKeyEnum,
    TensorSingleton,
    PACK(InputDimension + 1),
    PACK(InputDimension + OutputDimension, OutputDimension),
    PACK(OutputDimension + 1),
    PACK(typename, size_t, size_t),
    PACK(ValueType, InputDimension, OutputDimension)
)
