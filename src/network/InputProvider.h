#pragma once

#include <rtti/Rtti.h>

template <typename ValueType>
class AbstractTensorMap;

template <typename ValueType>
class InputProvider
{
public:
    virtual ~InputProvider() = default;

    virtual void getNewInput(AbstractTensorMap<ValueType>& inputMap) = 0;

    virtual bool hasMoreThan(size_t nbInputs) const = 0;

    DECLARE_AS_RTTI_BASE_TYPE(InputProvider<ValueType>)
};
