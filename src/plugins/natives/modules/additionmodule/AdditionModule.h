#pragma once

#include <module/ModuleWrapper.h>

#include <helpers/RawTuple.h>

#include <tensor/TensorIndex.h>

template <typename ScalarType, size_t Dimension>
class AdditionModuleImpl;

DECLARE_MODULE_WRAPPER(
    AdditionModule,
    ScalarType,
    AdditionModuleImpl,
    PACK(RawTuple<const TensorIndex<Dimension>&>),
    PACK(typename, size_t),
    PACK(ScalarType, Dimension)
)
