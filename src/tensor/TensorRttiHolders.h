#pragma once

#include <CommonMacros.h>

#include <tensor/api.h>

template <typename ValueType>
class AbstractTemplateRtti;
template <typename ValueType>
class AbstractTensor;

namespace TensorRttis
{
    template <typename ValueType>
    const AbstractTemplateRtti<AbstractTensor<ValueType>>& tensorTemplateRtti();
};
