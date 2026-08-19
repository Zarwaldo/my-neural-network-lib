#pragma once

#include <network/AbstractNetworkBuilder.h>

template <typename ScalarType>
class NeuralNetwork;
template <typename ScalarType>
class AbstractTensor;
template <typename ScalarType>
class AbstractTensorMap;

template <typename ScalarType>
struct EuclidianDistanceCostBuilderPimpl;

template <typename ScalarType, size_t Dimension>
class EuclidianDistanceCostBuilder : public AbstractNetworkBuilder<ScalarType>
{
public:
    EuclidianDistanceCostBuilder(NeuralNetwork<ScalarType>& network, AbstractTensorMap<ScalarType>& outputTensorMap, AbstractTensorMap<ScalarType>& expectedTensorMap, AbstractTensor<ScalarType>& costTensor);
    EuclidianDistanceCostBuilder(const EuclidianDistanceCostBuilder& other) = delete;
    EuclidianDistanceCostBuilder(EuclidianDistanceCostBuilder&& other);

    virtual ~EuclidianDistanceCostBuilder();

    EuclidianDistanceCostBuilder& operator=(const EuclidianDistanceCostBuilder& other) = delete;
    EuclidianDistanceCostBuilder& operator=(EuclidianDistanceCostBuilder&& other);

    virtual std::map<std::string, void*> build() override;

private:
    EuclidianDistanceCostBuilderPimpl<ScalarType>* m_pimpl;

    DECLARE_RTTI(AbstractNetworkBuilder<ScalarType>)
};
