#include <network/NeuralNetworkBuilderHolder.h>

#include <network/AbstractNetworkBuilder.h>

template <typename ValueType>
NeuralNetworkBuilderHolder<ValueType>::NeuralNetworkBuilderHolder()
    : m_builders()
{
}

template <typename ValueType>
NeuralNetworkBuilderHolder<ValueType>::NeuralNetworkBuilderHolder(NeuralNetworkBuilderHolder&& other)
    : m_builders(other.m_builders)
{
    other.m_builders.clear();
}

template <typename ValueType>
NeuralNetworkBuilderHolder<ValueType>::~NeuralNetworkBuilderHolder()
{
    if (m_builders.size() <= 0)
        return;

    for (long long int i = m_builders.size() - 1; i >= 0; --i)
    {
        delete m_builders[i];
    }
}

template <typename ValueType>
NeuralNetworkBuilderHolder<ValueType>&
NeuralNetworkBuilderHolder<ValueType>::operator=(NeuralNetworkBuilderHolder&& other)
{
    std::swap(m_builders, other.m_builders);
    return *this;
}

template <typename ValueType>
std::map<std::string, void*>
NeuralNetworkBuilderHolder<ValueType>::add(const AbstractRtti<AbstractNetworkBuilder<ValueType>>& rtti, AbstractInitializer&& initializer, const AbstractNetworkBuilder<ValueType>** resultBuilder)
{
    AbstractNetworkBuilder<ValueType>* newBuilder = rtti.createInstance(std::move(initializer));
    m_builders.push_back(newBuilder);

    if (resultBuilder != nullptr)
        *resultBuilder = newBuilder;

    return newBuilder->build();
}

template <typename ValueType>
void
NeuralNetworkBuilderHolder<ValueType>::remove(const AbstractNetworkBuilder<ValueType>& builder)
{
    const std::vector<AbstractNetworkBuilder<ValueType>*>::iterator builderIt = std::find(m_builders.begin(), m_builders.end(), &builder);
    if (builderIt == m_builders.end())
    {
        throw std::runtime_error("NeuralNetworkBuilderHolder::remove: The passed builder does not belong to this builder holder.");
    }

    delete &builder;

    m_builders.erase(builderIt);
}

template class NeuralNetworkBuilderHolder<float>;
template class NeuralNetworkBuilderHolder<double>;
