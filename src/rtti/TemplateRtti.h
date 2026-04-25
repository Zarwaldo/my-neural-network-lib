#pragma once

#include <rtti/AbstractTemplateRtti.h>

#include <rtti/Rtti.h>

#include <helpers/BuildTimeList.h>

template <typename BaseType, template <auto...> typename Template, typename TemplateArgsToConstructorParamsMap>
class TemplateRtti
{};

template <typename BaseType, template <auto...> typename Template>
class TemplateRtti<BaseType, Template, BuildTimeList::Map<>> : public AbstractTemplateRtti<BaseType>
{
public:
    inline TemplateRtti(const std::string& typeName);

    virtual inline ~TemplateRtti();

    virtual inline const std::string& getTypeName() const override;

    virtual inline const AbstractRtti<BaseType>* instantiate(AbstractInitializer&& initializer) const override;

private:
    const std::string m_typeName;
};

template <typename BaseType, template <auto...> typename Template, auto... FirstTemplateArgs, typename... FirstConstructorParamTypes, typename... NextMapEntries>
class TemplateRtti<BaseType, Template, BuildTimeList::Map<BuildTimeList::MapEntry<BuildTimeList::Tuple<FirstTemplateArgs...>, BuildTimeList::TypeList<FirstConstructorParamTypes...>>, NextMapEntries...>> : public TemplateRtti<BaseType, Template, BuildTimeList::Map<NextMapEntries...>>
{
public:
    inline TemplateRtti(const std::string& typeName);

    virtual inline ~TemplateRtti();

    virtual inline const AbstractRtti<BaseType>* instantiate(AbstractInitializer&& initializer) const override;

private:
    static inline std::string buildTypeName(const std::string& templateTypeName);

    const Rtti<BaseType, Template<FirstTemplateArgs...>, FirstConstructorParamTypes...> m_rtti;
};

#include <rtti/TemplateRtti.impl.h>
