#pragma once

#include <network/NullOutputReceiver.h>

template <typename ValueType>
NullOutputReceiver<ValueType>::NullOutputReceiver()
{}

template <typename ValueType>
NullOutputReceiver<ValueType>::~NullOutputReceiver()
{}

template <typename ValueType>
void
NullOutputReceiver<ValueType>::sendOutput(const AbstractTensorMap<ValueType>& outputMap)
{}

IMPLEMENT_RTTI(NullOutputReceiver, OutputReceiver<ValueType>, PACK(typename), PACK(ValueType))
