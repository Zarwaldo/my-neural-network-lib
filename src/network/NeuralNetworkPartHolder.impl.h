#pragma once

#include <network/NeuralNetworkPartHolder.h>

#include <network/NeuralNetwork.h>

template <typename ValueType>
class NeuralNetworkPartHolderPimpl
{
public:
    NeuralNetworkPartHolderPimpl(NeuralNetwork<ValueType>& network)
        : m_network(&network)
        , m_tensors()
        , m_tensorMaps()
        , m_modules()
    {}

    NeuralNetwork<ValueType>* const m_network;
    std::vector<AbstractTensor<ValueType>*> m_tensors;
    std::vector<AbstractTensorMap<ValueType>*> m_tensorMaps;
    std::vector<Module<ValueType>*> m_modules;
};

template <typename ValueType>
NeuralNetworkPartHolder<ValueType>::NeuralNetworkPartHolder(NeuralNetwork<ValueType>& network)
    : m_pimpl(new NeuralNetworkPartHolderPimpl<ValueType>(network))
{}

template <typename ValueType>
NeuralNetworkPartHolder<ValueType>::NeuralNetworkPartHolder(NeuralNetworkPartHolder&& other)
    : m_pimpl(other.m_pimpl)
{
    other.m_pimpl = nullptr;
}

template <typename ValueType>
NeuralNetworkPartHolder<ValueType>::~NeuralNetworkPartHolder()
{
    for (const Module<ValueType>* module : m_pimpl->m_modules)
    {
        m_pimpl->m_network->removeModule(*module);
    }

    for (const AbstractTensorMap<ValueType>* tensorMap : m_pimpl->m_tensorMaps)
    {
        m_pimpl->m_network->removeTensorMap(*tensorMap);
    }

    for (const AbstractTensor<ValueType>* tensor : m_pimpl->m_tensors)
    {
        m_pimpl->m_network->removeTensor(*tensor);
    }

    delete m_pimpl;
}

template <typename ValueType>
NeuralNetworkPartHolder<ValueType>&
NeuralNetworkPartHolder<ValueType>::operator=(NeuralNetworkPartHolder&& other)
{
    std::swap(m_pimpl, other.m_pimpl);
    return *this;
}

template <typename ValueType>
AbstractTensor<ValueType>&
NeuralNetworkPartHolder<ValueType>::addTensor(const AbstractTensorIndex& size, bool addThicknessDimension)
{
    AbstractTensor<ValueType>& tensor = m_pimpl->m_network->addTensor(size, addThicknessDimension);
    m_pimpl->m_tensors.push_back(&tensor);
    return tensor;
}

template <typename ValueType>
AbstractTensorMap<ValueType>&
NeuralNetworkPartHolder<ValueType>::addTensorMap(AbstractTensorMap<ValueType>* tensorMap)
{
    AbstractTensorMap<ValueType>& resultTensorMap = m_pimpl->m_network->addTensorMap(tensorMap);
    m_pimpl->m_tensorMaps.push_back(&resultTensorMap);
    return resultTensorMap;
}

template <typename ValueType>
Module<ValueType>&
NeuralNetworkPartHolder<ValueType>::addModule(const AbstractRtti<Module<ValueType>>& moduleRtti, AbstractInitializer&& modulesCtorParams, AbstractTensorMap<ValueType>& inputMap, AbstractTensorMap<ValueType>& outputMap, const ParamTensorFiller<ValueType>& paramTensorFiller)
{
    Module<ValueType>& module = m_pimpl->m_network->addModule(moduleRtti, std::move(modulesCtorParams), inputMap, outputMap, paramTensorFiller);
    m_pimpl->m_modules.push_back(&module);
    return module;
}
