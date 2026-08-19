#pragma once

#include <rtti/Rtti.h>

template <typename ScalarType>
class AbstractTensor;

template <typename ScalarType>
class LearningMethod
{
public:
    virtual ~LearningMethod() = default;

    virtual void learn(AbstractTensor<ScalarType>& parameterTensor, const AbstractTensor<ScalarType>& costPartDerivWRTParamTensor) const = 0;

    DECLARE_AS_RTTI_BASE_TYPE(LearningMethod<ScalarType>)
};
