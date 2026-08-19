#pragma once

#include <map>
#include <vector>
#include <string>

class AbstractInitializer;
template <typename ScalarType>
class AbstractNetworkBuilder;
template <typename BaseType>
class AbstractRtti;
template <typename ScalarType>
class AbstractTensor;

template <typename ScalarType>
class NeuralNetworkBuilderHolder
{
public:
    NeuralNetworkBuilderHolder();
    NeuralNetworkBuilderHolder(const NeuralNetworkBuilderHolder& other) = delete;
    NeuralNetworkBuilderHolder(NeuralNetworkBuilderHolder&& other);

    ~NeuralNetworkBuilderHolder();

    NeuralNetworkBuilderHolder& operator=(const NeuralNetworkBuilderHolder& other) = delete;
    NeuralNetworkBuilderHolder& operator=(NeuralNetworkBuilderHolder&& other);

    std::map<std::string, void*> add(const AbstractRtti<AbstractNetworkBuilder<ScalarType>>& rtti, AbstractInitializer&& initializer, const AbstractNetworkBuilder<ScalarType>** resultBuilder = nullptr);
    void remove(const AbstractNetworkBuilder<ScalarType>& builder);

private:
    std::vector<AbstractNetworkBuilder<ScalarType>*> m_builders;
};
