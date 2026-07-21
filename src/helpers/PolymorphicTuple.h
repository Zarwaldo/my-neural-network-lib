#pragma once

#include <helpers/TupleTemplate.h>

#include <helpers/RawTuple.h>

template <typename Type, typename Enable = void>
class PolymorphicTupleElementContainer
{
};

template <typename Type>
class PolymorphicTupleElementContainer<Type, std::enable_if_t<std::is_object_v<Type>>> : public RawTupleElementContainer<Type>
{
public:
    HOST DEVICE PolymorphicTupleElementContainer(const Type& obj);
};

template <typename Type>
class PolymorphicTupleElementContainer<Type&, std::enable_if_t<std::is_object_v<Type> && !std::is_const_v<Type>>>
{
public:
    HOST DEVICE PolymorphicTupleElementContainer(Type& obj);

    HOST DEVICE Type& get() const;

private:
    Type* m_ptr;
};

template <typename Type>
class PolymorphicTupleElementContainer<const Type&, std::enable_if_t<std::is_object_v<Type> && !std::is_const_v<Type>>>
{
public:
    HOST DEVICE PolymorphicTupleElementContainer(const Type& obj);

    HOST DEVICE const Type& get() const;

private:
    const Type* m_ptr;
};

template <typename... Types>
using PolymorphicTuple = TupleTemplate<PolymorphicTupleElementContainer, Types...>;

template <typename... Types>
HOST DEVICE PolymorphicTuple<Types...> makePolymorphicTuple(Types&&... elements);

#include <helpers/PolymorphicTuple.impl.h>
