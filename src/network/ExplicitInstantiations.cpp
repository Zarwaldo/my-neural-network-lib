#include <network/api.h>

#include <rtti/StaticRttiHolder.h>

#include <network/DuplicateTensorBuilder.impl.h>
#include <network/DuplicateTensorMapBuilder.impl.h>
#include <network/NeuralNetwork.impl.h>
#include <network/NeuralNetworkBuilderHolder.impl.h>
#include <network/NeuralNetworkPartHolder.impl.h>
#include <network/NullInputProvider.impl.h>
#include <network/NullOutputReceiver.impl.h>
#include <network/NullParamTensorFiller.impl.h>
#include <network/TrainingNeuralNetwork.impl.h>

template class MY_NEURAL_NETWORK_LIB__NETWORK__API DuplicateTensorBuilder<float>;
template class MY_NEURAL_NETWORK_LIB__NETWORK__API DuplicateTensorBuilder<double>;
StaticRttiHolder<AbstractNetworkBuilder<float>> duplicateTensorBuilderFloatRttiHolderInitializer([](RttiHolderToken<AbstractNetworkBuilder<float>>& token) {
    token.subscribe(
        new Rtti<
            AbstractNetworkBuilder<float>,
            DuplicateTensorBuilder<float>,
            BuildTimeList::TypeList<NeuralNetwork<float>&, const AbstractTensor<float>&>
        >("DuplicateTensorBuilder<float>")
    );
});
StaticRttiHolder<AbstractNetworkBuilder<double>> duplicateTensorBuilderDoubleRttiHolderInitializer([](RttiHolderToken<AbstractNetworkBuilder<double>>& token) {
    token.subscribe(
        new Rtti<
            AbstractNetworkBuilder<double>,
            DuplicateTensorBuilder<double>,
            BuildTimeList::TypeList<NeuralNetwork<double>&, const AbstractTensor<double>&>
        >("DuplicateTensorBuilder<double>")
    );
});

template class MY_NEURAL_NETWORK_LIB__NETWORK__API DuplicateTensorMapBuilder<float>;
template class MY_NEURAL_NETWORK_LIB__NETWORK__API DuplicateTensorMapBuilder<double>;
StaticRttiHolder<AbstractNetworkBuilder<float>> duplicateTensorMapBuilderFloatRttiHolderInitializer([](RttiHolderToken<AbstractNetworkBuilder<float>>& token) {
    token.subscribe(
        new Rtti<
            AbstractNetworkBuilder<float>,
            DuplicateTensorMapBuilder<float>,
            BuildTimeList::TypeList<NeuralNetwork<float>&, const AbstractTensorMap<float>&, const std::map<const AbstractTensor<float>*, AbstractTensor<float>*>&>
        >("DuplicateTensorMapBuilder<float>")
    );
});
StaticRttiHolder<AbstractNetworkBuilder<double>> duplicateTensorMapBuilderDoubleRttiHolderInitializer([](RttiHolderToken<AbstractNetworkBuilder<double>>& token) {
    token.subscribe(
        new Rtti<
            AbstractNetworkBuilder<double>,
            DuplicateTensorMapBuilder<double>,
            BuildTimeList::TypeList<NeuralNetwork<double>&, const AbstractTensorMap<double>&, const std::map<const AbstractTensor<double>*, AbstractTensor<double>*>&>
        >("DuplicateTensorMapBuilder<float>")
    );
});

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

template class MY_NEURAL_NETWORK_LIB__NETWORK__API TrainingNeuralNetwork<float>;
template class MY_NEURAL_NETWORK_LIB__NETWORK__API TrainingNeuralNetwork<double>;
