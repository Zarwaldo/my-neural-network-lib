#pragma once

#include <network/ParamTensorFiller.h>

template <typename ValueType>
class NullParamTensorFiller : public ParamTensorFiller<ValueType>
{
public:
    NullParamTensorFiller();

    virtual ~NullParamTensorFiller();

    virtual void fill(AbstractTensorMap<ValueType>& tensorMap) const override;

    DECLARE_RTTI(ParamTensorFiller<ValueType>)
};
