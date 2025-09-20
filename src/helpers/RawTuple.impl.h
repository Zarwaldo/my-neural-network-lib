#include <helpers/RawTuple.h>

#include <helpers/BuildTimeList.h>

template <typename Type>
HOST DEVICE
RawTupleElementContainer<Type>::RawTupleElementContainer(const RawType& obj)
    : m_obj(obj)
{}

template <typename Type>
HOST DEVICE
const typename RawTupleElementContainer<Type>::RawType&
RawTupleElementContainer<Type>::get() const
{
    return m_obj;
}

template <typename Type>
HOST DEVICE
typename RawTupleElementContainer<Type>::RawType&
RawTupleElementContainer<Type>::get()
{
    return const_cast<RawType&>(static_cast<const RawTupleElementContainer<Type>*>(this)->get());
}

template <typename... Types>
HOST DEVICE
RawTuple<Types...>
makeRawTuple(Types&&... elements)
{
    return MakeTuple<RawTupleElementContainer>::doIt(std::forward<Types>(elements)...);
}
