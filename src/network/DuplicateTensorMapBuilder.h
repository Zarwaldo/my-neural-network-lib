#pragma once

#include <network/AbstractNetworkBuilder.h>

#include <map>
#include <string>

template <typename ScalarType>
class AbstractTensorMap;
template <typename ScalarType>
class NeuralNetwork;

template <typename ScalarType>
struct DuplicateTensorMapBuilderPimpl;

template <typename ScalarType>
class DuplicateTensorMapBuilder : public AbstractNetworkBuilder<ScalarType>
{
public:
    DuplicateTensorMapBuilder(NeuralNetwork<ScalarType>& network, const AbstractTensorMap<ScalarType>& tensorMap, const std::map<const AbstractTensor<ScalarType>*, AbstractTensor<ScalarType>*>& correspondingTensors = {});
    DuplicateTensorMapBuilder(const DuplicateTensorMapBuilder& other) = delete;
    DuplicateTensorMapBuilder(DuplicateTensorMapBuilder&& other);

    virtual ~DuplicateTensorMapBuilder();

    DuplicateTensorMapBuilder& operator=(const DuplicateTensorMapBuilder& other) = delete;
    DuplicateTensorMapBuilder& operator=(DuplicateTensorMapBuilder&& other);

    virtual std::map<std::string, void*> build() override;

    static inline const std::string addedTensorMapKey = "added_tensor_map";

private:
    DuplicateTensorMapBuilderPimpl<ScalarType>* m_pimpl;

    DECLARE_RTTI(AbstractNetworkBuilder<ScalarType>)
};
