#pragma once

#include <rtti/Initializer.h>

#include <utility>

AbstractInitializer::~AbstractInitializer()
{}

template <typename... ParamTypes>
Initializer<ParamTypes...>::Initializer(ParamTypes&&... args)
    : m_args(makePolymorphicTuple(std::forward<ParamTypes>(args)...))
{}

template <typename... ParamTypes>
Initializer<ParamTypes...>::~Initializer()
{}

template <typename... ParamTypes>
template <typename FunctionType>
auto
Initializer<ParamTypes...>::apply(FunctionType&& function) const
{
    return m_args.hostApply(std::move(function));
}
