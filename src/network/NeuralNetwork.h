#pragma once

#include <CommonMacros.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

class AbstractInitializer;
template <typename BaseType>
class AbstractRtti;
template <typename ScalarType>
class AbstractNetworkBuilder;
template <typename ScalarType>
class AbstractTensor;
class AbstractTensorIndex;
template <typename ScalarType>
class AbstractTensorMap;
template <typename ScalarType>
class InputProvider;
template <typename ScalarType>
class Module;
template <typename ScalarType>
class OutputReceiver;
template <typename ScalarType>
class ParamTensorFiller;
template <typename BaseType>
class RttiHolder;

template <typename ScalarType>
class NeuralNetworkPimpl;

template <typename ScalarType>
class NeuralNetwork
{
public:
    NeuralNetwork(size_t thickness);
    NeuralNetwork(const NeuralNetwork& other) = delete;
    NeuralNetwork(NeuralNetwork&& other);

    virtual ~NeuralNetwork();

    NeuralNetwork<ScalarType>& operator=(const NeuralNetwork<ScalarType>& other) = delete;
    NeuralNetwork<ScalarType>& operator=(NeuralNetwork<ScalarType>&& other);

    AbstractTensor<ScalarType>& addTensor(const AbstractTensorIndex& size, bool addThicknessDimension = true);
    AbstractTensorMap<ScalarType>& addTensorMap(AbstractTensorMap<ScalarType>* tensorMap);
    Module<ScalarType>& addModule(const AbstractRtti<Module<ScalarType>>& moduleRtti, AbstractInitializer&& modulesCtorParams, AbstractTensorMap<ScalarType>& inputMap, AbstractTensorMap<ScalarType>& outputMap, const ParamTensorFiller<ScalarType>& paramTensorFiller);
    std::map<std::string, void*> build(const AbstractRtti<AbstractNetworkBuilder<ScalarType>>& builderRtti, AbstractInitializer&& initializer, const AbstractNetworkBuilder<ScalarType>** resultBuilder = nullptr);

    void removeTensor(const AbstractTensor<ScalarType>& tensor);
    void removeTensorMap(const AbstractTensorMap<ScalarType>& tensorMap);
    void removeModule(const Module<ScalarType>& module);
    void unbuild(const AbstractNetworkBuilder<ScalarType>& builder);

    virtual void setInput(AbstractTensorMap<ScalarType>* map);
    virtual void setOutput(AbstractTensorMap<ScalarType>* map);

    const std::vector<AbstractTensor<ScalarType>*>& getTensors() const;
    const std::vector<AbstractTensorMap<ScalarType>*>& getTensorMaps() const;
    const std::vector<Module<ScalarType>*>& getModules() const;

    const AbstractTensorMap<ScalarType>* getInput() const;
    AbstractTensorMap<ScalarType>* getInput();
    const AbstractTensorMap<ScalarType>* getOutput() const;
    AbstractTensorMap<ScalarType>* getOutput();

    bool canExecute() const;
    void execute(InputProvider<ScalarType>& inputProvider, OutputReceiver<ScalarType>& outputReceiver) const;
    void executeAll(InputProvider<ScalarType>& inputProvider, OutputReceiver<ScalarType>& outputReceiver) const;

    size_t thickness() const;

private:
    NeuralNetworkPimpl<ScalarType>* m_pimpl;
};
