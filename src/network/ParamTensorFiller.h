#pragma once

#include <rtti/Rtti.h>

template <typename ValueType>
class AbstractTensorMap;

template <typename ValueType>
class ParamTensorFiller
{
public:
    virtual ~ParamTensorFiller() = default;

    virtual void fill(AbstractTensorMap<ValueType>& tensorMap) const = 0;

    DECLARE_AS_RTTI_BASE_TYPE(ParamTensorFiller<ValueType>)
};
