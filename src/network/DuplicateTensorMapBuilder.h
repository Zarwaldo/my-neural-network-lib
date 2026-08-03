#pragma once

#include <network/AbstractNetworkBuilder.h>

#include <map>
#include <string>

template <typename ValueType>
class AbstractTensorMap;
template <typename ValueType>
class NeuralNetwork;

template <typename ValueType>
struct DuplicateTensorMapBuilderPimpl;

template <typename ValueType>
class DuplicateTensorMapBuilder : public AbstractNetworkBuilder<ValueType>
{
public:
    DuplicateTensorMapBuilder(NeuralNetwork<ValueType>& network, const AbstractTensorMap<ValueType>& tensorMap, const std::map<const AbstractTensor<ValueType>*, AbstractTensor<ValueType>*>& correspondingTensors = {});
    DuplicateTensorMapBuilder(const DuplicateTensorMapBuilder& other) = delete;
    DuplicateTensorMapBuilder(DuplicateTensorMapBuilder&& other);

    virtual ~DuplicateTensorMapBuilder();

    DuplicateTensorMapBuilder& operator=(const DuplicateTensorMapBuilder& other) = delete;
    DuplicateTensorMapBuilder& operator=(DuplicateTensorMapBuilder&& other);

    virtual std::map<std::string, void*> build() override;

    static inline const std::string addedTensorMapKey = "added_tensor_map";

private:
    DuplicateTensorMapBuilderPimpl<ValueType>* m_pimpl;

    DECLARE_RTTI(AbstractNetworkBuilder<ValueType>)
};
