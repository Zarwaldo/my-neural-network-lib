#pragma once

#include <network/OutputReceiver.h>

template <typename ScalarType>
class NullOutputReceiver : public OutputReceiver<ScalarType>
{
public:
    NullOutputReceiver();

    virtual ~NullOutputReceiver();

    virtual void sendOutput(const AbstractTensorMap<ScalarType>& outputMap) override;

    DECLARE_RTTI(OutputReceiver<ScalarType>)
};
