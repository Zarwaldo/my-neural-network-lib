#pragma once

#include <rtti/RttiHolder.h>

template <typename... ContainedTypes>
class ResourcesContainerImpl;

template <typename... ContainedTypes>
class ResourcesContainerTokenImpl
{};

template <>
class ResourcesContainerTokenImpl<>
{
public:
    ResourcesContainerTokenImpl(const ResourcesContainerTokenImpl& other) = delete;
    ResourcesContainerTokenImpl(ResourcesContainerTokenImpl&& other);

    virtual ~ResourcesContainerTokenImpl();

    ResourcesContainerTokenImpl& operator=(const ResourcesContainerTokenImpl& other) = delete;
    ResourcesContainerTokenImpl& operator=(ResourcesContainerTokenImpl&& other);

    template <typename Type>
    RttiHolderToken<Type>& getRttiHolderToken();

private:
    explicit ResourcesContainerTokenImpl(ResourcesContainerImpl<>& owner);

    template <typename... ContainedTypes>
    friend class ResourcesContainerTokenImpl;
    template <typename... ContainedTypes>
    friend class ResourcesContainerImpl;
};

template <typename FirstContainedType, typename... NextContainedTypes>
class ResourcesContainerTokenImpl<FirstContainedType, NextContainedTypes...> : public ResourcesContainerTokenImpl<NextContainedTypes...>
{
public:
    ResourcesContainerTokenImpl(const ResourcesContainerTokenImpl& other) = delete;
    ResourcesContainerTokenImpl(ResourcesContainerTokenImpl&& other);

    virtual ~ResourcesContainerTokenImpl();

    ResourcesContainerTokenImpl& operator=(const ResourcesContainerTokenImpl& other) = delete;
    ResourcesContainerTokenImpl& operator=(ResourcesContainerTokenImpl&& other);

    template <typename Type>
    RttiHolderToken<Type>& getRttiHolderToken();

private:
    explicit ResourcesContainerTokenImpl(ResourcesContainerImpl<FirstContainedType, NextContainedTypes...>& owner);

    RttiHolderToken<FirstContainedType> m_firstToken;

    template <typename... ContainedTypes>
    friend class ResourcesContainerTokenImpl;
    template <typename... ContainedTypes>
    friend class ResourcesContainerImpl;
};

template <typename... ContainedTypes>
class ResourcesContainerImpl
{};

template <>
class ResourcesContainerImpl<>
{
public:
    ResourcesContainerImpl();
    ResourcesContainerImpl(const ResourcesContainerImpl& other) = delete;
    ResourcesContainerImpl(ResourcesContainerImpl&& other);

    virtual ~ResourcesContainerImpl();

    ResourcesContainerImpl& operator=(const ResourcesContainerImpl& other) = delete;
    ResourcesContainerImpl& operator=(ResourcesContainerImpl&& other);

    ResourcesContainerTokenImpl<> edit();

private:
    template <typename... ContainedTypes>
    friend class ResourcesContainerTokenImpl;
};

template <typename FirstContainedType, typename... NextContainedTypes>
class ResourcesContainerImpl<FirstContainedType, NextContainedTypes...> : public ResourcesContainerImpl<NextContainedTypes...>
{
public:
    ResourcesContainerImpl();
    ResourcesContainerImpl(const ResourcesContainerImpl& other) = delete;
    ResourcesContainerImpl(ResourcesContainerImpl&& other);

    virtual ~ResourcesContainerImpl();

    ResourcesContainerImpl& operator=(const ResourcesContainerImpl& other) = delete;
    ResourcesContainerImpl& operator=(ResourcesContainerImpl&& other);

    template <typename Type>
    const RttiHolder<Type>& getRttiHolder() const;

    ResourcesContainerTokenImpl<FirstContainedType, NextContainedTypes...> edit();

private:
    RttiHolder<FirstContainedType> m_firstRttiHolder;

    template <typename... ContainedTypes>
    friend class ResourcesContainerTokenImpl;
};
