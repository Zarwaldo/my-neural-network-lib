#pragma once

#include <rtti/StaticRttiHolder.h>

template <typename BaseType>
StaticRttiHolder<BaseType>::StaticRttiHolder(const std::function<void(RttiHolderToken<BaseType>&)>& subscriptionFunction)
    : m_rttiHolder(new RttiHolder<BaseType>())
    , m_rttiHolderToken(new RttiHolderToken<BaseType>(m_rttiHolder->edit()))
{
    subscriptionFunction(*m_rttiHolderToken);
}

template <typename BaseType>
StaticRttiHolder<BaseType>::~StaticRttiHolder()
{
    delete m_rttiHolderToken;
    delete m_rttiHolder;
}

template <typename BaseType>
const RttiHolder<BaseType>&
StaticRttiHolder<BaseType>::getRttiHolder() const
{
    return *m_rttiHolder;
}

template <typename BaseType>
StaticRttiHolder<BaseType>::operator const RttiHolder<BaseType>&() const
{
    return getRttiHolder();
}
