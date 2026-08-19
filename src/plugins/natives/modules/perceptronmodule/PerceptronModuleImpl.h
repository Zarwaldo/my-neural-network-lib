#pragma once

#include <module/ModuleMacros.h>

#include <plugins/natives/tensormaps/TensorMapKeyEnums.h>

DECLARE_MODULE(
    PerceptronModuleImpl,
    ScalarType,
    TensorSingleton,
    PerceptronParamsKeyEnum,
    TensorSingleton,
    PACK(InputDimension + 1),
    PACK(InputDimension + OutputDimension, OutputDimension),
    PACK(OutputDimension + 1),
    PACK(typename, size_t, size_t),
    PACK(ScalarType, InputDimension, OutputDimension)
)
