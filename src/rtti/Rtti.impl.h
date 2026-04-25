#pragma once

#include <rtti/Rtti.h>

#include <rtti/Initializer.h>

#include <string>

template <typename BaseType, typename Type>
Rtti<BaseType, Type>::Rtti(const std::string& typeName)
    : m_typeName(typeName)
{
    Type::setRtti(this);
}

template <typename BaseType, typename Type>
Rtti<BaseType, Type>::~Rtti()
{
    Type::setRtti(nullptr);
}

template <typename BaseType, typename Type>
BaseType*
Rtti<BaseType, Type>::createInstance(AbstractInitializer&& initializer) const
{
    return nullptr;
}

template <typename BaseType, typename Type>
const std::string&
Rtti<BaseType, Type>::getTypeName() const
{
    return m_typeName;
}

template <typename BaseType, typename Type, typename... FirstConstructorParameterTypes, typename... NextConstructorArgumentLists>
Rtti<BaseType, Type, BuildTimeList::TypeList<FirstConstructorParameterTypes...>, NextConstructorArgumentLists...>::Rtti(const std::string& typeName)
    : Rtti<BaseType, Type, NextConstructorArgumentLists...>(typeName)
{}

template <typename BaseType, typename Type, typename... FirstConstructorParameterTypes, typename... NextConstructorArgumentLists>
Rtti<BaseType, Type, BuildTimeList::TypeList<FirstConstructorParameterTypes...>, NextConstructorArgumentLists...>::~Rtti()
{}

template <typename BaseType, typename Type, typename... FirstConstructorParameterTypes, typename... NextConstructorArgumentLists>
BaseType*
Rtti<BaseType, Type, BuildTimeList::TypeList<FirstConstructorParameterTypes...>, NextConstructorArgumentLists...>::createInstance(AbstractInitializer&& initializer) const
{
    if (Initializer<FirstConstructorParameterTypes...>* concreteInitializer = dynamic_cast<Initializer<FirstConstructorParameterTypes...>*>(&initializer))
    {
        return concreteInitializer->apply([](FirstConstructorParameterTypes... params) {
            return new Type(params...);
        });
    }

    return Rtti<BaseType, Type, NextConstructorArgumentLists...>::createInstance(std::move(initializer));
}

template <typename BaseType, typename Type, typename... FirstConstructorParameterTypes, Type* (*CreateFuncPtr)(FirstConstructorParameterTypes...), typename... NextConstructorArgumentLists>
Rtti<BaseType, Type, BuildTimeFunctionPointer<CreateFuncPtr>, NextConstructorArgumentLists...>::Rtti(const std::string& typeName)
    : Rtti<BaseType, Type, NextConstructorArgumentLists...>(typeName)
{}

template <typename BaseType, typename Type, typename... FirstConstructorParameterTypes, Type* (*CreateFuncPtr)(FirstConstructorParameterTypes...), typename... NextConstructorArgumentLists>
Rtti<BaseType, Type, BuildTimeFunctionPointer<CreateFuncPtr>, NextConstructorArgumentLists...>::~Rtti()
{}

template <typename BaseType, typename Type, typename... FirstConstructorParameterTypes, Type* (*CreateFuncPtr)(FirstConstructorParameterTypes...), typename... NextConstructorArgumentLists>
BaseType*
Rtti<BaseType, Type, BuildTimeFunctionPointer<CreateFuncPtr>, NextConstructorArgumentLists...>::createInstance(AbstractInitializer&& initializer) const
{
    if (Initializer<FirstConstructorParameterTypes...>* concreteInitializer = dynamic_cast<Initializer<FirstConstructorParameterTypes...>*>(&initializer))
    {
        return concreteInitializer->apply([](FirstConstructorParameterTypes... params) {
            return (*CreateFuncPtr)(params...);
        });
    }

    return Rtti<BaseType, Type, NextConstructorArgumentLists...>::createInstance(std::move(initializer));
}

#define IMPLEMENT_RTTI(ConcreteClass, AbstractClass, TemplateParamTypesPack, TemplateParamNamesPack)                                                             \
    TEMPLATE_MACROS__TEMPLATE_PREFIX(TemplateParamTypesPack, TemplateParamNamesPack)                                                                             \
    const AbstractRtti<AbstractClass>*                                                                                                                           \
    TEMPLATE_MACROS__TEMPLATE_INSTANTIATION(ConcreteClass, TemplateParamTypesPack, TemplateParamNamesPack)::getClassRtti() const                                 \
    {                                                                                                                                                            \
        return getRtti();                                                                                                                                        \
    }                                                                                                                                                            \
                                                                                                                                                                 \
    TEMPLATE_MACROS__TEMPLATE_PREFIX(TemplateParamTypesPack, TemplateParamNamesPack)                                                                             \
    const AbstractRtti<AbstractClass>*                                                                                                                           \
    TEMPLATE_MACROS__TEMPLATE_INSTANTIATION(ConcreteClass, TemplateParamTypesPack, TemplateParamNamesPack)::getRtti()                                            \
    {                                                                                                                                                            \
        return s_rtti;                                                                                                                                           \
    }                                                                                                                                                            \
                                                                                                                                                                 \
    TEMPLATE_MACROS__TEMPLATE_PREFIX(TemplateParamTypesPack, TemplateParamNamesPack)                                                                             \
    void                                                                                                                                                         \
    TEMPLATE_MACROS__TEMPLATE_INSTANTIATION(ConcreteClass, TemplateParamTypesPack, TemplateParamNamesPack)::setRtti(const AbstractRtti<AbstractClass>* rtti)     \
    {                                                                                                                                                            \
        if (rtti != nullptr && s_rtti != nullptr)                                                                                                                \
        {                                                                                                                                                        \
            throw std::runtime_error(s_rtti->getTypeName() + ": This type already has an rtti.");                                                                \
        }                                                                                                                                                        \
                                                                                                                                                                 \
        s_rtti = rtti;                                                                                                                                           \
    }                                                                                                                                                            \
                                                                                                                                                                 \
    TEMPLATE_MACROS__TEMPLATE_PREFIX(TemplateParamTypesPack, TemplateParamNamesPack)                                                                             \
    const AbstractRtti<AbstractClass>* TEMPLATE_MACROS__TEMPLATE_INSTANTIATION(ConcreteClass, TemplateParamTypesPack, TemplateParamNamesPack)::s_rtti = nullptr;
