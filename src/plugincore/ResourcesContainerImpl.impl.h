#pragma once

#include <plugincore/ResourcesContainerImpl.h>

ResourcesContainerTokenImpl<>::ResourcesContainerTokenImpl(ResourcesContainerImpl<>& owner)
{}

ResourcesContainerTokenImpl<>::ResourcesContainerTokenImpl(ResourcesContainerTokenImpl&& other)
{}

ResourcesContainerTokenImpl<>::~ResourcesContainerTokenImpl()
{}

ResourcesContainerTokenImpl<>&
ResourcesContainerTokenImpl<>::operator=(ResourcesContainerTokenImpl&& other)
{
    return *this;
}

template <typename Type>
RttiHolderToken<Type>&
ResourcesContainerTokenImpl<>::getRttiHolderToken()
{}

template <typename FirstContainedType, typename... NextContainedTypes>
ResourcesContainerTokenImpl<FirstContainedType, NextContainedTypes...>::ResourcesContainerTokenImpl(ResourcesContainerImpl<FirstContainedType, NextContainedTypes...>& owner)
    : ResourcesContainerTokenImpl<NextContainedTypes...>(owner)
    , m_firstToken(owner.m_firstRttiHolder.edit())
{}

template <typename FirstContainedType, typename... NextContainedTypes>
ResourcesContainerTokenImpl<FirstContainedType, NextContainedTypes...>::ResourcesContainerTokenImpl(ResourcesContainerTokenImpl&& other)
    : ResourcesContainerTokenImpl<NextContainedTypes...>(std::move(other))
    , m_firstToken(std::move(other.m_firstToken))
{}

template <typename FirstContainedType, typename... NextContainedTypes>
ResourcesContainerTokenImpl<FirstContainedType, NextContainedTypes...>::~ResourcesContainerTokenImpl()
{}

template <typename FirstContainedType, typename... NextContainedTypes>
ResourcesContainerTokenImpl<FirstContainedType, NextContainedTypes...>&
ResourcesContainerTokenImpl<FirstContainedType, NextContainedTypes...>::operator=(ResourcesContainerTokenImpl&& other)
{
    ResourcesContainerTokenImpl<NextContainedTypes...>::operator=(std::move(other));
    m_firstToken = std::move(other.m_firstToken);
    return *this;
}

template <typename FirstContainedType, typename... NextContainedTypes>
template <typename Type>
RttiHolderToken<Type>&
ResourcesContainerTokenImpl<FirstContainedType, NextContainedTypes...>::getRttiHolderToken()
{
    if constexpr (std::is_same_v<Type, FirstContainedType>)
    {
        return m_firstToken;
    }
    else
    {
        return ResourcesContainerTokenImpl<NextContainedTypes...>::getRttiHolderToken<Type>();
    }
}

ResourcesContainerImpl<>::ResourcesContainerImpl()
{}

ResourcesContainerImpl<>::ResourcesContainerImpl(ResourcesContainerImpl&& other)
{}

ResourcesContainerImpl<>::~ResourcesContainerImpl()
{}

ResourcesContainerImpl<>&
ResourcesContainerImpl<>::operator=(ResourcesContainerImpl&& other)
{
    return *this;
}

ResourcesContainerTokenImpl<>
ResourcesContainerImpl<>::edit()
{
    return ResourcesContainerTokenImpl<>(*this);
}

template <typename FirstContainedType, typename... NextContainedTypes>
ResourcesContainerImpl<FirstContainedType, NextContainedTypes...>::ResourcesContainerImpl()
    : ResourcesContainerImpl<NextContainedTypes...>()
    , m_firstRttiHolder()
{}

template <typename FirstContainedType, typename... NextContainedTypes>
ResourcesContainerImpl<FirstContainedType, NextContainedTypes...>::ResourcesContainerImpl(ResourcesContainerImpl&& other)
    : ResourcesContainerImpl<NextContainedTypes...>(std::move(other))
    , m_firstRttiHolder(std::move(other.m_firstRttiHolder))
{}

template <typename FirstContainedType, typename... NextContainedTypes>
ResourcesContainerImpl<FirstContainedType, NextContainedTypes...>::~ResourcesContainerImpl()
{}

template <typename FirstContainedType, typename... NextContainedTypes>
ResourcesContainerImpl<FirstContainedType, NextContainedTypes...>&
ResourcesContainerImpl<FirstContainedType, NextContainedTypes...>::operator=(ResourcesContainerImpl&& other)
{
    ResourcesContainerImpl<NextContainedTypes...>::operator=(std::move(other));
    m_firstRttiHolder = std::move(other.m_firstRttiHolder);
    return *this;
}

template <typename FirstContainedType, typename... NextContainedTypes>
template <typename Type>
const RttiHolder<Type>&
ResourcesContainerImpl<FirstContainedType, NextContainedTypes...>::getRttiHolder() const
{
    if constexpr (std::is_same_v<Type, FirstContainedType>)
    {
        return m_firstRttiHolder;
    }
    else
    {
        return ResourcesContainerImpl<NextContainedTypes...>::getRttiHolder<Type>();
    }
}

template <typename FirstContainedType, typename... NextContainedTypes>
ResourcesContainerTokenImpl<FirstContainedType, NextContainedTypes...>
ResourcesContainerImpl<FirstContainedType, NextContainedTypes...>::edit()
{
    return ResourcesContainerTokenImpl<FirstContainedType, NextContainedTypes...>(*this);
}
