#pragma once

#include <helpers/PolymorphicTuple.h>

class AbstractInitializer
{
public:
    virtual inline ~AbstractInitializer();
};

template <typename... ParamTypes>
class Initializer : public AbstractInitializer
{
public:
    inline Initializer(ParamTypes&&... construcutorArgs);

    virtual inline ~Initializer();

    template <typename FunctionType>
    inline auto apply(FunctionType&& function) const;

private:
    PolymorphicTuple<ParamTypes...> m_args;
};

#include <rtti/Initializer.impl.h>
