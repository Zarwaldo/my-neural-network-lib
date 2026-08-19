#pragma once

#include <network/NullParamTensorFiller.h>

template <typename ScalarType>
NullParamTensorFiller<ScalarType>::NullParamTensorFiller()
{}

template <typename ScalarType>
NullParamTensorFiller<ScalarType>::~NullParamTensorFiller()
{}

template <typename ScalarType>
void
NullParamTensorFiller<ScalarType>::fill(AbstractTensorMap<ScalarType>& tensorMap) const
{}

IMPLEMENT_RTTI(NullParamTensorFiller, ParamTensorFiller<ScalarType>, PACK(typename), PACK(ScalarType))
