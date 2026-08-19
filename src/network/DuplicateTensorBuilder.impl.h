#pragma once

#include <network/DuplicateTensorBuilder.h>

#include <network/NeuralNetworkPartHolder.h>

#include <rtti/RttiHolder.impl.h>

#include <tensor/Tensor.h>

template <typename ScalarType>
struct DuplicateTensorBuilderPimpl
{
    DuplicateTensorBuilderPimpl(NeuralNetwork<ScalarType>& network, const AbstractTensor<ScalarType>& originalTensor)
        : networkPartHolder(NeuralNetworkPartHolder<ScalarType>(network))
        , originalTensor(originalTensor)
    {}

    NeuralNetworkPartHolder<ScalarType> networkPartHolder;
    const AbstractTensor<ScalarType>& originalTensor;
};

template <typename ScalarType>
DuplicateTensorBuilder<ScalarType>::DuplicateTensorBuilder(NeuralNetwork<ScalarType>& network, const AbstractTensor<ScalarType>& tensor)
    : m_pimpl(new DuplicateTensorBuilderPimpl<ScalarType>(network, tensor))
{}

template <typename ScalarType>
DuplicateTensorBuilder<ScalarType>::DuplicateTensorBuilder(DuplicateTensorBuilder&& other)
    : m_pimpl(other.m_pimpl)
{
    other.m_pimpl = nullptr;
}

template <typename ScalarType>
DuplicateTensorBuilder<ScalarType>::~DuplicateTensorBuilder()
{
    delete m_pimpl;
}

template <typename ScalarType>
DuplicateTensorBuilder<ScalarType>&
DuplicateTensorBuilder<ScalarType>::operator=(DuplicateTensorBuilder&& other)
{
    std::swap(m_pimpl, other.m_pimpl);
    return *this;
}

template <typename ScalarType>
std::map<std::string, void*>
DuplicateTensorBuilder<ScalarType>::build()
{
    AbstractTensor<ScalarType>& tensor = m_pimpl->networkPartHolder.addTensor(m_pimpl->originalTensor.sizes(), false);

    std::map<std::string, void*> result;
    result[addedTensorKey] = &tensor;
    return result;
}

IMPLEMENT_RTTI(DuplicateTensorBuilder, AbstractNetworkBuilder<ScalarType>, PACK(typename), PACK(ScalarType))
