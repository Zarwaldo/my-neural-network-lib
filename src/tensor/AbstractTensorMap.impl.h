#pragma once

#include <tensor/AbstractTensorMap.h>

template <typename ValueType>
HOST
AbstractTensorMap<ValueType>::~AbstractTensorMap()
{}

#define __TENSOR_MAP__ITERATOR_IMPLEMENTATION(IteratorName, constQualifier)                                                                              \
    template <typename ValueType>                                                                                                                        \
    AbstractTensorMap<ValueType>::IteratorName::IteratorName(constQualifier AbstractTensorMap<ValueType>& tensorMap, size_t index)                       \
        : m_tensorMap(tensorMap)                                                                                                                         \
        , m_index(index)                                                                                                                                 \
    {}                                                                                                                                                   \
                                                                                                                                                         \
    template <typename ValueType>                                                                                                                        \
    bool                                                                                                                                                 \
    AbstractTensorMap<ValueType>::IteratorName::operator==(const IteratorName& other) const                                                              \
    {                                                                                                                                                    \
        return (&m_tensorMap == &other.m_tensorMap) && (m_index == other.m_index);                                                                       \
    }                                                                                                                                                    \
                                                                                                                                                         \
    template <typename ValueType>                                                                                                                        \
    bool                                                                                                                                                 \
    AbstractTensorMap<ValueType>::IteratorName::operator!=(const IteratorName& other) const                                                              \
    {                                                                                                                                                    \
        return !(*this == other);                                                                                                                        \
    }                                                                                                                                                    \
                                                                                                                                                         \
    template <typename ValueType>                                                                                                                        \
    typename AbstractTensorMap<ValueType>::IteratorName&                                                                                                 \
    AbstractTensorMap<ValueType>::IteratorName::operator++()                                                                                             \
    {                                                                                                                                                    \
        ++m_index;                                                                                                                                       \
        return *this;                                                                                                                                    \
    }                                                                                                                                                    \
                                                                                                                                                         \
    template <typename ValueType>                                                                                                                        \
    const AbstractTensor<ValueType>&                                                                                                                     \
    AbstractTensorMap<ValueType>::IteratorName::operator*() const                                                                                        \
    {                                                                                                                                                    \
        return m_tensorMap.get(m_index);                                                                                                                 \
    }                                                                                                                                                    \
                                                                                                                                                         \
    template <typename ValueType>                                                                                                                        \
    constQualifier AbstractTensor<ValueType>&                                                                                                            \
    AbstractTensorMap<ValueType>::IteratorName::operator*()                                                                                              \
    {                                                                                                                                                    \
        return const_cast<constQualifier AbstractTensor<ValueType>&>(static_cast<const AbstractTensorMap<ValueType>::IteratorName*>(this)->operator*()); \
    }

__TENSOR_MAP__ITERATOR_IMPLEMENTATION(IteratorConst, const)
__TENSOR_MAP__ITERATOR_IMPLEMENTATION(Iterator, )

template <typename ValueType>
typename AbstractTensorMap<ValueType>::IteratorConst
AbstractTensorMap<ValueType>::begin() const
{
    return IteratorConst(*this, 0);
}

template <typename ValueType>
typename AbstractTensorMap<ValueType>::Iterator
AbstractTensorMap<ValueType>::begin()
{
    return Iterator(*this, 0);
}

template <typename ValueType>
typename AbstractTensorMap<ValueType>::IteratorConst
AbstractTensorMap<ValueType>::end() const
{
    return IteratorConst(*this, nbTensors());
}

template <typename ValueType>
typename AbstractTensorMap<ValueType>::Iterator
AbstractTensorMap<ValueType>::end()
{
    return Iterator(*this, nbTensors());
}
