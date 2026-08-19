#pragma once

#include <CommonMacros.h>

#include <rtti/Rtti.h>

template <typename ScalarType>
class AbstractTensorMap;

template <typename ScalarType>
class Module
{
public:
    HOST virtual ~Module();

    HOST virtual AbstractTensorMap<ScalarType>* getInputAbstractTensorMap() const = 0;

    HOST virtual const AbstractTensorMap<ScalarType>& getParameterAbstractTensorMap() const = 0;
    HOST virtual AbstractTensorMap<ScalarType>& getParameterAbstractTensorMap() = 0;

    HOST virtual AbstractTensorMap<ScalarType>* getOutputAbstractTensorMap() const = 0;

    HOST virtual void setInputTensorMap(AbstractTensorMap<ScalarType>* map) = 0;
    HOST virtual void setOutputTensorMap(AbstractTensorMap<ScalarType>* map) = 0;

    HOST virtual void compute() const = 0;

    HOST virtual void backpropagate(AbstractTensorMap<ScalarType>& costPartDerivWRTInput, AbstractTensorMap<ScalarType>& costPartDerivWRTParameter, const AbstractTensorMap<ScalarType>& costPartDerivWRTOutput) const = 0;

    DECLARE_AS_RTTI_BASE_TYPE(Module<ScalarType>)
};
