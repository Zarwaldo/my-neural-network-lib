#pragma once

class AbstractInitializer;
template <typename BaseType>
class AbstractRtti;
template <typename ScalarType>
class AbstractTensor;
class AbstractTensorIndex;
template <typename ScalarType>
class AbstractTensorMap;
template <typename ScalarType>
class Module;
template <typename ScalarType>
class ParamTensorFiller;
template <typename ScalarType>
class NeuralNetwork;

template <typename ScalarType>
class NeuralNetworkPartHolderPimpl;

template <typename ScalarType>
class NeuralNetworkPartHolder
{
public:
    explicit NeuralNetworkPartHolder(NeuralNetwork<ScalarType>& network);
    NeuralNetworkPartHolder(const NeuralNetworkPartHolder& other) = delete;
    NeuralNetworkPartHolder(NeuralNetworkPartHolder&& other);

    ~NeuralNetworkPartHolder();

    NeuralNetworkPartHolder& operator=(const NeuralNetworkPartHolder& other) = delete;
    NeuralNetworkPartHolder& operator=(NeuralNetworkPartHolder&& other);

    AbstractTensor<ScalarType>& addTensor(const AbstractTensorIndex& size, bool addThicknessDimension = true);
    AbstractTensorMap<ScalarType>& addTensorMap(AbstractTensorMap<ScalarType>* tensorMap);
    Module<ScalarType>& addModule(const AbstractRtti<Module<ScalarType>>& moduleRtti, AbstractInitializer&& modulesCtorParams, AbstractTensorMap<ScalarType>& inputMap, AbstractTensorMap<ScalarType>& outputMap, const ParamTensorFiller<ScalarType>& paramTensorFiller);

private:
    NeuralNetworkPartHolderPimpl<ScalarType>* m_pimpl;
};
