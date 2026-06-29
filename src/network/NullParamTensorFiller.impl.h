#pragma once

#include <network/NullParamTensorFiller.h>

template <typename ValueType>
NullParamTensorFiller<ValueType>::NullParamTensorFiller()
{}

template <typename ValueType>
NullParamTensorFiller<ValueType>::~NullParamTensorFiller()
{}

template <typename ValueType>
void
NullParamTensorFiller<ValueType>::fill(AbstractTensorMap<ValueType>& tensorMap) const
{}

IMPLEMENT_RTTI(NullParamTensorFiller, ParamTensorFiller<ValueType>, PACK(typename), PACK(ValueType))
