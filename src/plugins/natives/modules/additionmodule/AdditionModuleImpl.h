#pragma once

#include <module/ModuleMacros.h>

#include <plugins/natives/tensormaps/TensorMapKeyEnums.h>

DECLARE_MODULE(
    AdditionModuleImpl,
    ScalarType,
    TensorSingleton,
    TensorSingleton,
    TensorSingleton,
    PACK(Dimension + 1),
    PACK(Dimension),
    PACK(Dimension + 1),
    PACK(typename, size_t),
    PACK(ScalarType, Dimension)
)
