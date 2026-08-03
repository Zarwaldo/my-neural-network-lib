#pragma once

#include <network/AbstractNetworkBuilder.h>

template <typename ValueType>
class NeuralNetwork;
template <typename ValueType>
class AbstractTensor;
template <typename ValueType>
class AbstractTensorMap;

template <typename ValueType>
struct EuclidianDistanceCostBuilderPimpl;

template <typename ValueType, size_t Dimension>
class EuclidianDistanceCostBuilder : public AbstractNetworkBuilder<ValueType>
{
public:
    EuclidianDistanceCostBuilder(NeuralNetwork<ValueType>& network, AbstractTensorMap<ValueType>& outputTensorMap, AbstractTensorMap<ValueType>& expectedTensorMap, AbstractTensor<ValueType>& costTensor);
    EuclidianDistanceCostBuilder(const EuclidianDistanceCostBuilder& other) = delete;
    EuclidianDistanceCostBuilder(EuclidianDistanceCostBuilder&& other);

    virtual ~EuclidianDistanceCostBuilder();

    EuclidianDistanceCostBuilder& operator=(const EuclidianDistanceCostBuilder& other) = delete;
    EuclidianDistanceCostBuilder& operator=(EuclidianDistanceCostBuilder&& other);

    virtual std::map<std::string, void*> build() override;

private:
    EuclidianDistanceCostBuilderPimpl<ValueType>* m_pimpl;

    DECLARE_RTTI(AbstractNetworkBuilder<ValueType>)
};
