#pragma once

#include <network/NeuralNetworkBuilderHolder.h>

#include <network/AbstractNetworkBuilder.h>

template <typename ScalarType>
NeuralNetworkBuilderHolder<ScalarType>::NeuralNetworkBuilderHolder()
    : m_builders()
{
}

template <typename ScalarType>
NeuralNetworkBuilderHolder<ScalarType>::NeuralNetworkBuilderHolder(NeuralNetworkBuilderHolder&& other)
    : m_builders(other.m_builders)
{
    other.m_builders.clear();
}

template <typename ScalarType>
NeuralNetworkBuilderHolder<ScalarType>::~NeuralNetworkBuilderHolder()
{
    if (m_builders.size() <= 0)
        return;

    for (long long int i = m_builders.size() - 1; i >= 0; --i)
    {
        delete m_builders[i];
    }
}

template <typename ScalarType>
NeuralNetworkBuilderHolder<ScalarType>&
NeuralNetworkBuilderHolder<ScalarType>::operator=(NeuralNetworkBuilderHolder&& other)
{
    std::swap(m_builders, other.m_builders);
    return *this;
}

template <typename ScalarType>
std::map<std::string, void*>
NeuralNetworkBuilderHolder<ScalarType>::add(const AbstractRtti<AbstractNetworkBuilder<ScalarType>>& rtti, AbstractInitializer&& initializer, const AbstractNetworkBuilder<ScalarType>** resultBuilder)
{
    AbstractNetworkBuilder<ScalarType>* newBuilder = rtti.createInstance(std::move(initializer));
    m_builders.push_back(newBuilder);

    if (resultBuilder != nullptr)
        *resultBuilder = newBuilder;

    return newBuilder->build();
}

template <typename ScalarType>
void
NeuralNetworkBuilderHolder<ScalarType>::remove(const AbstractNetworkBuilder<ScalarType>& builder)
{
    const std::vector<AbstractNetworkBuilder<ScalarType>*>::iterator builderIt = std::find(m_builders.begin(), m_builders.end(), &builder);
    if (builderIt == m_builders.end())
    {
        throw std::runtime_error("NeuralNetworkBuilderHolder::remove: The passed builder does not belong to this builder holder.");
    }

    delete &builder;

    m_builders.erase(builderIt);
}
