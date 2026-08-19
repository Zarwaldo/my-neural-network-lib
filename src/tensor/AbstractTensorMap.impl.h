#pragma once

#include <tensor/AbstractTensorMap.h>

template <typename ScalarType>
HOST
AbstractTensorMap<ScalarType>::~AbstractTensorMap()
{}

#define __TENSOR_MAP__ITERATOR_IMPLEMENTATION(IteratorName, constQualifier)                                                                                \
    template <typename ScalarType>                                                                                                                         \
    AbstractTensorMap<ScalarType>::IteratorName::IteratorName(constQualifier AbstractTensorMap<ScalarType>& tensorMap, size_t index)                       \
        : m_tensorMap(tensorMap)                                                                                                                           \
        , m_index(index)                                                                                                                                   \
    {}                                                                                                                                                     \
                                                                                                                                                           \
    template <typename ScalarType>                                                                                                                         \
    bool                                                                                                                                                   \
    AbstractTensorMap<ScalarType>::IteratorName::operator==(const IteratorName& other) const                                                               \
    {                                                                                                                                                      \
        return (&m_tensorMap == &other.m_tensorMap) && (m_index == other.m_index);                                                                         \
    }                                                                                                                                                      \
                                                                                                                                                           \
    template <typename ScalarType>                                                                                                                         \
    bool                                                                                                                                                   \
    AbstractTensorMap<ScalarType>::IteratorName::operator!=(const IteratorName& other) const                                                               \
    {                                                                                                                                                      \
        return !(*this == other);                                                                                                                          \
    }                                                                                                                                                      \
                                                                                                                                                           \
    template <typename ScalarType>                                                                                                                         \
    typename AbstractTensorMap<ScalarType>::IteratorName&                                                                                                  \
    AbstractTensorMap<ScalarType>::IteratorName::operator++()                                                                                              \
    {                                                                                                                                                      \
        ++m_index;                                                                                                                                         \
        return *this;                                                                                                                                      \
    }                                                                                                                                                      \
                                                                                                                                                           \
    template <typename ScalarType>                                                                                                                         \
    const AbstractTensor<ScalarType>&                                                                                                                      \
    AbstractTensorMap<ScalarType>::IteratorName::operator*() const                                                                                         \
    {                                                                                                                                                      \
        return m_tensorMap.get(m_index);                                                                                                                   \
    }                                                                                                                                                      \
                                                                                                                                                           \
    template <typename ScalarType>                                                                                                                         \
    constQualifier AbstractTensor<ScalarType>&                                                                                                             \
    AbstractTensorMap<ScalarType>::IteratorName::operator*()                                                                                               \
    {                                                                                                                                                      \
        return const_cast<constQualifier AbstractTensor<ScalarType>&>(static_cast<const AbstractTensorMap<ScalarType>::IteratorName*>(this)->operator*()); \
    }

__TENSOR_MAP__ITERATOR_IMPLEMENTATION(IteratorConst, const)
__TENSOR_MAP__ITERATOR_IMPLEMENTATION(Iterator, )

template <typename ScalarType>
typename AbstractTensorMap<ScalarType>::IteratorConst
AbstractTensorMap<ScalarType>::begin() const
{
    return IteratorConst(*this, 0);
}

template <typename ScalarType>
typename AbstractTensorMap<ScalarType>::Iterator
AbstractTensorMap<ScalarType>::begin()
{
    return Iterator(*this, 0);
}

template <typename ScalarType>
typename AbstractTensorMap<ScalarType>::IteratorConst
AbstractTensorMap<ScalarType>::end() const
{
    return IteratorConst(*this, nbTensors());
}

template <typename ScalarType>
typename AbstractTensorMap<ScalarType>::Iterator
AbstractTensorMap<ScalarType>::end()
{
    return Iterator(*this, nbTensors());
}
