#pragma once

#include <network/OutputReceiver.h>

template <typename ValueType>
class NullOutputReceiver : public OutputReceiver<ValueType>
{
public:
    NullOutputReceiver();

    virtual ~NullOutputReceiver();

    virtual void sendOutput(const AbstractTensorMap<ValueType>& outputMap) override;

    DECLARE_RTTI(OutputReceiver<ValueType>)
};
