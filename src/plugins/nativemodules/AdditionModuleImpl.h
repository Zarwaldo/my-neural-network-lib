#pragma once

#include <module/ModuleMacros.h>

#include <plugins/nativemodules/TensorMapKeyEnums.h>

DECLARE_MODULE(
    AdditionModuleImpl,
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
