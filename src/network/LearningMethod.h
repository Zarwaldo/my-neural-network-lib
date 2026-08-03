#pragma once

#include <rtti/Rtti.h>

template <typename ValueType>
class AbstractTensor;

template <typename ValueType>
class LearningMethod
{
public:
    virtual ~LearningMethod() = default;

    virtual void learn(AbstractTensor<ValueType>& parameterTensor, const AbstractTensor<ValueType>& costPartDerivWRTParamTensor) const = 0;

    DECLARE_AS_RTTI_BASE_TYPE(LearningMethod<ValueType>)
};
