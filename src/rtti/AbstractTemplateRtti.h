#pragma once

#include <rtti/RttiBase.h>

#include <rtti/AbstractRtti.h>

class AbstractInitializer;

template <typename BaseType>
class AbstractTemplateRtti : public RttiBase<BaseType>
{
public:
    virtual inline ~AbstractTemplateRtti();

    virtual inline const AbstractRtti<BaseType>* instantiate(AbstractInitializer&& initializer) const = 0;
};

#include <rtti/AbstractTemplateRtti.impl.h>
