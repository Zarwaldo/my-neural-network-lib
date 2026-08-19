#pragma once

#include <rtti/Rtti.h>

template <typename ScalarType>
class AbstractTensorMap;

template <typename ScalarType>
class OutputReceiver
{
public:
    virtual ~OutputReceiver() = default;

    virtual void sendOutput(const AbstractTensorMap<ScalarType>& outputMap) = 0;

    DECLARE_AS_RTTI_BASE_TYPE(OutputReceiver<ScalarType>)
};
