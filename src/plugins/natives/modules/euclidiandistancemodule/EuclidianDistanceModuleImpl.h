#pragma once

#include <module/ModuleMacros.h>

#include <plugins/natives/tensormaps/TensorMapKeyEnums.h>

DECLARE_MODULE(
    EuclidianDistanceModuleImpl,
    ValueType,
    TensorPair,
    TensorSingleton,
    TensorSingleton,
    PACK(Dimension + 1, Dimension + 1),
    PACK(0),
    PACK(0),
    PACK(typename, size_t),
    PACK(ValueType, Dimension)
)
