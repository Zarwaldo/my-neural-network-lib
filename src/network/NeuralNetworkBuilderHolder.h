#pragma once

#include <map>
#include <vector>
#include <string>

class AbstractInitializer;
template <typename ValueType>
class AbstractNetworkBuilder;
template <typename BaseType>
class AbstractRtti;
template <typename ValueType>
class AbstractTensor;

template <typename ValueType>
class NeuralNetworkBuilderHolder
{
public:
    NeuralNetworkBuilderHolder();
    NeuralNetworkBuilderHolder(const NeuralNetworkBuilderHolder& other) = delete;
    NeuralNetworkBuilderHolder(NeuralNetworkBuilderHolder&& other);

    ~NeuralNetworkBuilderHolder();

    NeuralNetworkBuilderHolder& operator=(const NeuralNetworkBuilderHolder& other) = delete;
    NeuralNetworkBuilderHolder& operator=(NeuralNetworkBuilderHolder&& other);

    std::map<std::string, void*> add(const AbstractRtti<AbstractNetworkBuilder<ValueType>>& rtti, AbstractInitializer&& initializer, const AbstractNetworkBuilder<ValueType>** resultBuilder = nullptr);
    void remove(const AbstractNetworkBuilder<ValueType>& builder);

private:
    std::vector<AbstractNetworkBuilder<ValueType>*> m_builders;
};
