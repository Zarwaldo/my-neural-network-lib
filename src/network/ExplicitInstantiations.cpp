#include <network/api.h>

#include <network/NeuralNetwork.impl.h>
#include <network/NeuralNetworkBuilderHolder.impl.h>
#include <network/NeuralNetworkPartHolder.impl.h>
#include <network/NullInputProvider.impl.h>
#include <network/NullOutputReceiver.impl.h>
#include <network/NullParamTensorFiller.impl.h>

template class MY_NEURAL_NETWORK_LIB__NETWORK__API NeuralNetwork<float>;
template class MY_NEURAL_NETWORK_LIB__NETWORK__API NeuralNetwork<double>;
template class NeuralNetworkBuilderHolder<float>;
template class NeuralNetworkBuilderHolder<double>;

template class MY_NEURAL_NETWORK_LIB__NETWORK__API NeuralNetworkPartHolder<float>;
template class MY_NEURAL_NETWORK_LIB__NETWORK__API NeuralNetworkPartHolder<double>;

template class MY_NEURAL_NETWORK_LIB__NETWORK__API NullInputProvider<float>;
template class MY_NEURAL_NETWORK_LIB__NETWORK__API NullInputProvider<double>;

template class MY_NEURAL_NETWORK_LIB__NETWORK__API NullOutputReceiver<float>;
template class MY_NEURAL_NETWORK_LIB__NETWORK__API NullOutputReceiver<double>;

template class MY_NEURAL_NETWORK_LIB__NETWORK__API NullParamTensorFiller<float>;
template class MY_NEURAL_NETWORK_LIB__NETWORK__API NullParamTensorFiller<double>;
