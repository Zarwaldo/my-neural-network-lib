#pragma once

#include <tensor/api.h>

#include <CommonMacros.h>

#include <rtti/Rtti.h>

template <typename BaseType>
class AbstractTemplateRtti;

class MY_NEURAL_NETWORK_LIB__TENSOR__API TensorBase
{
public:
    HOST virtual ~TensorBase();

    HOST static const AbstractTemplateRtti<TensorBase>& templateRtti();

    DECLARE_AS_RTTI_BASE_TYPE(TensorBase)
};
