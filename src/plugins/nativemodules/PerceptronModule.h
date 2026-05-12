#pragma once

#include <module/MainTensorMapKeyEnums.h>
#include <module/ModuleMacros.h>

DECLARE_MODULE(
    PerceptronModule,
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
