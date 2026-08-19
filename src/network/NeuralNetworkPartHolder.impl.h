#pragma once

#include <network/NeuralNetworkPartHolder.h>

#include <network/NeuralNetwork.h>

template <typename ScalarType>
class NeuralNetworkPartHolderPimpl
{
public:
    NeuralNetworkPartHolderPimpl(NeuralNetwork<ScalarType>& network)
        : m_network(&network)
        , m_tensors()
        , m_tensorMaps()
        , m_modules()
    {}

    NeuralNetwork<ScalarType>* const m_network;
    std::vector<AbstractTensor<ScalarType>*> m_tensors;
    std::vector<AbstractTensorMap<ScalarType>*> m_tensorMaps;
    std::vector<Module<ScalarType>*> m_modules;
};

template <typename ScalarType>
NeuralNetworkPartHolder<ScalarType>::NeuralNetworkPartHolder(NeuralNetwork<ScalarType>& network)
    : m_pimpl(new NeuralNetworkPartHolderPimpl<ScalarType>(network))
{}

template <typename ScalarType>
NeuralNetworkPartHolder<ScalarType>::NeuralNetworkPartHolder(NeuralNetworkPartHolder&& other)
    : m_pimpl(other.m_pimpl)
{
    other.m_pimpl = nullptr;
}

template <typename ScalarType>
NeuralNetworkPartHolder<ScalarType>::~NeuralNetworkPartHolder()
{
    if (m_pimpl == nullptr)
    {
        return;
    }

    for (const Module<ScalarType>* module : m_pimpl->m_modules)
    {
        m_pimpl->m_network->removeModule(*module);
    }

    for (const AbstractTensorMap<ScalarType>* tensorMap : m_pimpl->m_tensorMaps)
    {
        m_pimpl->m_network->removeTensorMap(*tensorMap);
    }

    for (const AbstractTensor<ScalarType>* tensor : m_pimpl->m_tensors)
    {
        m_pimpl->m_network->removeTensor(*tensor);
    }

    delete m_pimpl;
}

template <typename ScalarType>
NeuralNetworkPartHolder<ScalarType>&
NeuralNetworkPartHolder<ScalarType>::operator=(NeuralNetworkPartHolder&& other)
{
    std::swap(m_pimpl, other.m_pimpl);
    return *this;
}

template <typename ScalarType>
AbstractTensor<ScalarType>&
NeuralNetworkPartHolder<ScalarType>::addTensor(const AbstractTensorIndex& size, bool addThicknessDimension)
{
    AbstractTensor<ScalarType>& tensor = m_pimpl->m_network->addTensor(size, addThicknessDimension);
    m_pimpl->m_tensors.push_back(&tensor);
    return tensor;
}

template <typename ScalarType>
AbstractTensorMap<ScalarType>&
NeuralNetworkPartHolder<ScalarType>::addTensorMap(AbstractTensorMap<ScalarType>* tensorMap)
{
    AbstractTensorMap<ScalarType>& resultTensorMap = m_pimpl->m_network->addTensorMap(tensorMap);
    m_pimpl->m_tensorMaps.push_back(&resultTensorMap);
    return resultTensorMap;
}

template <typename ScalarType>
Module<ScalarType>&
NeuralNetworkPartHolder<ScalarType>::addModule(const AbstractRtti<Module<ScalarType>>& moduleRtti, AbstractInitializer&& modulesCtorParams, AbstractTensorMap<ScalarType>& inputMap, AbstractTensorMap<ScalarType>& outputMap, const ParamTensorFiller<ScalarType>& paramTensorFiller)
{
    Module<ScalarType>& module = m_pimpl->m_network->addModule(moduleRtti, std::move(modulesCtorParams), inputMap, outputMap, paramTensorFiller);
    m_pimpl->m_modules.push_back(&module);
    return module;
}
