#pragma once

#include <rtti/Rtti.h>

template <typename ValueType>
class OutputReceiver
{
public:
    virtual ~OutputReceiver() = default;

    virtual void sendOutput(const AbstractTensorMap<ValueType>& outputMap) = 0;

    DECLARE_AS_RTTI_BASE_TYPE(OutputReceiver<ValueType>)
};
