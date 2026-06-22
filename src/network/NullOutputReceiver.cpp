#include <network/NullOutputReceiver.h>

#include <network/api.h>

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

template class MY_NEURAL_NETWORK_LIB__NETWORK__API NullOutputReceiver<float>;
template class MY_NEURAL_NETWORK_LIB__NETWORK__API NullOutputReceiver<double>;
