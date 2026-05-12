#pragma once

#include <module/MainTensorMapKeyEnums.h>
#include <module/ModuleMacros.h>

DECLARE_MODULE(
    AdditionModule,
    ValueType,
    TensorSingleton,
    TensorSingleton,
    TensorSingleton,
    PACK(Dimension + 1),
    PACK(Dimension),
    PACK(Dimension + 1),
    PACK(typename, size_t),
    PACK(ValueType, Dimension)
)
