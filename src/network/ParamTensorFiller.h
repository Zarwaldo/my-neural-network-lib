#pragma once

#include <rtti/Rtti.h>

template <typename ScalarType>
class AbstractTensorMap;

template <typename ScalarType>
class ParamTensorFiller
{
public:
    virtual ~ParamTensorFiller() = default;

    virtual void fill(AbstractTensorMap<ScalarType>& tensorMap) const = 0;

    DECLARE_AS_RTTI_BASE_TYPE(ParamTensorFiller<ScalarType>)
};
