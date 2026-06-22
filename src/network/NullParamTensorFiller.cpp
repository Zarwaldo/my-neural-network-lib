#include <network/NullParamTensorFiller.h>

#include <network/api.h>

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

template class MY_NEURAL_NETWORK_LIB__NETWORK__API NullParamTensorFiller<float>;
template class MY_NEURAL_NETWORK_LIB__NETWORK__API NullParamTensorFiller<double>;
