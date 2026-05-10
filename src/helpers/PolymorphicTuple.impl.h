#pragma once

#include <helpers/PolymorphicTuple.h>

template <typename Type>
HOST DEVICE
PolymorphicTupleElementContainer<Type, std::enable_if_t<std::is_object_v<Type>>>::PolymorphicTupleElementContainer(const Type& obj)
    : RawTupleElementContainer<Type>(obj)
{}

template <typename Type>
HOST DEVICE
PolymorphicTupleElementContainer<Type&, std::enable_if_t<std::is_object_v<Type> && !std::is_const_v<Type>>>::PolymorphicTupleElementContainer(Type& obj)
    : m_ptr(&obj)
{}

template <typename Type>
HOST DEVICE
const Type&
PolymorphicTupleElementContainer<Type&, std::enable_if_t<std::is_object_v<Type> && !std::is_const_v<Type>>>::get() const
{
    return *m_ptr;
}

template <typename Type>
HOST DEVICE
Type&
PolymorphicTupleElementContainer<Type&, std::enable_if_t<std::is_object_v<Type> && !std::is_const_v<Type>>>::get()
{
    return const_cast<Type&>(static_cast<const PolymorphicTupleElementContainer<Type&>*>(this)->get());
}

template <typename Type>
HOST DEVICE
PolymorphicTupleElementContainer<const Type&, std::enable_if_t<std::is_object_v<Type> && !std::is_const_v<Type>>>::PolymorphicTupleElementContainer(const Type& obj)
    : m_ptr(&obj)
{}

template <typename Type>
HOST DEVICE
const Type&
PolymorphicTupleElementContainer<const Type&, std::enable_if_t<std::is_object_v<Type> && !std::is_const_v<Type>>>::get() const
{
    return *m_ptr;
}

template <typename... Types>
HOST DEVICE
PolymorphicTuple<Types...>
makePolymorphicTuple(Types&&... elements)
{
    return MakeTuple<PolymorphicTupleElementContainer>::doIt(std::forward<Types>(elements)...);
}
