#pragma once

class AbstractInitializer;
template <typename BaseType>
class AbstractRtti;
template <typename ValueType>
class AbstractTensor;
class AbstractTensorIndex;
template <typename ValueType>
class AbstractTensorMap;
template <typename ValueType>
class Module;
template <typename ValueType>
class ParamTensorFiller;
template <typename ValueType>
class NeuralNetwork;

template <typename ValueType>
class NeuralNetworkPartHolderPimpl;

template <typename ValueType>
class NeuralNetworkPartHolder
{
public:
    NeuralNetworkPartHolder(NeuralNetwork<ValueType>& network);
    NeuralNetworkPartHolder(const NeuralNetworkPartHolder& other) = delete;
    NeuralNetworkPartHolder(NeuralNetworkPartHolder&& other);

    ~NeuralNetworkPartHolder();

    NeuralNetworkPartHolder& operator=(const NeuralNetworkPartHolder& other) = delete;
    NeuralNetworkPartHolder& operator=(NeuralNetworkPartHolder&& other);

    AbstractTensor<ValueType>& addTensor(const AbstractTensorIndex& size);
    AbstractTensorMap<ValueType>& addTensorMap(AbstractTensorMap<ValueType>* tensorMap);
    Module<ValueType>& addModule(const AbstractRtti<Module<ValueType>>& moduleRtti, AbstractInitializer&& modulesCtorParams, AbstractTensorMap<ValueType>& inputMap, AbstractTensorMap<ValueType>& outputMap, const ParamTensorFiller<ValueType>& paramTensorFiller);

private:
    NeuralNetworkPartHolderPimpl<ValueType>* m_pimpl;
};
