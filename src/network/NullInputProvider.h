#pragma once

#include <network/InputProvider.h>

template <typename ValueType>
class NullInputProvider : public InputProvider<ValueType>
{
public:
    NullInputProvider();

    virtual ~NullInputProvider();

    virtual void getNewInput(AbstractTensorMap<ValueType>& inputMap) override;

    virtual bool hasMoreThan(size_t nbInputs) const override;

    DECLARE_RTTI(InputProvider<ValueType>)
};
