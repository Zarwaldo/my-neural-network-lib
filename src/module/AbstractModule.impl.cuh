#pragma once

#include <module/AbstractModule.h>

#include <CommonMacros.h>

#include <tensor/TensorMap.h>
#include <tensor/Tensor.h>

template <typename ValueType>
struct AllocateTensorsFromSizes
{
    template <typename TensorIndexType>
    struct MapFunction
    {};

    template <size_t Dimension>
    struct MapFunction<TensorIndex<Dimension>>
    {
        static Tensor<ValueType, Dimension>* doIt(const TensorIndex<Dimension>& sizes)
        {
            return Tensor<ValueType, Dimension>::create(sizes);
        }
    };
};

template <ABSTRACT_MODULE_TEMPLATE_PARAMS>
class AbstractModulePimpl
{};

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
class AbstractModulePimpl<ABSTRACT_MODULE_SPECIALIZATION_ARGS>
{
public:
    static constexpr size_t NbInputTensors = sizeof...(InputTensorDimensions);
    static_assert((NbInputTensors == InputKeyEnum::NbValues), "AbstractModule: Wrong number of input dimensions");
    static constexpr size_t NbParameterTensors = sizeof...(ParameterTensorDimensions);
    static_assert((NbParameterTensors == ParameterKeyEnum::NbValues), "AbstractModule: Wrong number of parameter dimensions");
    static constexpr size_t NbOutputTensors = sizeof...(OutputTensorDimensions);
    static_assert((NbOutputTensors == OutputKeyEnum::NbValues), "AbstractModule: Wrong number of output dimensions");

    using InputTensorMapType = TensorMap<ValueType, InputKeyEnum>;
    using ParameterTensorMapType = TensorMap<ValueType, ParameterKeyEnum>;
    using OutputTensorMapType = TensorMap<ValueType, OutputKeyEnum>;

    HOST AbstractModulePimpl(const RawTuple<const TensorIndex<ParameterTensorDimensions>&...>& parameterTensorsSizes)
        : m_parameterTensors(parameterTensorsSizes.template hostMap<AllocateTensorsFromSizes<ValueType>::MapFunction>())
        , m_inputMap(nullptr)
        , m_parameterMap(
            m_parameterTensors.hostApply([](Tensor<ValueType, ParameterTensorDimensions>*&... tensors) {
                return ParameterTensorMapType{ tensors... };
            })
        )
        , m_outputMap(nullptr)
        , m_managedMemory(nullptr)
        , m_computationMemory(nullptr)
        , m_inputBackpropagationMemory(nullptr)
        , m_parameterBackpropagationMemory(nullptr)
    {
        static constexpr size_t sizeofInputRawTensors = (0 + ... + sizeof(RawTensor<ValueType, InputTensorDimensions>));
        static constexpr size_t sizeofParameterRawTensors = (0 + ... + sizeof(RawTensor<ValueType, ParameterTensorDimensions>));
        static constexpr size_t sizeofOutputRawTensors = (0 + ... + sizeof(RawTensor<ValueType, OutputTensorDimensions>));

        cudaMallocManaged(&m_managedMemory, (
            // Memory space used for computation
            sizeofInputRawTensors
            + sizeofParameterRawTensors
            + sizeofOutputRawTensors

            // Memory space used for input backpropagation
            + sizeofInputRawTensors
            + sizeofParameterRawTensors
            + sizeofOutputRawTensors
            + sizeofOutputRawTensors
            + sizeofInputRawTensors

            // Memory space used for parameter backpropagation
            + sizeofInputRawTensors
            + sizeofParameterRawTensors
            + sizeofOutputRawTensors
            + sizeofOutputRawTensors
            + sizeofParameterRawTensors
        ));

        m_computationMemory = m_managedMemory;
        m_inputBackpropagationMemory = static_cast<char*>(m_computationMemory)
            + sizeofInputRawTensors
            + sizeofParameterRawTensors
            + sizeofOutputRawTensors;
        m_parameterBackpropagationMemory = static_cast<char*>(m_inputBackpropagationMemory)
            + sizeofInputRawTensors
            + sizeofParameterRawTensors
            + sizeofOutputRawTensors
            + sizeofOutputRawTensors
            + sizeofInputRawTensors;
    }

    HOST ~AbstractModulePimpl()
    {
        cudaFree(m_managedMemory);

        m_parameterTensors.hostApply([](Tensor<ValueType, ParameterTensorDimensions>*&... tensors) {
            (delete tensors, ...);
        });
    }

    RawTuple<Tensor<ValueType, ParameterTensorDimensions>*...> m_parameterTensors;

    InputTensorMapType* m_inputMap;
    ParameterTensorMapType m_parameterMap;
    OutputTensorMapType* m_outputMap;

    void* m_managedMemory;
    void* m_computationMemory;
    void* m_inputBackpropagationMemory;
    void* m_parameterBackpropagationMemory;
};

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::AbstractModule(const RawTuple<const TensorIndex<ParameterTensorDimensions>&...>& parameterTensorsSizes)
    : m_p(new AbstractModulePimpl<ABSTRACT_MODULE_SPECIALIZATION_ARGS>(parameterTensorsSizes))
{}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::AbstractModule(AbstractModule&& other)
    : m_p(other.m_p)
{
    other.m_p = nullptr;
}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::~AbstractModule()
{
    delete m_p;
}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>&
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::operator=(AbstractModule&& other)
{
    std::swap(m_p, other.m_p);
    return *this;
}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
AbstractTensorMap<ValueType>*
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::getInputAbstractTensorMap() const
{
    return dynamic_cast<AbstractTensorMap<ValueType>*>(getInputTensorMap());
}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
TensorMap<ValueType, InputKeyEnum>*
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::getInputTensorMap() const
{
    return m_p->m_inputMap;
}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
const AbstractTensorMap<ValueType>&
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::getParameterAbstractTensorMap() const
{
    return dynamic_cast<const AbstractTensorMap<ValueType>&>(getParameterTensorMap());
}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
const TensorMap<ValueType, ParameterKeyEnum>&
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::getParameterTensorMap() const
{
    return m_p->m_parameterMap;
}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
AbstractTensorMap<ValueType>&
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::getParameterAbstractTensorMap()
{
    return dynamic_cast<AbstractTensorMap<ValueType>&>(getParameterTensorMap());
}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
TensorMap<ValueType, ParameterKeyEnum>&
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::getParameterTensorMap()
{
    return const_cast<TensorMap<ValueType, ParameterKeyEnum>&>(static_cast<const AbstractModule*>(this)->getParameterTensorMap());
}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
AbstractTensorMap<ValueType>*
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::getOutputAbstractTensorMap() const
{
    return dynamic_cast<AbstractTensorMap<ValueType>*>(getOutputTensorMap());
}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
TensorMap<ValueType, OutputKeyEnum>*
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::getOutputTensorMap() const
{
    return m_p->m_outputMap;
}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
void
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::setInputTensorMap(AbstractTensorMap<ValueType>* map)
{
    m_p->m_inputMap = dynamic_cast<TensorMap<ValueType, InputKeyEnum>*>(map);
}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
void
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::setOutputTensorMap(AbstractTensorMap<ValueType>* map)
{
    m_p->m_outputMap = dynamic_cast<TensorMap<ValueType, OutputKeyEnum>*>(map);
}

template <typename KeyEnum, size_t Dimension>
using GetKeyEnum = KeyEnum;

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
RawTuple<const TensorIndex<InputTensorDimensions>&...>
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::getInputTensorSizes() const
{
    const TensorMap<ValueType, InputKeyEnum>* inputMap = getInputTensorMap();
    if (inputMap == nullptr)
    {
        throw std::runtime_error("AbstractModule: input map not set");
    }

    return InputKeyEnum::getValuesTuple().hostApply([inputMap](GetKeyEnum<InputKeyEnum, InputTensorDimensions>... keys) {
        return makeRawTuple<const TensorIndex<InputTensorDimensions>&...>(inputMap->get(keys).sizes()...);
    });
}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
RawTuple<const TensorIndex<ParameterTensorDimensions>&...>
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::getParameterTensorSizes() const
{
    const TensorMap<ValueType, ParameterKeyEnum>& parameterMap = getParameterTensorMap();

    return ParameterKeyEnum::getValuesTuple().hostApply([&parameterMap](GetKeyEnum<ParameterKeyEnum, ParameterTensorDimensions>... keys) {
        return makeRawTuple<const TensorIndex<ParameterTensorDimensions>&...>(parameterMap.get(keys).sizes()...);
    });
}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
RawTuple<const TensorIndex<OutputTensorDimensions>&...>
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::getOutputTensorSizes() const
{
    const TensorMap<ValueType, OutputKeyEnum>* outputMap = getOutputTensorMap();
    if (outputMap == nullptr)
    {
        throw std::runtime_error("AbstractModule: output map not set");
    }

    return OutputKeyEnum::getValuesTuple().hostApply([outputMap](GetKeyEnum<OutputKeyEnum, OutputTensorDimensions>... keys) {
        return makeRawTuple<const TensorIndex<OutputTensorDimensions>&...>(outputMap->get(keys).sizes()...);
    });
}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
void*
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::getComputationMemory() const
{
    return m_p->m_computationMemory;
}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
void*
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::getInputBackpropagationMemory() const
{
    return m_p->m_inputBackpropagationMemory;
}

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
HOST
void*
AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS>::getParameterBackpropagationMemory() const
{
    return m_p->m_parameterBackpropagationMemory;
}
