#pragma once

#include <rtti/TemplateRtti.h>

#include <rtti/AbstractRtti.h>
#include <rtti/Initializer.h>

#include <vector>

template <typename BaseType>
struct TemplateRttiPimpl
{
    TemplateRttiPimpl(const std::string& templateName)
        : m_templateName(templateName)
        , m_rttisMap()
    {}

    const std::string m_templateName;
    std::vector<std::pair<const AbstractInitializer*, const AbstractRtti<BaseType>*>> m_rttisMap;
};

template <typename BaseType, template <auto...> typename Template>
TemplateRtti<BaseType, Template>::TemplateRtti(const std::string& templateName)
    : m_pimpl(new TemplateRttiPimpl<BaseType>(templateName))
{}

template <typename BaseType, template <auto...> typename Template>
TemplateRtti<BaseType, Template>::~TemplateRtti()
{
    for (std::pair<const AbstractInitializer*, const AbstractRtti<BaseType>*> entry : m_pimpl->m_rttisMap)
    {
        delete entry.first;
        delete entry.second;
    }

    delete m_pimpl;
}

template <typename BaseType, template <auto...> typename Template>
const std::string&
TemplateRtti<BaseType, Template>::getTypeName() const
{
    return m_pimpl->m_templateName;
}

template <typename BaseType, template <auto...> typename Template>
const AbstractRtti<BaseType>*
TemplateRtti<BaseType, Template>::instantiate(AbstractInitializer&& initializer) const
{
    for (std::pair<const AbstractInitializer*, const AbstractRtti<BaseType>*> entry : m_pimpl->m_rttisMap)
    {
        if (*entry.first == initializer)
        {
            return entry.second;
        }
    }

    return nullptr;
}

template <typename BaseType, template <auto...> typename Template>
template <auto... TemplateParams>
void
TemplateRtti<BaseType, Template>::subscribe(const AbstractRtti<BaseType>* rtti)
{
    if (dynamic_cast<const Rtti<BaseType, Template<TemplateParams...>>*>(rtti) == nullptr)
    {
        throw std::runtime_error("TemplateRtti<BaseType, Template>::subscribe: The given rtti must be for type Template<TemplateParams...>.");
    }

    m_pimpl->m_rttisMap.push_back(
        std::pair<const AbstractInitializer*, const AbstractRtti<BaseType>*>(
            new Initializer{TemplateParams...},
            rtti
        )
    );
}
