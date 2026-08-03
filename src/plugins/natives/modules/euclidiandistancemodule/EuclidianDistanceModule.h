#pragma once

#include <module/ModuleWrapper.h>

#include <helpers/RawTuple.h>

#include <tensor/TensorIndex.h>

template <typename ValueType, size_t Dimension>
class EuclidianDistanceModuleImpl;

DECLARE_MODULE_WRAPPER(
    EuclidianDistanceModule,
    ValueType,
    EuclidianDistanceModuleImpl,
    PACK(RawTuple<const TensorIndex<0>&>),
    PACK(typename, size_t),
    PACK(ValueType, Dimension)
)
