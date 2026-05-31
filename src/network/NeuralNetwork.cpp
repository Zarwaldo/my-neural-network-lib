#include <network/NeuralNetwork.h>

#include <network/api.h>

#include <module/Module.h>

#include <network/Flow.h>
#include <network/InputProvider.h>
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

template <typename ValueType>
class NeuralNetworkPimpl
{
public:
    NeuralNetworkPimpl(size_t thickness)
        : m_tensors()
        , m_tensorMaps()
        , m_modules()
        , m_inputMap(nullptr)
        , m_outputMap(nullptr)
        , m_thickness(thickness)
        , m_canExecuteCache()
    {}

    const Flow toFlow() const
    {
        Flow flow;

        for (const AbstractTensor<ValueType>* tensor : m_tensors)
        {
            flow.addVertex(tensor);
        }

        for (const AbstractTensorMap<ValueType>* tensorMap : m_tensorMaps)
        {
            flow.addVertex(tensorMap);

            for (const AbstractTensor<ValueType>& tensor : *tensorMap)
            {
                flow.addEdge(tensorMap, &tensor);
                flow.addEdge(&tensor, tensorMap);
            }
        }

        for (const Module<ValueType>* module : m_modules)
        {
            flow.addVertex(module);

            flow.addEdge(module->getInputAbstractTensorMap(), module);
            flow.addEdge(module, module->getOutputAbstractTensorMap());
        }

        flow.setVertexAsSource(m_inputMap);

        return flow;
    }

    std::vector<AbstractTensor<ValueType>*> m_tensors;
    std::vector<AbstractTensorMap<ValueType>*> m_tensorMaps;
    std::vector<Module<ValueType>*> m_modules;
    AbstractTensorMap<ValueType>* m_inputMap;
    AbstractTensorMap<ValueType>* m_outputMap;
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

template <typename ValueType>
NeuralNetwork<ValueType>::NeuralNetwork(size_t thickness)
    : m_pimpl(new NeuralNetworkPimpl<ValueType>(thickness))
{}

template <typename ValueType>
NeuralNetwork<ValueType>::NeuralNetwork(NeuralNetwork&& other)
    : m_pimpl(other.m_pimpl)
{
    other.m_pimpl = nullptr;
}

template <typename ValueType>
NeuralNetwork<ValueType>::~NeuralNetwork()
{
    if (m_pimpl == nullptr)
        return;

    for (Module<ValueType>* module : m_pimpl->m_modules)
    {
        delete module;
    }

    for (AbstractTensorMap<ValueType>* tensorMap : m_pimpl->m_tensorMaps)
    {
        delete tensorMap;
    }

    for (AbstractTensor<ValueType>* tensor : m_pimpl->m_tensors)
    {
        delete tensor;
    }

    delete m_pimpl;
}

template <typename ValueType>
NeuralNetwork<ValueType>&
NeuralNetwork<ValueType>::operator=(NeuralNetwork<ValueType>&& other)
{
    std::swap(m_pimpl, other.m_pimpl);
    return *this;
}

template <typename ValueType>
AbstractTensor<ValueType>&
NeuralNetwork<ValueType>::addTensor(const AbstractTensorIndex& size)
{
    const size_t finalDimension = 1 + size.dim(); // Thickness and dimensions required by user.

    const AbstractRtti<TensorBase>* tensorRtti = TensorBase::templateRtti().instantiate(Initializer<TypenameArgId, size_t>(std::move(TypenameArgId{TypenameArgIdOf<ValueType>}), std::move(size_t{finalDimension})));
    if (tensorRtti == nullptr)
    {
        throw std::runtime_error("NeuralNetwork::addTensor: Got no Tensor<ValueType, " + std::to_string(finalDimension) + "> RTTI.");
    }

    AbstractTensorIndex* finalTensorSize = static_cast<AbstractTensorIndex&>(TensorIndex<1>{ m_pimpl->m_thickness }) * size;
    AbstractTensor<ValueType>* tensor = static_cast<AbstractTensor<ValueType>*>(tensorRtti->createInstance(Initializer<const AbstractTensorIndex&>(*finalTensorSize)));
    delete finalTensorSize;

    safeAdd(tensor, m_pimpl->m_tensors);
    m_pimpl->m_canExecuteCache.reset();

    return *tensor;
}

template <typename ValueType>
AbstractTensorMap<ValueType>&
NeuralNetwork<ValueType>::addTensorMap(AbstractTensorMap<ValueType>* tensorMap)
{
    if (tensorMap == nullptr)
    {
        throw std::runtime_error("NeuralNetwork::addTensorMap: Input tensor map is null.");
    }

    for (const AbstractTensor<ValueType>& tensor : *tensorMap)
    {
        if (std::find(m_pimpl->m_tensors.begin(), m_pimpl->m_tensors.end(), &tensor) == m_pimpl->m_tensors.end())
        {
            throw std::runtime_error("NeuralNetwork::addTensorMap: One of the provided tensors does not belong to the network.");
        }
    }

    safeAdd(tensorMap, m_pimpl->m_tensorMaps);

    return *tensorMap;
}

template <typename ValueType>
Module<ValueType>&
NeuralNetwork<ValueType>::addModule(const AbstractRtti<Module<ValueType>>& moduleRtti, AbstractInitializer&& modulesCtorParams, AbstractTensorMap<ValueType>& inputMap, AbstractTensorMap<ValueType>& outputMap, const ParamTensorFiller<ValueType>& paramTensorFiller)
{
    if (std::find(m_pimpl->m_tensorMaps.begin(), m_pimpl->m_tensorMaps.end(), &inputMap) == m_pimpl->m_tensorMaps.end())
    {
        throw std::runtime_error("NeuralNetwork::addModule: Input tensor map does not belong to the network.");
    }

    if (std::find(m_pimpl->m_tensorMaps.begin(), m_pimpl->m_tensorMaps.end(), &outputMap) == m_pimpl->m_tensorMaps.end())
    {
        throw std::runtime_error("NeuralNetwork::addModule: Output tensor map does not belong to the network.");
    }

    Module<ValueType>* module = moduleRtti.createInstance(std::move(modulesCtorParams));
    module->setInputTensorMap(&inputMap);
    module->setOutputTensorMap(&outputMap);
    paramTensorFiller.fill(module->getParameterAbstractTensorMap());

    safeAdd(module, m_pimpl->m_modules);
    m_pimpl->m_canExecuteCache.reset();

    return *module;
}

template <typename ValueType>
void
NeuralNetwork<ValueType>::setInput(AbstractTensorMap<ValueType>& map)
{
    if (std::find(m_pimpl->m_tensorMaps.begin(), m_pimpl->m_tensorMaps.end(), &map) == m_pimpl->m_tensorMaps.end())
    {
        throw std::runtime_error("NeuralNetwork::setInput: The provided tensor map does not belong to the network.");
    }

    m_pimpl->m_inputMap = &map;
    m_pimpl->m_canExecuteCache.reset();
}

template <typename ValueType>
void
NeuralNetwork<ValueType>::setOutput(AbstractTensorMap<ValueType>& map)
{
    if (std::find(m_pimpl->m_tensorMaps.begin(), m_pimpl->m_tensorMaps.end(), &map) == m_pimpl->m_tensorMaps.end())
    {
        throw std::runtime_error("NeuralNetwork::setOutput: The provided tensor map does not belong to the network.");
    }

    m_pimpl->m_outputMap = &map;
    m_pimpl->m_canExecuteCache.reset();
}

template <typename ValueType>
bool
NeuralNetwork<ValueType>::canExecute() const
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
            [&reordering](const Module<ValueType>* first, const Module<ValueType>* second) {
                return reordering[first] < reordering[second];
            }
        );

    m_pimpl->m_canExecuteCache = result;
    return result;
}

template <typename ValueType>
void
NeuralNetwork<ValueType>::execute(InputProvider<ValueType>& inputProvider, OutputReceiver<ValueType>& outputReceiver) const
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

    for (Module<ValueType>* module : m_pimpl->m_modules)
    {
        module->compute();
    }

    outputReceiver.sendOutput(*m_pimpl->m_outputMap);
}

template <typename ValueType>
void
NeuralNetwork<ValueType>::executeAll(InputProvider<ValueType>& inputProvider, OutputReceiver<ValueType>& outputReceiver) const
{
    while (inputProvider.hasMoreThan(m_pimpl->m_thickness))
    {
        execute(inputProvider, outputReceiver);
    }
}

template class MY_NEURAL_NETWORK_LIB__NETWORK__API NeuralNetwork<float>;
template class MY_NEURAL_NETWORK_LIB__NETWORK__API NeuralNetwork<double>;
