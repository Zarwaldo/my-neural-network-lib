#pragma once

#include <network/NullOutputReceiver.h>

template <typename ScalarType>
NullOutputReceiver<ScalarType>::NullOutputReceiver()
{}

template <typename ScalarType>
NullOutputReceiver<ScalarType>::~NullOutputReceiver()
{}

template <typename ScalarType>
void
NullOutputReceiver<ScalarType>::sendOutput(const AbstractTensorMap<ScalarType>& outputMap)
{}

IMPLEMENT_RTTI(NullOutputReceiver, OutputReceiver<ScalarType>, PACK(typename), PACK(ScalarType))
