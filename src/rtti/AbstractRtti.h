#pragma once

#include <rtti/RttiBase.h>

class AbstractInitializer;

template <typename BaseType>
class AbstractRtti : public RttiBase<BaseType>
{
public:
    virtual inline ~AbstractRtti();

    virtual inline BaseType* createInstance(AbstractInitializer&& initializer) const = 0;
};

#include <rtti/AbstractRtti.impl.h>
