#include <plugins/natives/networkbuilders/EuclidianDistanceCostBuilder.h>

#include <network/NeuralNetwork.h>
#include <network/NeuralNetworkPartHolder.h>
#include <network/NullParamTensorFiller.h>

#include <plugins/natives/api.h>
#include <plugins/natives/tensormaps/TensorMapKeyEnums.h>
#include <plugins/natives/modules/euclidiandistancemodule/EuclidianDistanceModule.h>

#include <tensor/Tensor.h>
#include <tensor/TensorMap.h>

template <typename ScalarType>
struct EuclidianDistanceCostBuilderPimpl
{
    EuclidianDistanceCostBuilderPimpl(NeuralNetworkPartHolder<ScalarType>&& networkPartHolder, TensorMap<ScalarType, TensorSingleton>& outputTensorMap, TensorMap<ScalarType, TensorSingleton>& expectedTensorMap, Tensor<ScalarType, 0>& costTensor)
        : networkPartHolder(std::move(networkPartHolder))
        , outputTensorMap(outputTensorMap)
        , expectedTensorMap(expectedTensorMap)
        , costTensor(costTensor)
    {}

    NeuralNetworkPartHolder<ScalarType> networkPartHolder;
    TensorMap<ScalarType, TensorSingleton>& outputTensorMap;
    TensorMap<ScalarType, TensorSingleton>& expectedTensorMap;
    Tensor<ScalarType, 0>& costTensor;
};

template <typename ScalarType, size_t Dimension>
EuclidianDistanceCostBuilder<ScalarType, Dimension>::EuclidianDistanceCostBuilder(NeuralNetwork<ScalarType>& network, AbstractTensorMap<ScalarType>& outputTensorMap, AbstractTensorMap<ScalarType>& expectedTensorMap, AbstractTensor<ScalarType>& costTensor)
    : m_pimpl(
        new EuclidianDistanceCostBuilderPimpl<ScalarType>(
            NeuralNetworkPartHolder<ScalarType>(network),
            static_cast<TensorMap<ScalarType, TensorSingleton>&>(outputTensorMap),
            static_cast<TensorMap<ScalarType, TensorSingleton>&>(expectedTensorMap),
            static_cast<Tensor<ScalarType, 0>&>(costTensor)
        )
    )
{}

template <typename ScalarType, size_t Dimension>
EuclidianDistanceCostBuilder<ScalarType, Dimension>::EuclidianDistanceCostBuilder(EuclidianDistanceCostBuilder&& other)
    : m_pimpl(other.m_pimpl)
{
    other.m_pimpl = nullptr;
}

template <typename ScalarType, size_t Dimension>
EuclidianDistanceCostBuilder<ScalarType, Dimension>::~EuclidianDistanceCostBuilder()
{
    delete m_pimpl;
}

template <typename ScalarType, size_t Dimension>
EuclidianDistanceCostBuilder<ScalarType, Dimension>&
EuclidianDistanceCostBuilder<ScalarType, Dimension>::operator=(EuclidianDistanceCostBuilder&& other)
{
    std::swap(m_pimpl, other.m_pimpl);
    return *this;
}

template <typename ScalarType, size_t Dimension>
std::map<std::string, void*>
EuclidianDistanceCostBuilder<ScalarType, Dimension>::build()
{
    AbstractTensorMap<ScalarType>& inputTensorMap = m_pimpl->networkPartHolder.addTensorMap(
        new TensorMap<ScalarType, TensorPair>{
            &m_pimpl->outputTensorMap.get(0),
            &m_pimpl->expectedTensorMap.get(0)
        }
    );

    AbstractTensorMap<ScalarType>& outputTensorMap = m_pimpl->networkPartHolder.addTensorMap(
        new TensorMap<ScalarType, TensorSingleton>{
            &m_pimpl->costTensor
        }
    );

    const RawTuple<const TensorIndex<0>&> moduleParameter = makeRawTuple<const TensorIndex<0>&>(TensorIndex<0>(true));
    const NullParamTensorFiller<ScalarType> nullParamTensorFiller;
    Module<ScalarType>& module = m_pimpl->networkPartHolder.addModule(
        *EuclidianDistanceModule<ScalarType, Dimension>::getRtti(),
        Initializer<const RawTuple<const TensorIndex<0>&>&>(moduleParameter),
        inputTensorMap,
        outputTensorMap,
        nullParamTensorFiller
    );

    return {};
}

IMPLEMENT_RTTI(EuclidianDistanceCostBuilder, AbstractNetworkBuilder<ScalarType>, PACK(typename, size_t), PACK(ScalarType, Dimension))

template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceCostBuilder<float, 0>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceCostBuilder<float, 1>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceCostBuilder<float, 2>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceCostBuilder<float, 3>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceCostBuilder<float, 4>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceCostBuilder<float, 5>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceCostBuilder<double, 0>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceCostBuilder<double, 1>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceCostBuilder<double, 2>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceCostBuilder<double, 3>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceCostBuilder<double, 4>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceCostBuilder<double, 5>;
