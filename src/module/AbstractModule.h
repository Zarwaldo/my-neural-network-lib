#pragma once

#include <module/Module.h>

#include <helpers/BuildTimeList.h>
#include <helpers/RawTuple.h>

template <typename ValueType>
class AbstractTensorMap;

template <typename ValueType, typename KeyEnum>
class TensorMap;

template <typename ValueType, size_t Dimension>
class Tensor;

template <size_t Dimension>
class TensorIndex;

#define ABSTRACT_MODULE_TEMPLATE_PARAMS     \
    typename ValueType,                     \
    typename InputKeyEnum,                  \
    typename ParameterKeyEnum,              \
    typename OutputKeyEnum,                 \
    typename InputTensorDimensionsList,     \
    typename ParameterTensorDimensionsList, \
    typename OutputTensorDimensionsList

template <ABSTRACT_MODULE_TEMPLATE_PARAMS>
class AbstractModulePimpl;

template <ABSTRACT_MODULE_TEMPLATE_PARAMS>
class AbstractModule
{};

#define ABSTRACT_MODULE_SPECIALIZATION_PARAMS \
    typename ValueType,                       \
    typename InputKeyEnum,                    \
    typename ParameterKeyEnum,                \
    typename OutputKeyEnum,                   \
    size_t... InputTensorDimensions,          \
    size_t... ParameterTensorDimensions,      \
    size_t... OutputTensorDimensions

#define ABSTRACT_MODULE_SPECIALIZATION_ARGS                   \
    ValueType,                                                \
    InputKeyEnum,                                             \
    ParameterKeyEnum,                                         \
    OutputKeyEnum,                                            \
    BuildTimeList::IntegerList<InputTensorDimensions...>,     \
    BuildTimeList::IntegerList<ParameterTensorDimensions...>, \
    BuildTimeList::IntegerList<OutputTensorDimensions...>

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
class AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS> : public Module<ValueType>
{
public:
    using InputTensorMapType = TensorMap<ValueType, InputKeyEnum>;
    using ParameterTensorMapType = TensorMap<ValueType, ParameterKeyEnum>;
    using OutputTensorMapType = TensorMap<ValueType, OutputKeyEnum>;

    HOST AbstractModule(const RawTuple<const TensorIndex<ParameterTensorDimensions>&...>& parameterTensorsSizes);
    HOST virtual ~AbstractModule();

    HOST virtual AbstractTensorMap<ValueType>* getInputAbstractTensorMap() const override;
    HOST virtual InputTensorMapType* getInputTensorMap() const;

    HOST virtual const AbstractTensorMap<ValueType>& getParameterAbstractTensorMap() const override;
    HOST virtual const ParameterTensorMapType& getParameterTensorMap() const;
    HOST virtual AbstractTensorMap<ValueType>& getParameterAbstractTensorMap() override;
    HOST virtual ParameterTensorMapType& getParameterTensorMap();

    HOST virtual AbstractTensorMap<ValueType>* getOutputAbstractTensorMap() const override;
    HOST virtual OutputTensorMapType* getOutputTensorMap() const;

    HOST virtual void setInputTensorMap(AbstractTensorMap<ValueType>* map) override;
    HOST virtual void setOutputTensorMap(AbstractTensorMap<ValueType>* map) override;

    HOST virtual RawTuple<const TensorIndex<InputTensorDimensions>&...> getInputTensorSizes() const;
    HOST virtual RawTuple<const TensorIndex<ParameterTensorDimensions>&...> getParameterTensorSizes() const;
    HOST virtual RawTuple<const TensorIndex<OutputTensorDimensions>&...> getOutputTensorSizes() const;

    HOST virtual void compute() const = 0;

    HOST virtual void backpropagate(AbstractTensorMap<ValueType>& costPartDerivWRTInput, AbstractTensorMap<ValueType>& costPartDerivWRTParameter, const AbstractTensorMap<ValueType>& costPartDerivWRTOutput) const = 0;

protected:
    HOST virtual void* getComputationMemory() const;

    HOST virtual void* getInputBackpropagationMemory() const;

    HOST virtual void* getParameterBackpropagationMemory() const;

private:
    AbstractModulePimpl<ABSTRACT_MODULE_SPECIALIZATION_ARGS>* m_p;
};
