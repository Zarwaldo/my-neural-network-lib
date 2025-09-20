#pragma once

#include <helpers/TupleTemplate.h>

template <typename Type>
class RawTupleElementContainer
{
    using RawType = std::remove_const_t<std::remove_reference_t<Type>>;

public:
    HOST DEVICE RawTupleElementContainer(const RawType& obj);

    HOST DEVICE const RawType& get() const;
    HOST DEVICE RawType& get();

private:
    RawType m_obj;
};

template <typename... Types>
using RawTuple = TupleTemplate<RawTupleElementContainer, Types...>;

template <typename... Types>
HOST DEVICE
RawTuple<Types...> makeRawTuple(Types&&... elements);

#include <helpers/RawTuple.impl.h>
