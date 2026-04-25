#pragma once

#include <rtti/TemplateRtti.h>

#include <rtti/AbstractRtti.h>
#include <rtti/Initializer.h>

template <typename BaseType, template <auto...> typename Template>
TemplateRtti<BaseType, Template, BuildTimeList::Map<>>::TemplateRtti(const std::string& typeName)
    : m_typeName(typeName)
{}

template <typename BaseType, template <auto...> typename Template>
TemplateRtti<BaseType, Template, BuildTimeList::Map<>>::~TemplateRtti()
{}


template <typename BaseType, template <auto...> typename Template>
const std::string&
TemplateRtti<BaseType, Template, BuildTimeList::Map<>>::getTypeName() const
{
    return m_typeName;
}

template <typename BaseType, template <auto...> typename Template>
const AbstractRtti<BaseType>*
TemplateRtti<BaseType, Template, BuildTimeList::Map<>>::instantiate(AbstractInitializer&& initializer) const
{
    return nullptr;
}

template <typename BaseType, template <auto...> typename Template, auto... FirstTemplateArgs, typename... FirstConstructorParamTypes, typename... NextMapEntries>
TemplateRtti<BaseType, Template, BuildTimeList::Map<BuildTimeList::MapEntry<BuildTimeList::Tuple<FirstTemplateArgs...>, BuildTimeList::TypeList<FirstConstructorParamTypes...>>, NextMapEntries...>>::TemplateRtti(const std::string& templateTypeName)
    : TemplateRtti<BaseType, Template, BuildTimeList::Map<NextMapEntries...>>(templateTypeName)
    , m_rtti(buildTypeName(templateTypeName))
{}

template <typename BaseType, template <auto...> typename Template, auto... FirstTemplateArgs, typename... FirstConstructorParamTypes, typename... NextMapEntries>
TemplateRtti<BaseType, Template, BuildTimeList::Map<BuildTimeList::MapEntry<BuildTimeList::Tuple<FirstTemplateArgs...>, BuildTimeList::TypeList<FirstConstructorParamTypes...>>, NextMapEntries...>>::~TemplateRtti()
{
}

template <typename BaseType, template <auto...> typename Template, auto... FirstTemplateArgs, typename... FirstConstructorParamTypes, typename... NextMapEntries>
const AbstractRtti<BaseType>*
TemplateRtti<BaseType, Template, BuildTimeList::Map<BuildTimeList::MapEntry<BuildTimeList::Tuple<FirstTemplateArgs...>, BuildTimeList::TypeList<FirstConstructorParamTypes...>>, NextMapEntries...>>::instantiate(AbstractInitializer&& initializer) const
{
    if (Initializer<decltype(FirstTemplateArgs)...>* concreteInitializer = dynamic_cast<Initializer<decltype(FirstTemplateArgs)...>*>(&initializer))
    {
        if (concreteInitializer->apply([](decltype(FirstTemplateArgs)... runtimeArgs) { return ((FirstTemplateArgs == runtimeArgs) && ...); }))
        {
            return &m_rtti;
        }
    }

    return TemplateRtti<BaseType, Template, BuildTimeList::Map<NextMapEntries...>>::instantiate(std::move(initializer));
}

template <typename BaseType, template <auto...> typename Template, auto... FirstTemplateArgs, typename... FirstConstructorParamTypes, typename... NextMapEntries>
std::string
TemplateRtti<BaseType, Template, BuildTimeList::Map<BuildTimeList::MapEntry<BuildTimeList::Tuple<FirstTemplateArgs...>, BuildTimeList::TypeList<FirstConstructorParamTypes...>>, NextMapEntries...>>::buildTypeName(const std::string& templateTypeName)
{
    const std::string argsStrings[] = { std::to_string(FirstTemplateArgs)... };

    std::string result = templateTypeName + "<";
    for (size_t i = 0; i < sizeof...(FirstTemplateArgs); ++i)
    {
        result += argsStrings[i];
        if (i + 1 < sizeof...(FirstTemplateArgs))
            result += ",";
    }
    result += ">";

    return result;
}
