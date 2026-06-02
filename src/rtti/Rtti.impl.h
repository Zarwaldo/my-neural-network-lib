#pragma once

#include <rtti/Rtti.h>

#include <rtti/Initializer.h>

#include <stdexcept>
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

template <typename BaseType, typename Type, typename... FirstCtorParameterTypes, typename... NextCtorArgumentLists>
Rtti<BaseType, Type, BuildTimeList::TypeList<FirstCtorParameterTypes...>, NextCtorArgumentLists...>::Rtti(const std::string& typeName)
    : Rtti<BaseType, Type, NextCtorArgumentLists...>(typeName)
{}

template <typename BaseType, typename Type, typename... FirstCtorParameterTypes, typename... NextCtorArgumentLists>
Rtti<BaseType, Type, BuildTimeList::TypeList<FirstCtorParameterTypes...>, NextCtorArgumentLists...>::~Rtti()
{}

template <typename BaseType, typename Type, typename... FirstCtorParameterTypes, typename... NextCtorArgumentLists>
BaseType*
Rtti<BaseType, Type, BuildTimeList::TypeList<FirstCtorParameterTypes...>, NextCtorArgumentLists...>::createInstance(AbstractInitializer&& initializer) const
{
    if (Initializer<FirstCtorParameterTypes...>* concreteInitializer = dynamic_cast<Initializer<FirstCtorParameterTypes...>*>(&initializer))
    {
        return concreteInitializer->apply([](FirstCtorParameterTypes... params) {
            return new Type(params...);
        });
    }

    return Rtti<BaseType, Type, NextCtorArgumentLists...>::createInstance(std::move(initializer));
}

template <typename BaseType, typename Type, typename... FirstCtorParameterTypes, Type* (*CreateFuncPtr)(FirstCtorParameterTypes...), typename... NextCtorArgumentLists>
Rtti<BaseType, Type, BuildTimeFunctionPointer<CreateFuncPtr>, NextCtorArgumentLists...>::Rtti(const std::string& typeName)
    : Rtti<BaseType, Type, NextCtorArgumentLists...>(typeName)
{}

template <typename BaseType, typename Type, typename... FirstCtorParameterTypes, Type* (*CreateFuncPtr)(FirstCtorParameterTypes...), typename... NextCtorArgumentLists>
Rtti<BaseType, Type, BuildTimeFunctionPointer<CreateFuncPtr>, NextCtorArgumentLists...>::~Rtti()
{}

template <typename BaseType, typename Type, typename... FirstCtorParameterTypes, Type* (*CreateFuncPtr)(FirstCtorParameterTypes...), typename... NextCtorArgumentLists>
BaseType*
Rtti<BaseType, Type, BuildTimeFunctionPointer<CreateFuncPtr>, NextCtorArgumentLists...>::createInstance(AbstractInitializer&& initializer) const
{
    if (Initializer<FirstCtorParameterTypes...>* concreteInitializer = dynamic_cast<Initializer<FirstCtorParameterTypes...>*>(&initializer))
    {
        return concreteInitializer->apply([](FirstCtorParameterTypes... params) {
            return (*CreateFuncPtr)(params...);
        });
    }

    return Rtti<BaseType, Type, NextCtorArgumentLists...>::createInstance(std::move(initializer));
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
