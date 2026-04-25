#pragma once

#include <rtti/RttiHolder.h>

#include <rtti/AbstractRtti.h>
#include <rtti/AbstractTemplateRtti.h>
#include <rtti/RttiBase.h>

#include <vector>

template <typename BaseType>
class RttiHolderTokenPimpl
{
public:
    RttiHolderTokenPimpl(RttiHolder<BaseType>* const owner)
        : m_owner(owner)
        , m_rttis()
    {}

    RttiHolder<BaseType>* const m_owner;

    std::vector<const RttiBase<BaseType>*> m_rttis;
};

template <typename BaseType>
RttiHolderToken<BaseType>::RttiHolderToken(RttiHolder<BaseType>* owner)
    : m_pimpl(new RttiHolderTokenPimpl<BaseType>(owner))
{}

template <typename BaseType>
RttiHolderToken<BaseType>::RttiHolderToken(RttiHolderToken<BaseType>&& other)
    : m_pimpl(other.m_pimpl)
{
    other.m_pimpl = nullptr;
}

template <typename BaseType>
RttiHolderToken<BaseType>::~RttiHolderToken()
{
    if (m_pimpl != nullptr)
    {
        for (const RttiBase<BaseType>* rtti : m_pimpl->m_rttis)
        {
            m_pimpl->m_owner->removeRtti(rtti);
        }
    }

    delete m_pimpl;
}

template <typename BaseType>
RttiHolderToken<BaseType>&
RttiHolderToken<BaseType>::operator=(RttiHolderToken<BaseType>&& other)
{
    if (m_pimpl != nullptr)
    {
        for (const RttiBase<BaseType>* rtti : m_pimpl->m_rttis)
        {
            m_pimpl->m_owner->removeRtti(rtti);
        }
    }

    delete m_pimpl;

    m_pimpl = other.m_pimpl;
    other.m_pimpl = nullptr;
    return *this;
}

template <typename BaseType>
void
RttiHolderToken<BaseType>::subscribe(const RttiBase<BaseType>* rtti)
{
    if (rtti == nullptr)
        return;

    m_pimpl->m_rttis.push_back(rtti);
    m_pimpl->m_owner->addRtti(rtti);
}

template <typename BaseType>
class RttiHolderPimpl
{
public:
    RttiHolderPimpl()
        : m_rttis()
    {}

    RttiHolderPimpl(const RttiHolderPimpl& other)
        : m_rttis(other.m_rttis)
    {}

    ~RttiHolderPimpl()
    {}

    std::vector<const RttiBase<BaseType>*> m_rttis;
};

template <typename BaseType>
RttiHolder<BaseType>::RttiHolder()
    : m_pimpl(new RttiHolderPimpl<BaseType>)
{}

template <typename BaseType>
RttiHolder<BaseType>::RttiHolder(const RttiHolder& other)
    : m_pimpl(new RttiHolderPimpl<BaseType>(*other.m_pimpl))
{}

template <typename BaseType>
RttiHolder<BaseType>::RttiHolder(RttiHolder&& other)
    : m_pimpl(other.m_pimpl)
{
    other.m_pimpl = nullptr;
}

template <typename BaseType>
RttiHolder<BaseType>::~RttiHolder()
{
    for (const RttiBase<BaseType>* rtti : m_pimpl->m_rttis)
    {
        delete rtti;
    }

    delete m_pimpl;
}

template <typename BaseType>
RttiHolder<BaseType>&
RttiHolder<BaseType>::operator=(const RttiHolder<BaseType>& other)
{
    *m_pimpl = *other.m_pimpl;
    return *this;
}

template <typename BaseType>
RttiHolder<BaseType>&
RttiHolder<BaseType>::operator=(RttiHolder<BaseType>&& other)
{
    for (const RttiBase<BaseType>* rtti : m_pimpl->m_rttis)
    {
        delete rtti;
    }

    delete m_pimpl;

    m_pimpl = other.m_pimpl;
    other.m_pimpl = nullptr;
    return *this;
}

template <typename BaseType>
const AbstractRtti<BaseType>*
RttiHolder<BaseType>::getRttiByName(const std::string& typeName) const
{
    for (const RttiBase<BaseType>* rtti : m_pimpl->m_rttis)
    {
        if (rtti->getTypeName() == typeName)
        {
            return dynamic_cast<const AbstractRtti<BaseType>*>(rtti);
        }
    }

    return nullptr;
}

template <typename BaseType>
const AbstractTemplateRtti<BaseType>*
RttiHolder<BaseType>::getTemplateRttiByName(const std::string& typeName) const
{
    for (const RttiBase<BaseType>* rtti : m_pimpl->m_rttis)
    {
        if (rtti->getTypeName() == typeName)
        {
            return dynamic_cast<const AbstractTemplateRtti<BaseType>*>(rtti);
        }
    }

    return nullptr;
}

template <typename BaseType>
RttiHolderToken<BaseType>
RttiHolder<BaseType>::edit()
{
    return RttiHolderToken<BaseType>(this);
}

template <typename BaseType>
void
RttiHolder<BaseType>::addRtti(const RttiBase<BaseType>* rtti)
{
    if (rtti == nullptr)
        return;

    m_pimpl->m_rttis.push_back(rtti);
}

template <typename BaseType>
void
RttiHolder<BaseType>::removeRtti(const RttiBase<BaseType>* rtti)
{
    for (typename std::vector<const RttiBase<BaseType>*>::const_iterator it = m_pimpl->m_rttis.begin(); it != m_pimpl->m_rttis.end(); it++)
    {
        if (*it == rtti)
        {
            m_pimpl->m_rttis.erase(it);
            delete rtti;
            return;
        }
    }
}
