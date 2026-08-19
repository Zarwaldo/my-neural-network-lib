#pragma once

#include <module/Module.h>

#include <helpers/BuildTimeList.h>
#include <helpers/RawTuple.h>

template <typename ScalarType>
class AbstractTensorMap;

template <typename ScalarType, typename KeyEnum>
class TensorMap;

template <typename ScalarType, size_t Dimension>
class Tensor;

template <size_t Dimension>
class TensorIndex;

#define ABSTRACT_MODULE_TEMPLATE_PARAMS     \
    typename ScalarType,                    \
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
    typename ScalarType,                      \
    typename InputKeyEnum,                    \
    typename ParameterKeyEnum,                \
    typename OutputKeyEnum,                   \
    size_t... InputTensorDimensions,          \
    size_t... ParameterTensorDimensions,      \
    size_t... OutputTensorDimensions

#define ABSTRACT_MODULE_SPECIALIZATION_ARGS                   \
    ScalarType,                                               \
    InputKeyEnum,                                             \
    ParameterKeyEnum,                                         \
    OutputKeyEnum,                                            \
    BuildTimeList::IntegerList<InputTensorDimensions...>,     \
    BuildTimeList::IntegerList<ParameterTensorDimensions...>, \
    BuildTimeList::IntegerList<OutputTensorDimensions...>

template <ABSTRACT_MODULE_SPECIALIZATION_PARAMS>
class AbstractModule<ABSTRACT_MODULE_SPECIALIZATION_ARGS> : public Module<ScalarType>
{
public:
    using InputTensorMapType = TensorMap<ScalarType, InputKeyEnum>;
    using ParameterTensorMapType = TensorMap<ScalarType, ParameterKeyEnum>;
    using OutputTensorMapType = TensorMap<ScalarType, OutputKeyEnum>;

    HOST AbstractModule(const RawTuple<const TensorIndex<ParameterTensorDimensions>&...>& parameterTensorsSizes);
    HOST AbstractModule(const AbstractModule& other) = delete;
    HOST AbstractModule(AbstractModule&& other);

    HOST virtual ~AbstractModule();

    HOST AbstractModule& operator=(const AbstractModule& other) = delete;
    HOST AbstractModule& operator=(AbstractModule&& other);

    HOST virtual AbstractTensorMap<ScalarType>* getInputAbstractTensorMap() const override;
    HOST virtual InputTensorMapType* getInputTensorMap() const;

    HOST virtual const AbstractTensorMap<ScalarType>& getParameterAbstractTensorMap() const override;
    HOST virtual const ParameterTensorMapType& getParameterTensorMap() const;
    HOST virtual AbstractTensorMap<ScalarType>& getParameterAbstractTensorMap() override;
    HOST virtual ParameterTensorMapType& getParameterTensorMap();

    HOST virtual AbstractTensorMap<ScalarType>* getOutputAbstractTensorMap() const override;
    HOST virtual OutputTensorMapType* getOutputTensorMap() const;

    HOST virtual void setInputTensorMap(AbstractTensorMap<ScalarType>* map) override;
    HOST virtual void setOutputTensorMap(AbstractTensorMap<ScalarType>* map) override;

    HOST virtual RawTuple<const TensorIndex<InputTensorDimensions>&...> getInputTensorSizes() const;
    HOST virtual RawTuple<const TensorIndex<ParameterTensorDimensions>&...> getParameterTensorSizes() const;
    HOST virtual RawTuple<const TensorIndex<OutputTensorDimensions>&...> getOutputTensorSizes() const;

    HOST virtual void compute() const = 0;

    HOST virtual void backpropagate(AbstractTensorMap<ScalarType>& costPartDerivWRTInput, AbstractTensorMap<ScalarType>& costPartDerivWRTParameter, const AbstractTensorMap<ScalarType>& costPartDerivWRTOutput) const = 0;

protected:
    HOST virtual void* getComputationMemory() const;

    HOST virtual void* getInputBackpropagationMemory() const;

    HOST virtual void* getParameterBackpropagationMemory() const;

private:
    AbstractModulePimpl<ABSTRACT_MODULE_SPECIALIZATION_ARGS>* m_p;
};
