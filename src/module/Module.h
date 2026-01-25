#pragma once

#include <CommonMacros.h>

#include <rtti/Rtti.h>

template <typename ValueType>
class AbstractTensorMap;

template <typename ValueType>
class Module
{
public:
    HOST virtual ~Module();

    HOST virtual AbstractTensorMap<ValueType>* getInputAbstractTensorMap() const = 0;

    HOST virtual const AbstractTensorMap<ValueType>& getParameterAbstractTensorMap() const = 0;
    HOST virtual AbstractTensorMap<ValueType>& getParameterAbstractTensorMap() = 0;

    HOST virtual AbstractTensorMap<ValueType>* getOutputAbstractTensorMap() const = 0;

    HOST virtual void setInputTensorMap(AbstractTensorMap<ValueType>* map) = 0;
    HOST virtual void setOutputTensorMap(AbstractTensorMap<ValueType>* map) = 0;

    HOST virtual void compute() const = 0;

    HOST virtual void backpropagate(AbstractTensorMap<ValueType>& costPartDerivWRTInput, AbstractTensorMap<ValueType>& costPartDerivWRTParameter, const AbstractTensorMap<ValueType>& costPartDerivWRTOutput) const = 0;

    DECLARE_AS_RTTI_BASE_TYPE(Module<ValueType>)
};
