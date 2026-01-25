#include <module/Module.h>

#include <module/api.h>

template <typename ValueType>
HOST
Module<ValueType>::~Module()
{}

template class MY_NEURAL_NETWORK_LIB__MODULE__API Module<float>;
template class MY_NEURAL_NETWORK_LIB__MODULE__API Module<double>;
