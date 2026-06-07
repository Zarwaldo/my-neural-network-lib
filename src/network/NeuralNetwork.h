#pragma once

#include <CommonMacros.h>

#include <memory>

class AbstractInitializer;
template <typename BaseType>
class AbstractRtti;
template <typename ValueType>
class AbstractTensor;
class AbstractTensorIndex;
template <typename ValueType>
class InputProvider;
template <typename ValueType>
class Module;
template <typename ValueType>
class OutputReceiver;
template <typename ValueType>
class ParamTensorFiller;
template <typename BaseType>
class RttiHolder;
template <typename ValueType>
class AbstractTensorMap;

template <typename ValueType>
class NeuralNetworkPimpl;

template <typename ValueType>
class NeuralNetwork
{
public:
    NeuralNetwork(size_t thickness);
    NeuralNetwork(const NeuralNetwork& other) = delete;
    NeuralNetwork(NeuralNetwork&& other);

    ~NeuralNetwork();

    NeuralNetwork<ValueType>& operator=(const NeuralNetwork<ValueType>& other) = delete;
    NeuralNetwork<ValueType>& operator=(NeuralNetwork<ValueType>&& other);

    AbstractTensor<ValueType>& addTensor(const AbstractTensorIndex& size);
    AbstractTensorMap<ValueType>& addTensorMap(AbstractTensorMap<ValueType>* tensorMap);
    Module<ValueType>& addModule(const AbstractRtti<Module<ValueType>>& moduleRtti, AbstractInitializer&& modulesCtorParams, AbstractTensorMap<ValueType>& inputMap, AbstractTensorMap<ValueType>& outputMap, const ParamTensorFiller<ValueType>& paramTensorFiller);

    void setInput(AbstractTensorMap<ValueType>* map);
    void setOutput(AbstractTensorMap<ValueType>* map);

    bool canExecute() const;
    void execute(InputProvider<ValueType>& inputProvider, OutputReceiver<ValueType>& outputReceiver) const;
    void executeAll(InputProvider<ValueType>& inputProvider, OutputReceiver<ValueType>& outputReceiver) const;

private:
    NeuralNetworkPimpl<ValueType>* m_pimpl;
};
