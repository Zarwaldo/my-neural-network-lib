#pragma once

#include <rtti/AbstractRtti.h>

#include <helpers/BuildTimeList.h>
#include <helpers/TemplateMacros.h>

#include <string>

template <auto FuncPtr>
struct BuildTimeFunctionPointer
{};

template <typename BaseType, typename Type, typename... CtorArgumentLists>
class Rtti
{};

template <typename BaseType, typename Type>
class Rtti<BaseType, Type> : public AbstractRtti<BaseType>
{
public:
    inline Rtti(const std::string& typeName);

    virtual inline ~Rtti();

    virtual inline BaseType* createInstance(AbstractInitializer&& initializer) const override;

    virtual inline TypenameArgId getTypenameArgId() const override;

    virtual inline const std::string& getTypeName() const override;

private:
    const std::string m_typeName;
};

template <typename BaseType, typename Type, typename... FirstCtorParameterTypes, typename... NextCtorArgumentLists>
class Rtti<BaseType, Type, BuildTimeList::TypeList<FirstCtorParameterTypes...>, NextCtorArgumentLists...> : public Rtti<BaseType, Type, NextCtorArgumentLists...>
{
public:
    inline Rtti(const std::string& typeName);

    virtual inline ~Rtti();

    virtual inline BaseType* createInstance(AbstractInitializer&& initializer) const;
};

template <typename BaseType, typename Type, typename... FirstCtorParameterTypes, Type* (*CreateFuncPtr)(FirstCtorParameterTypes...), typename... NextCtorArgumentLists>
class Rtti<BaseType, Type, BuildTimeFunctionPointer<CreateFuncPtr>, NextCtorArgumentLists...> : public Rtti<BaseType, Type, NextCtorArgumentLists...>
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
