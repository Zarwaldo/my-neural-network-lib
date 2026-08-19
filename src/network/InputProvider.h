#pragma once

#include <rtti/Rtti.h>

template <typename ScalarType>
class AbstractTensorMap;

template <typename ScalarType>
class InputProvider
{
public:
    virtual ~InputProvider() = default;

    virtual void getNewInput(AbstractTensorMap<ScalarType>& inputMap) = 0;

    virtual bool hasMoreThan(size_t nbInputs) const = 0;

    DECLARE_AS_RTTI_BASE_TYPE(InputProvider<ScalarType>)
};
