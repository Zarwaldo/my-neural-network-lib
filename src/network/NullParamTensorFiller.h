#pragma once

#include <network/ParamTensorFiller.h>

template <typename ScalarType>
class NullParamTensorFiller : public ParamTensorFiller<ScalarType>
{
public:
    NullParamTensorFiller();

    virtual ~NullParamTensorFiller();

    virtual void fill(AbstractTensorMap<ScalarType>& tensorMap) const override;

    DECLARE_RTTI(ParamTensorFiller<ScalarType>)
};
