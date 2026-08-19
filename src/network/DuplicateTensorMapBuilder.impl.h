#pragma once

#include <network/DuplicateTensorMapBuilder.h>

#include <network/DuplicateTensorBuilder.h>
#include <network/NeuralNetworkPartHolder.h>

#include <tensor/TensorMap.h>

#include <rtti/RttiHolder.impl.h>

#include <vector>

template <typename ScalarType>
struct DuplicateTensorMapBuilderPimpl
{
    DuplicateTensorMapBuilderPimpl(NeuralNetwork<ScalarType>& network, const AbstractTensorMap<ScalarType>& originalTensorMap, const std::map<const AbstractTensor<ScalarType>*, AbstractTensor<ScalarType>*>& correspondingTensors, NeuralNetworkPartHolder<ScalarType>* networkPartHolder, std::vector<DuplicateTensorBuilder<ScalarType>>* subBuilders)
        : network(network)
        , originalTensorMap(originalTensorMap)
        , correspondingTensors(correspondingTensors)
        , networkPartHolder(networkPartHolder)
        , subBuilders(subBuilders)
    {}

    NeuralNetwork<ScalarType>& network;
    const AbstractTensorMap<ScalarType>& originalTensorMap;
    const std::map<const AbstractTensor<ScalarType>*, AbstractTensor<ScalarType>*>& correspondingTensors;
    NeuralNetworkPartHolder<ScalarType>* networkPartHolder;
    std::vector<DuplicateTensorBuilder<ScalarType>>* subBuilders;
};

template <typename ScalarType>
DuplicateTensorMapBuilder<ScalarType>::DuplicateTensorMapBuilder(NeuralNetwork<ScalarType>& network, const AbstractTensorMap<ScalarType>& tensorMap, const std::map<const AbstractTensor<ScalarType>*, AbstractTensor<ScalarType>*>& correspondingTensors)
    : m_pimpl(
        new DuplicateTensorMapBuilderPimpl<ScalarType>(
            network,
            tensorMap,
            correspondingTensors,
            new NeuralNetworkPartHolder<ScalarType>(network),
            new std::vector<DuplicateTensorBuilder<ScalarType>>{}
        )
    )
{
    for (std::pair<const AbstractTensor<ScalarType>*, AbstractTensor<ScalarType>*> entry : m_pimpl->correspondingTensors)
    {
        if (entry.first->sizes() != entry.second->sizes())
        {
            throw std::runtime_error("DuplicateTensorMapBuilder<ScalarType>::DuplicateTensorMapBuilder: The correspondingTensors map associates tensors of different sizes.");
        }
    }
}

template <typename ScalarType>
DuplicateTensorMapBuilder<ScalarType>::DuplicateTensorMapBuilder(DuplicateTensorMapBuilder&& other)
    : m_pimpl(other.m_pimpl)
{
    other.m_pimpl = nullptr;
}

template <typename ScalarType>
DuplicateTensorMapBuilder<ScalarType>::~DuplicateTensorMapBuilder()
{
    delete m_pimpl->networkPartHolder;
    delete m_pimpl->subBuilders;
    delete m_pimpl;
}

template <typename ScalarType>
DuplicateTensorMapBuilder<ScalarType>&
DuplicateTensorMapBuilder<ScalarType>::operator=(DuplicateTensorMapBuilder&& other)
{
    std::swap(m_pimpl, other.m_pimpl);
    return *this;
}

template <typename ScalarType>
std::map<std::string, void*>
DuplicateTensorMapBuilder<ScalarType>::build()
{
    if (!m_pimpl->subBuilders->empty())
        throw std::runtime_error("DuplicateTensorMapBuilder<ScalarType>::build: This builder was already used.");

    const size_t nbTensors = m_pimpl->originalTensorMap.nbTensors();
    AbstractTensor<ScalarType>** duplicatedTensorsArray = new AbstractTensor<ScalarType>*[nbTensors];

    for (size_t tensorIndex = 0; tensorIndex < nbTensors; ++tensorIndex)
    {
        const AbstractTensor<ScalarType>& tensor = m_pimpl->originalTensorMap.get(tensorIndex);
        const std::map<const AbstractTensor<ScalarType>*, AbstractTensor<ScalarType>*>::const_iterator correspondingTensorsIt = m_pimpl->correspondingTensors.find(&tensor);
        if (correspondingTensorsIt == m_pimpl->correspondingTensors.cend())
        {
            m_pimpl->subBuilders->push_back(DuplicateTensorBuilder<ScalarType>(m_pimpl->network, tensor));
            std::map<std::string, void*> buildingResult = m_pimpl->subBuilders->back().build();
            duplicatedTensorsArray[tensorIndex] = static_cast<AbstractTensor<ScalarType>*>(buildingResult[DuplicateTensorBuilder<ScalarType>::addedTensorKey]);
        }
        else
        {
            duplicatedTensorsArray[tensorIndex] = correspondingTensorsIt->second;
        }
    }

    const AbstractRtti<AbstractTensorMap<ScalarType>>* tensorMapRtti = m_pimpl->originalTensorMap.getClassRtti();
    AbstractTensorMap<ScalarType>& duplicatedTensorMap = m_pimpl->networkPartHolder->addTensorMap(tensorMapRtti->createInstance(Initializer<AbstractTensor<ScalarType>* const *>(static_cast<AbstractTensor<ScalarType>* const *>(duplicatedTensorsArray))));

    delete[] duplicatedTensorsArray;

    std::map<std::string, void*> result;
    result[addedTensorMapKey] = &duplicatedTensorMap;
    return result;
}

IMPLEMENT_RTTI(DuplicateTensorMapBuilder, AbstractNetworkBuilder<ScalarType>, PACK(typename), PACK(ScalarType))
