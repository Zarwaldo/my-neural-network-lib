#pragma once

#include <tensor/api.h>

#include <rtti/Rtti.h>

#include <CommonMacros.h>

class MY_NEURAL_NETWORK_LIB__TENSOR__API AbstractTensorMapKeyEnum
{
public:
    HOST virtual inline ~AbstractTensorMapKeyEnum();

    HOST virtual operator size_t() const = 0;
    HOST virtual bool operator==(const AbstractTensorMapKeyEnum& other) const = 0;
    HOST virtual bool operator!=(const AbstractTensorMapKeyEnum& other) const = 0;

    DECLARE_AS_RTTI_BASE_TYPE(AbstractTensorMapKeyEnum)
};
