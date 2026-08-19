#pragma once

#include <network/NullInputProvider.h>

template <typename ScalarType>
NullInputProvider<ScalarType>::NullInputProvider()
{}

template <typename ScalarType>
NullInputProvider<ScalarType>::~NullInputProvider()
{}

template <typename ScalarType>
void
NullInputProvider<ScalarType>::getNewInput(AbstractTensorMap<ScalarType>& inputMap)
{}

template <typename ScalarType>
bool
NullInputProvider<ScalarType>::hasMoreThan(size_t nbInputs) const
{
    return true;
}

IMPLEMENT_RTTI(NullInputProvider, InputProvider<ScalarType>, PACK(typename), PACK(ScalarType))
