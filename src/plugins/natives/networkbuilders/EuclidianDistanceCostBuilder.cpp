#include <plugins/natives/networkbuilders/EuclidianDistanceCostBuilder.h>

#include <network/NeuralNetwork.h>
#include <network/NeuralNetworkPartHolder.h>
#include <network/NullParamTensorFiller.h>

#include <plugins/natives/api.h>
#include <plugins/natives/tensormaps/TensorMapKeyEnums.h>
#include <plugins/natives/modules/euclidiandistancemodule/EuclidianDistanceModule.h>

#include <tensor/Tensor.h>
#include <tensor/TensorMap.h>

template <typename ValueType>
struct EuclidianDistanceCostBuilderPimpl
{
    EuclidianDistanceCostBuilderPimpl(NeuralNetworkPartHolder<ValueType>&& networkPartHolder, TensorMap<ValueType, TensorSingleton>& outputTensorMap, TensorMap<ValueType, TensorSingleton>& expectedTensorMap, Tensor<ValueType, 0>& costTensor)
        : networkPartHolder(std::move(networkPartHolder))
        , outputTensorMap(outputTensorMap)
        , expectedTensorMap(expectedTensorMap)
        , costTensor(costTensor)
    {}

    NeuralNetworkPartHolder<ValueType> networkPartHolder;
    TensorMap<ValueType, TensorSingleton>& outputTensorMap;
    TensorMap<ValueType, TensorSingleton>& expectedTensorMap;
    Tensor<ValueType, 0>& costTensor;
};

template <typename ValueType, size_t Dimension>
EuclidianDistanceCostBuilder<ValueType, Dimension>::EuclidianDistanceCostBuilder(NeuralNetwork<ValueType>& network, AbstractTensorMap<ValueType>& outputTensorMap, AbstractTensorMap<ValueType>& expectedTensorMap, AbstractTensor<ValueType>& costTensor)
    : m_pimpl(
        new EuclidianDistanceCostBuilderPimpl<ValueType>(
            NeuralNetworkPartHolder<ValueType>(network),
            static_cast<TensorMap<ValueType, TensorSingleton>&>(outputTensorMap),
            static_cast<TensorMap<ValueType, TensorSingleton>&>(expectedTensorMap),
            static_cast<Tensor<ValueType, 0>&>(costTensor)
        )
    )
{}

template <typename ValueType, size_t Dimension>
EuclidianDistanceCostBuilder<ValueType, Dimension>::EuclidianDistanceCostBuilder(EuclidianDistanceCostBuilder&& other)
    : m_pimpl(other.m_pimpl)
{
    other.m_pimpl = nullptr;
}

template <typename ValueType, size_t Dimension>
EuclidianDistanceCostBuilder<ValueType, Dimension>::~EuclidianDistanceCostBuilder()
{
    delete m_pimpl;
}

template <typename ValueType, size_t Dimension>
EuclidianDistanceCostBuilder<ValueType, Dimension>&
EuclidianDistanceCostBuilder<ValueType, Dimension>::operator=(EuclidianDistanceCostBuilder&& other)
{
    std::swap(m_pimpl, other.m_pimpl);
    return *this;
}

template <typename ValueType, size_t Dimension>
std::map<std::string, void*>
EuclidianDistanceCostBuilder<ValueType, Dimension>::build()
{
    AbstractTensorMap<ValueType>& inputTensorMap = m_pimpl->networkPartHolder.addTensorMap(
        new TensorMap<ValueType, TensorPair>{
            &m_pimpl->outputTensorMap.get(0),
            &m_pimpl->expectedTensorMap.get(0)
        }
    );

    AbstractTensorMap<ValueType>& outputTensorMap = m_pimpl->networkPartHolder.addTensorMap(
        new TensorMap<ValueType, TensorSingleton>{
            &m_pimpl->costTensor
        }
    );

    const RawTuple<const TensorIndex<0>&> moduleParameter = makeRawTuple<const TensorIndex<0>&>(TensorIndex<0>(true));
    const NullParamTensorFiller<ValueType> nullParamTensorFiller;
    Module<ValueType>& module = m_pimpl->networkPartHolder.addModule(
        *EuclidianDistanceModule<ValueType, Dimension>::getRtti(),
        Initializer<const RawTuple<const TensorIndex<0>&>&>(moduleParameter),
        inputTensorMap,
        outputTensorMap,
        nullParamTensorFiller
    );

    return {};
}

IMPLEMENT_RTTI(EuclidianDistanceCostBuilder, AbstractNetworkBuilder<ValueType>, PACK(typename, size_t), PACK(ValueType, Dimension))

template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API EuclidianDistanceCostBuilder<float, 0>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API EuclidianDistanceCostBuilder<float, 1>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API EuclidianDistanceCostBuilder<float, 2>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API EuclidianDistanceCostBuilder<float, 3>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API EuclidianDistanceCostBuilder<float, 4>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API EuclidianDistanceCostBuilder<float, 5>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API EuclidianDistanceCostBuilder<double, 0>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API EuclidianDistanceCostBuilder<double, 1>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API EuclidianDistanceCostBuilder<double, 2>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API EuclidianDistanceCostBuilder<double, 3>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API EuclidianDistanceCostBuilder<double, 4>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API EuclidianDistanceCostBuilder<double, 5>;
