#pragma once

#include <rtti/RttiHolder.h>

#include <functional>

template <typename BaseType>
class StaticRttiHolder
{
public:
    explicit StaticRttiHolder(const std::function<void(RttiHolderToken<BaseType>&)>& subscriptionFunction);

    StaticRttiHolder(const StaticRttiHolder& other) = delete;
    StaticRttiHolder(StaticRttiHolder&& other) = delete;

    ~StaticRttiHolder();

    StaticRttiHolder operator=(const StaticRttiHolder& other) = delete;
    StaticRttiHolder operator=(StaticRttiHolder&& other) = delete;

    const RttiHolder<BaseType>& getRttiHolder() const;

    operator const RttiHolder<BaseType>&() const;

private:
    RttiHolder<BaseType>* m_rttiHolder;
    RttiHolderToken<BaseType>* m_rttiHolderToken;
};

#include <rtti/StaticRttiHolder.impl.h>
