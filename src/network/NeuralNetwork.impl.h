#pragma once

#include <network/NeuralNetwork.h>

#include <module/Module.h>

#include <network/Flow.h>
#include <network/InputProvider.h>
#include <network/NeuralNetworkBuilderHolder.h>
#include <network/OutputReceiver.h>
#include <network/ParamTensorFiller.h>

#include <rtti/AbstractRtti.h>
#include <rtti/AbstractTemplateRtti.h>
#include <rtti/RttiHolder.h>
#include <rtti/TypenameArgId.h>

#include <tensor/Tensor.h>
#include <tensor/TensorMap.h>

#include <algorithm>
#include <optional>
#include <vector>
#include <map>

template <typename ScalarType>
class NeuralNetworkPimpl
{
public:
    NeuralNetworkPimpl(size_t thickness)
        : m_tensors()
        , m_tensorMaps()
        , m_modules()
        , m_inputMap(nullptr)
        , m_outputMap(nullptr)
        , m_neuralNetworkBuilderHolder(new NeuralNetworkBuilderHolder<ScalarType>())
        , m_thickness(thickness)
        , m_canExecuteCache()
    {}

    const Flow toFlow() const
    {
        Flow flow;

        for (const AbstractTensor<ScalarType>* tensor : m_tensors)
        {
            flow.addVertex(tensor);
        }

        for (const AbstractTensorMap<ScalarType>* tensorMap : m_tensorMaps)
        {
            flow.addVertex(tensorMap);

            for (const AbstractTensor<ScalarType>& tensor : *tensorMap)
            {
                flow.addEdge(tensorMap, &tensor);
                flow.addEdge(&tensor, tensorMap);
            }
        }

        for (const Module<ScalarType>* module : m_modules)
        {
            flow.addVertex(module);

            flow.addEdge(module->getInputAbstractTensorMap(), module);
            flow.addEdge(module, module->getOutputAbstractTensorMap());
        }

        flow.setVertexAsSource(m_inputMap);

        return flow;
    }

    std::vector<AbstractTensor<ScalarType>*> m_tensors;
    std::vector<AbstractTensorMap<ScalarType>*> m_tensorMaps;
    std::vector<Module<ScalarType>*> m_modules;
    NeuralNetworkBuilderHolder<ScalarType>* m_neuralNetworkBuilderHolder;
    AbstractTensorMap<ScalarType>* m_inputMap;
    AbstractTensorMap<ScalarType>* m_outputMap;
    const size_t m_thickness;
    std::optional<bool> m_canExecuteCache;
};

namespace
{
    template <typename T>
    void safeAdd(T* element, std::vector<T*>& vector)
    {
        std::vector<T*>::iterator it = std::find(vector.begin(), vector.end(), element);
        if (it != vector.end())
            return;

        vector.push_back(element);
    }

    template <typename T>
    void safeRemove(T* element, std::vector<T*>& vector)
    {
        std::vector<T*>::iterator it = std::find(vector.begin(), vector.end(), element);
        if (it == vector.end())
            return;

        vector.erase(it);
        delete element;
    }
}

template <typename ScalarType>
NeuralNetwork<ScalarType>::NeuralNetwork(size_t thickness)
    : m_pimpl(new NeuralNetworkPimpl<ScalarType>(thickness))
{}

template <typename ScalarType>
NeuralNetwork<ScalarType>::NeuralNetwork(NeuralNetwork&& other)
    : m_pimpl(other.m_pimpl)
{
    other.m_pimpl = nullptr;
}

template <typename ScalarType>
NeuralNetwork<ScalarType>::~NeuralNetwork()
{
    if (m_pimpl == nullptr)
        return;

    delete m_pimpl->m_neuralNetworkBuilderHolder;

    for (Module<ScalarType>* module : m_pimpl->m_modules)
    {
        delete module;
    }

    for (AbstractTensorMap<ScalarType>* tensorMap : m_pimpl->m_tensorMaps)
    {
        delete tensorMap;
    }

    for (AbstractTensor<ScalarType>* tensor : m_pimpl->m_tensors)
    {
        delete tensor;
    }

    delete m_pimpl;
}

template <typename ScalarType>
NeuralNetwork<ScalarType>&
NeuralNetwork<ScalarType>::operator=(NeuralNetwork<ScalarType>&& other)
{
    std::swap(m_pimpl, other.m_pimpl);
    return *this;
}

template <typename ScalarType>
AbstractTensor<ScalarType>&
NeuralNetwork<ScalarType>::addTensor(const AbstractTensorIndex& size, bool addThicknessDimension)
{
    if (!size.isValid())
    {
        throw std::runtime_error("NeuralNetwork::addTensor: The passed size is invalid.");
    }

    const size_t finalDimension = (addThicknessDimension ? 1 : 0) + size.dim(); // Thickness and dimensions required by user.

    const AbstractRtti<TensorBase>* tensorRtti = TensorBase::templateRtti().instantiate(Initializer<TypenameArgId, size_t>(std::move(TypenameArgId{TypenameArgIdOf<ScalarType>}), std::move(size_t{finalDimension})));
    if (tensorRtti == nullptr)
    {
        throw std::runtime_error("NeuralNetwork::addTensor: Got no Tensor<ScalarType, " + std::to_string(finalDimension) + "> RTTI.");
    }

    const AbstractTensorIndex* finalTensorSize;
    if (addThicknessDimension)
    {
        finalTensorSize = static_cast<AbstractTensorIndex&>(TensorIndex<1>{ m_pimpl->m_thickness }) * size;
    }
    else
    {
        const AbstractRtti<AbstractTensorIndex>* tensorIndexRtti = AbstractTensorIndex::templateRtti().instantiate(Initializer<size_t>(size.dim()));
        if (tensorIndexRtti == nullptr)
        {
            throw std::runtime_error("NeuralNetwork::addTensor: Got no TensorIndex<" + std::to_string(size.dim()) + "> RTTI.");
        }
        finalTensorSize = tensorIndexRtti->createInstance(Initializer<const AbstractTensorIndex&>(size));
    }
    AbstractTensor<ScalarType>* tensor = static_cast<AbstractTensor<ScalarType>*>(tensorRtti->createInstance(Initializer<const AbstractTensorIndex&>(*finalTensorSize)));
    delete finalTensorSize;

    safeAdd(tensor, m_pimpl->m_tensors);
    m_pimpl->m_canExecuteCache.reset();

    return *tensor;
}

template <typename ScalarType>
AbstractTensorMap<ScalarType>&
NeuralNetwork<ScalarType>::addTensorMap(AbstractTensorMap<ScalarType>* tensorMap)
{
    if (tensorMap == nullptr)
    {
        throw std::runtime_error("NeuralNetwork::addTensorMap: Input tensor map is null.");
    }

    for (const AbstractTensor<ScalarType>& tensor : *tensorMap)
    {
        if (std::find(m_pimpl->m_tensors.begin(), m_pimpl->m_tensors.end(), &tensor) == m_pimpl->m_tensors.end())
        {
            throw std::runtime_error("NeuralNetwork::addTensorMap: One of the provided tensors does not belong to the network.");
        }
    }

    safeAdd(tensorMap, m_pimpl->m_tensorMaps);

    return *tensorMap;
}

template <typename ScalarType>
Module<ScalarType>&
NeuralNetwork<ScalarType>::addModule(const AbstractRtti<Module<ScalarType>>& moduleRtti, AbstractInitializer&& modulesCtorParams, AbstractTensorMap<ScalarType>& inputMap, AbstractTensorMap<ScalarType>& outputMap, const ParamTensorFiller<ScalarType>& paramTensorFiller)
{
    if (std::find(m_pimpl->m_tensorMaps.begin(), m_pimpl->m_tensorMaps.end(), &inputMap) == m_pimpl->m_tensorMaps.end())
    {
        throw std::runtime_error("NeuralNetwork::addModule: Input tensor map does not belong to the network.");
    }

    if (std::find(m_pimpl->m_tensorMaps.begin(), m_pimpl->m_tensorMaps.end(), &outputMap) == m_pimpl->m_tensorMaps.end())
    {
        throw std::runtime_error("NeuralNetwork::addModule: Output tensor map does not belong to the network.");
    }

    Module<ScalarType>* module = moduleRtti.createInstance(std::move(modulesCtorParams));
    module->setInputTensorMap(&inputMap);
    module->setOutputTensorMap(&outputMap);
    paramTensorFiller.fill(module->getParameterAbstractTensorMap());

    safeAdd(module, m_pimpl->m_modules);
    m_pimpl->m_canExecuteCache.reset();

    return *module;
}

template <typename ScalarType>
std::map<std::string, void*>
NeuralNetwork<ScalarType>::build(const AbstractRtti<AbstractNetworkBuilder<ScalarType>>& builderRtti, AbstractInitializer&& initializer, const AbstractNetworkBuilder<ScalarType>** resultBuilder)
{
    return m_pimpl->m_neuralNetworkBuilderHolder->add(builderRtti, std::move(initializer), resultBuilder);
}

template <typename ScalarType>
void
NeuralNetwork<ScalarType>::removeTensor(const AbstractTensor<ScalarType>& tensor)
{
    const std::vector<AbstractTensor<ScalarType>*>::iterator tensorIt = std::find(m_pimpl->m_tensors.begin(), m_pimpl->m_tensors.end(), &tensor);
    if (tensorIt == m_pimpl->m_tensors.end())
    {
        throw std::runtime_error("NeuralNetwork::removeTensor: The passed tensor does not belong to the network.");
    }

    for (AbstractTensorMap<ScalarType>* tensorMap : m_pimpl->m_tensorMaps)
    {
        for (const AbstractTensor<ScalarType>& iteratedTensor : *tensorMap)
        {
            if (&iteratedTensor == &tensor)
                throw std::runtime_error("NeuralNetwork::removeTensor: The passed tensor is used by a tensor map.");
        }
    }

    delete &tensor;

    m_pimpl->m_tensors.erase(tensorIt);
}

template <typename ScalarType>
void
NeuralNetwork<ScalarType>::removeTensorMap(const AbstractTensorMap<ScalarType>& tensorMap)
{
    const std::vector<AbstractTensorMap<ScalarType>*>::iterator tensorMapIt = std::find(m_pimpl->m_tensorMaps.begin(), m_pimpl->m_tensorMaps.end(), &tensorMap);
    if (tensorMapIt == m_pimpl->m_tensorMaps.end())
    {
        throw std::runtime_error("NeuralNetwork::removeTensorMap: The passed tensor map does not belong to the network.");
    }

    if (&tensorMap == getInput())
        throw std::runtime_error("NeuralNetwork::removeTensorMap: The passed tensor map is used as input of the network.");
    if (&tensorMap == getOutput())
        throw std::runtime_error("NeuralNetwork::removeTensorMap: The passed tensor map is used as output of the network.");

    for (const Module<ScalarType>* module : m_pimpl->m_modules)
    {
        if (&tensorMap == module->getInputAbstractTensorMap())
            throw std::runtime_error("NeuralNetwork::removeTensorMap: The passed tensor map is used as input of a module.");
        if (&tensorMap == module->getOutputAbstractTensorMap())
            throw std::runtime_error("NeuralNetwork::removeTensorMap: The passed tensor map is used as output of a module.");
    }

    delete &tensorMap;

    m_pimpl->m_tensorMaps.erase(tensorMapIt);
}

template <typename ScalarType>
void
NeuralNetwork<ScalarType>::removeModule(const Module<ScalarType>& module)
{
    const std::vector<Module<ScalarType>*>::iterator moduleIt = std::find(m_pimpl->m_modules.begin(), m_pimpl->m_modules.end(), &module);
    if (moduleIt == m_pimpl->m_modules.end())
    {
        throw std::runtime_error("NeuralNetwork::removeModule: The passed module does not belong to the network.");
    }

    delete &module;

    m_pimpl->m_modules.erase(moduleIt);
}

template <typename ScalarType>
void
NeuralNetwork<ScalarType>::unbuild(const AbstractNetworkBuilder<ScalarType>& builder)
{
    m_pimpl->m_neuralNetworkBuilderHolder->remove(builder);
}

template <typename ScalarType>
void
NeuralNetwork<ScalarType>::setInput(AbstractTensorMap<ScalarType>* map)
{
    if (map && std::find(m_pimpl->m_tensorMaps.begin(), m_pimpl->m_tensorMaps.end(), map) == m_pimpl->m_tensorMaps.end())
    {
        throw std::runtime_error("NeuralNetwork::setInput: The provided tensor map does not belong to the network.");
    }

    m_pimpl->m_inputMap = map;
    m_pimpl->m_canExecuteCache.reset();
}

template <typename ScalarType>
void
NeuralNetwork<ScalarType>::setOutput(AbstractTensorMap<ScalarType>* map)
{
    if (map && std::find(m_pimpl->m_tensorMaps.begin(), m_pimpl->m_tensorMaps.end(), map) == m_pimpl->m_tensorMaps.end())
    {
        throw std::runtime_error("NeuralNetwork::setOutput: The provided tensor map does not belong to the network.");
    }

    m_pimpl->m_outputMap = map;
    m_pimpl->m_canExecuteCache.reset();
}

template <typename ScalarType>
const std::vector<AbstractTensor<ScalarType>*>&
NeuralNetwork<ScalarType>::getTensors() const
{
    return m_pimpl->m_tensors;
}

template <typename ScalarType>
const std::vector<AbstractTensorMap<ScalarType>*>&
NeuralNetwork<ScalarType>::getTensorMaps() const
{
    return m_pimpl->m_tensorMaps;
}

template <typename ScalarType>
const std::vector<Module<ScalarType>*>&
NeuralNetwork<ScalarType>::getModules() const
{
    return m_pimpl->m_modules;
}

template <typename ScalarType>
const AbstractTensorMap<ScalarType>*
NeuralNetwork<ScalarType>::getInput() const
{
    return m_pimpl->m_inputMap;
}

template <typename ScalarType>
AbstractTensorMap<ScalarType>*
NeuralNetwork<ScalarType>::getInput()
{
    return const_cast<AbstractTensorMap<ScalarType>*>(static_cast<const NeuralNetwork<ScalarType>*>(this)->getInput());
}

template <typename ScalarType>
const AbstractTensorMap<ScalarType>*
NeuralNetwork<ScalarType>::getOutput() const
{
    return m_pimpl->m_outputMap;
}

template <typename ScalarType>
AbstractTensorMap<ScalarType>*
NeuralNetwork<ScalarType>::getOutput()
{
    return const_cast<AbstractTensorMap<ScalarType>*>(static_cast<const NeuralNetwork<ScalarType>*>(this)->getOutput());
}

template <typename ScalarType>
bool
NeuralNetwork<ScalarType>::canExecute() const
{
    if (m_pimpl->m_canExecuteCache.has_value())
    {
        return m_pimpl->m_canExecuteCache.value();
    }

    bool result = true;

    if (result)
        result &= (m_pimpl->m_inputMap != nullptr);

    if (result)
        result &= (m_pimpl->m_outputMap != nullptr);

    std::map<const void*, size_t> reordering;

    if (result)
        result &= m_pimpl->toFlow().flow(&reordering);

    if (result)
        std::sort(
            m_pimpl->m_modules.begin(),
            m_pimpl->m_modules.end(),
            [&reordering](const Module<ScalarType>* first, const Module<ScalarType>* second) {
                return reordering[first] < reordering[second];
            }
        );

    m_pimpl->m_canExecuteCache = result;
    return result;
}

template <typename ScalarType>
void
NeuralNetwork<ScalarType>::execute(InputProvider<ScalarType>& inputProvider, OutputReceiver<ScalarType>& outputReceiver) const
{
    if (!canExecute())
    {
        throw std::runtime_error("NeuralNetwork::execute: The network cannot yet be executed.");
    }

    if (!inputProvider.hasMoreThan(m_pimpl->m_thickness))
    {
        throw std::runtime_error("NeuralNetwork::execute: There must be at least " + std::to_string(m_pimpl->m_thickness) + " inputs in the input provider.");
    }

    inputProvider.getNewInput(*m_pimpl->m_inputMap);

    for (Module<ScalarType>* module : m_pimpl->m_modules)
    {
        module->compute();
    }

    outputReceiver.sendOutput(*m_pimpl->m_outputMap);
}

template <typename ScalarType>
void
NeuralNetwork<ScalarType>::executeAll(InputProvider<ScalarType>& inputProvider, OutputReceiver<ScalarType>& outputReceiver) const
{
    while (inputProvider.hasMoreThan(m_pimpl->m_thickness))
    {
        execute(inputProvider, outputReceiver);
    }
}

template <typename ScalarType>
size_t
NeuralNetwork<ScalarType>::thickness() const
{
    return m_pimpl->m_thickness;
}
