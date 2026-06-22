#include <network/NullInputProvider.h>

#include <network/api.h>

template <typename ValueType>
NullInputProvider<ValueType>::NullInputProvider()
{}

template <typename ValueType>
NullInputProvider<ValueType>::~NullInputProvider()
{}

template <typename ValueType>
void
NullInputProvider<ValueType>::getNewInput(AbstractTensorMap<ValueType>& inputMap)
{}

template <typename ValueType>
bool
NullInputProvider<ValueType>::hasMoreThan(size_t nbInputs) const
{
    return true;
}

IMPLEMENT_RTTI(NullInputProvider, InputProvider<ValueType>, PACK(typename), PACK(ValueType))

template class MY_NEURAL_NETWORK_LIB__NETWORK__API NullInputProvider<float>;
template class MY_NEURAL_NETWORK_LIB__NETWORK__API NullInputProvider<double>;
