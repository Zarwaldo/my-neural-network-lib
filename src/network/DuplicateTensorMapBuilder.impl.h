#pragma once

#include <network/DuplicateTensorMapBuilder.h>

#include <network/DuplicateTensorBuilder.h>
#include <network/NeuralNetworkPartHolder.h>

#include <tensor/TensorMap.h>

#include <rtti/RttiHolder.impl.h>

#include <vector>

template <typename ValueType>
struct DuplicateTensorMapBuilderPimpl
{
    DuplicateTensorMapBuilderPimpl(NeuralNetwork<ValueType>& network, const AbstractTensorMap<ValueType>& originalTensorMap, const std::map<const AbstractTensor<ValueType>*, AbstractTensor<ValueType>*>& correspondingTensors, NeuralNetworkPartHolder<ValueType>* networkPartHolder, std::vector<DuplicateTensorBuilder<ValueType>>* subBuilders)
        : network(network)
        , originalTensorMap(originalTensorMap)
        , correspondingTensors(correspondingTensors)
        , networkPartHolder(networkPartHolder)
        , subBuilders(subBuilders)
    {}

    NeuralNetwork<ValueType>& network;
    const AbstractTensorMap<ValueType>& originalTensorMap;
    const std::map<const AbstractTensor<ValueType>*, AbstractTensor<ValueType>*>& correspondingTensors;
    NeuralNetworkPartHolder<ValueType>* networkPartHolder;
    std::vector<DuplicateTensorBuilder<ValueType>>* subBuilders;
};

template <typename ValueType>
DuplicateTensorMapBuilder<ValueType>::DuplicateTensorMapBuilder(NeuralNetwork<ValueType>& network, const AbstractTensorMap<ValueType>& tensorMap, const std::map<const AbstractTensor<ValueType>*, AbstractTensor<ValueType>*>& correspondingTensors)
    : m_pimpl(
        new DuplicateTensorMapBuilderPimpl<ValueType>(
            network,
            tensorMap,
            correspondingTensors,
            new NeuralNetworkPartHolder<ValueType>(network),
            new std::vector<DuplicateTensorBuilder<ValueType>>{}
        )
    )
{
    for (std::pair<const AbstractTensor<ValueType>*, AbstractTensor<ValueType>*> entry : m_pimpl->correspondingTensors)
    {
        if (entry.first->sizes() != entry.second->sizes())
        {
            throw std::runtime_error("DuplicateTensorMapBuilder<ValueType>::DuplicateTensorMapBuilder: The correspondingTensors map associates tensors of different sizes.");
        }
    }
}

template <typename ValueType>
DuplicateTensorMapBuilder<ValueType>::DuplicateTensorMapBuilder(DuplicateTensorMapBuilder&& other)
    : m_pimpl(other.m_pimpl)
{
    other.m_pimpl = nullptr;
}

template <typename ValueType>
DuplicateTensorMapBuilder<ValueType>::~DuplicateTensorMapBuilder()
{
    delete m_pimpl->networkPartHolder;
    delete m_pimpl->subBuilders;
    delete m_pimpl;
}

template <typename ValueType>
DuplicateTensorMapBuilder<ValueType>&
DuplicateTensorMapBuilder<ValueType>::operator=(DuplicateTensorMapBuilder&& other)
{
    std::swap(m_pimpl, other.m_pimpl);
    return *this;
}

template <typename ValueType>
std::map<std::string, void*>
DuplicateTensorMapBuilder<ValueType>::build()
{
    if (!m_pimpl->subBuilders->empty())
        throw std::runtime_error("DuplicateTensorMapBuilder<ValueType>::build: This builder was already used.");

    const size_t nbTensors = m_pimpl->originalTensorMap.nbTensors();
    AbstractTensor<ValueType>** duplicatedTensorsArray = new AbstractTensor<ValueType>*[nbTensors];

    for (size_t tensorIndex = 0; tensorIndex < nbTensors; ++tensorIndex)
    {
        const AbstractTensor<ValueType>& tensor = m_pimpl->originalTensorMap.get(tensorIndex);
        const std::map<const AbstractTensor<ValueType>*, AbstractTensor<ValueType>*>::const_iterator correspondingTensorsIt = m_pimpl->correspondingTensors.find(&tensor);
        if (correspondingTensorsIt == m_pimpl->correspondingTensors.cend())
        {
            m_pimpl->subBuilders->push_back(DuplicateTensorBuilder<ValueType>(m_pimpl->network, tensor));
            std::map<std::string, void*> buildingResult = m_pimpl->subBuilders->back().build();
            duplicatedTensorsArray[tensorIndex] = static_cast<AbstractTensor<ValueType>*>(buildingResult[DuplicateTensorBuilder<ValueType>::addedTensorKey]);
        }
        else
        {
            duplicatedTensorsArray[tensorIndex] = correspondingTensorsIt->second;
        }
    }

    const AbstractRtti<AbstractTensorMap<ValueType>>* tensorMapRtti = m_pimpl->originalTensorMap.getClassRtti();
    AbstractTensorMap<ValueType>& duplicatedTensorMap = m_pimpl->networkPartHolder->addTensorMap(tensorMapRtti->createInstance(Initializer<AbstractTensor<ValueType>* const *>(static_cast<AbstractTensor<ValueType>* const *>(duplicatedTensorsArray))));

    delete[] duplicatedTensorsArray;

    std::map<std::string, void*> result;
    result[addedTensorMapKey] = &duplicatedTensorMap;
    return result;
}

IMPLEMENT_RTTI(DuplicateTensorMapBuilder, AbstractNetworkBuilder<ValueType>, PACK(typename), PACK(ValueType))
