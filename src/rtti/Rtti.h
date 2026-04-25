#pragma once

#include <rtti/AbstractRtti.h>

#include <helpers/BuildTimeList.h>
#include <helpers/TemplateMacros.h>

#include <string>

template <auto FuncPtr>
struct BuildTimeFunctionPointer
{};

template <typename BaseType, typename Type, typename... ConstructorArgumentLists>
class Rtti
{};

template <typename BaseType, typename Type>
class Rtti<BaseType, Type> : public AbstractRtti<BaseType>
{
public:
    inline Rtti(const std::string& typeName);

    virtual inline ~Rtti();

    virtual inline BaseType* createInstance(AbstractInitializer&& initializer) const override;

    virtual inline const std::string& getTypeName() const override;

private:
    const std::string m_typeName;
};

template <typename BaseType, typename Type, typename... FirstConstructorParameterTypes, typename... NextConstructorArgumentLists>
class Rtti<BaseType, Type, BuildTimeList::TypeList<FirstConstructorParameterTypes...>, NextConstructorArgumentLists...> : public Rtti<BaseType, Type, NextConstructorArgumentLists...>
{
public:
    inline Rtti(const std::string& typeName);

    virtual inline ~Rtti();

    virtual inline BaseType* createInstance(AbstractInitializer&& initializer) const;
};

template <typename BaseType, typename Type, typename... FirstConstructorParameterTypes, Type* (*CreateFuncPtr)(FirstConstructorParameterTypes...), typename... NextConstructorArgumentLists>
class Rtti<BaseType, Type, BuildTimeFunctionPointer<CreateFuncPtr>, NextConstructorArgumentLists...> : public Rtti<BaseType, Type, NextConstructorArgumentLists...>
{
public:
    inline Rtti(const std::string& typeName);

    virtual inline ~Rtti();

    virtual inline BaseType* createInstance(AbstractInitializer&& initializer) const;
};

#define DECLARE_AS_RTTI_BASE_TYPE(AbstractClass)                             \
    public:                                                                  \
        virtual const AbstractRtti<AbstractClass>* getClassRtti() const = 0;

#define DECLARE_RTTI(AbstractClass)                                      \
    public:                                                              \
        virtual const AbstractRtti<AbstractClass>* getClassRtti() const; \
                                                                         \
        static const AbstractRtti<AbstractClass>* getRtti();             \
        static void setRtti(const AbstractRtti<AbstractClass>* rtti);    \
                                                                         \
    private:                                                             \
        static const AbstractRtti<AbstractClass>* s_rtti;

#include <rtti/Rtti.impl.h>
