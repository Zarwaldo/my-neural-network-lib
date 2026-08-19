#pragma once

#include <network/InputProvider.h>

template <typename ScalarType>
class NullInputProvider : public InputProvider<ScalarType>
{
public:
    NullInputProvider();

    virtual ~NullInputProvider();

    virtual void getNewInput(AbstractTensorMap<ScalarType>& inputMap) override;

    virtual bool hasMoreThan(size_t nbInputs) const override;

    DECLARE_RTTI(InputProvider<ScalarType>)
};
