#pragma once

#include <network/DuplicateTensorBuilder.h>

#include <network/NeuralNetworkPartHolder.h>

#include <rtti/RttiHolder.impl.h>

#include <tensor/Tensor.h>

template <typename ValueType>
struct DuplicateTensorBuilderPimpl
{
    DuplicateTensorBuilderPimpl(NeuralNetwork<ValueType>& network, const AbstractTensor<ValueType>& originalTensor)
        : networkPartHolder(NeuralNetworkPartHolder<ValueType>(network))
        , originalTensor(originalTensor)
    {}

    NeuralNetworkPartHolder<ValueType> networkPartHolder;
    const AbstractTensor<ValueType>& originalTensor;
};

template <typename ValueType>
DuplicateTensorBuilder<ValueType>::DuplicateTensorBuilder(NeuralNetwork<ValueType>& network, const AbstractTensor<ValueType>& tensor)
    : m_pimpl(new DuplicateTensorBuilderPimpl<ValueType>(network, tensor))
{}

template <typename ValueType>
DuplicateTensorBuilder<ValueType>::DuplicateTensorBuilder(DuplicateTensorBuilder&& other)
    : m_pimpl(other.m_pimpl)
{
    other.m_pimpl = nullptr;
}

template <typename ValueType>
DuplicateTensorBuilder<ValueType>::~DuplicateTensorBuilder()
{
    delete m_pimpl;
}

template <typename ValueType>
DuplicateTensorBuilder<ValueType>&
DuplicateTensorBuilder<ValueType>::operator=(DuplicateTensorBuilder&& other)
{
    std::swap(m_pimpl, other.m_pimpl);
    return *this;
}

template <typename ValueType>
std::map<std::string, void*>
DuplicateTensorBuilder<ValueType>::build()
{
    AbstractTensor<ValueType>& tensor = m_pimpl->networkPartHolder.addTensor(m_pimpl->originalTensor.sizes(), false);

    std::map<std::string, void*> result;
    result[addedTensorKey] = &tensor;
    return result;
}

IMPLEMENT_RTTI(DuplicateTensorBuilder, AbstractNetworkBuilder<ValueType>, PACK(typename), PACK(ValueType))
