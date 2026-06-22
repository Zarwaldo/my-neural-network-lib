#pragma once

#include <CommonMacros.h>

#include <memory>
#include <vector>

class AbstractInitializer;
template <typename BaseType>
class AbstractRtti;
template <typename ValueType>
class AbstractTensor;
class AbstractTensorIndex;
template <typename ValueType>
class AbstractTensorMap;
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
class NeuralNetworkPimpl;

template <typename ValueType>
class NeuralNetwork
{
public:
    NeuralNetwork(size_t thickness);
    NeuralNetwork(const NeuralNetwork& other) = delete;
    NeuralNetwork(NeuralNetwork&& other);

    virtual ~NeuralNetwork();

    NeuralNetwork<ValueType>& operator=(const NeuralNetwork<ValueType>& other) = delete;
    NeuralNetwork<ValueType>& operator=(NeuralNetwork<ValueType>&& other);

    AbstractTensor<ValueType>& addTensor(const AbstractTensorIndex& size, bool addThicknessDimension = true);
    AbstractTensorMap<ValueType>& addTensorMap(AbstractTensorMap<ValueType>* tensorMap);
    Module<ValueType>& addModule(const AbstractRtti<Module<ValueType>>& moduleRtti, AbstractInitializer&& modulesCtorParams, AbstractTensorMap<ValueType>& inputMap, AbstractTensorMap<ValueType>& outputMap, const ParamTensorFiller<ValueType>& paramTensorFiller);

    void removeTensor(const AbstractTensor<ValueType>& tensor);
    void removeTensorMap(const AbstractTensorMap<ValueType>& tensorMap);
    void removeModule(const Module<ValueType>& module);

    virtual void setInput(AbstractTensorMap<ValueType>* map);
    virtual void setOutput(AbstractTensorMap<ValueType>* map);

    const std::vector<AbstractTensor<ValueType>*>& getTensors() const;
    const std::vector<AbstractTensorMap<ValueType>*>& getTensorMaps() const;
    const std::vector<Module<ValueType>*>& getModules() const;

    const AbstractTensorMap<ValueType>* getInput() const;
    AbstractTensorMap<ValueType>* getInput();
    const AbstractTensorMap<ValueType>* getOutput() const;
    AbstractTensorMap<ValueType>* getOutput();

    bool canExecute() const;
    void execute(InputProvider<ValueType>& inputProvider, OutputReceiver<ValueType>& outputReceiver) const;
    void executeAll(InputProvider<ValueType>& inputProvider, OutputReceiver<ValueType>& outputReceiver) const;

    size_t thickness() const;

private:
    NeuralNetworkPimpl<ValueType>* m_pimpl;
};
