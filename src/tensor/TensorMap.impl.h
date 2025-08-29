#pragma once

#include <tensor/TensorMap.h>

#include <tensor/Tensor.h>

#include <stdexcept>

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

template <typename ValueType, typename KeyEnum>
HOST
TensorMap<ValueType, KeyEnum>::TensorMap(AbstractTensor<ValueType>* const tensors[KeyEnum::NbValues])
{
    for (size_t i = 0; i < KeyEnum::NbValues; ++i)
    {
        m_tensors[i] = tensors[i];
    }
}

template <typename ValueType, typename KeyEnum>
HOST
TensorMap<ValueType, KeyEnum>::TensorMap(const std::initializer_list<AbstractTensor<ValueType>*>& list)
{
    if (list.size() != KeyEnum::NbValues)
    {
        throw std::runtime_error("TensorMap: wrong number of tensors in initializer list");
    }

    size_t index = 0;
    for (AbstractTensor<ValueType>* tensor : list)
    {
        m_tensors[index] = tensor;
        ++index;
    }
}

template <typename ValueType, typename KeyEnum>
HOST
TensorMap<ValueType, KeyEnum>::TensorMap(const TensorMap& other)
{
    for (size_t i = 0; i < KeyEnum::NbValues; ++i)
    {
        m_tensors[i] = other.m_tensors[i];
    }
}

template <typename ValueType, typename KeyEnum>
HOST
TensorMap<ValueType, KeyEnum>::~TensorMap()
{}

template <typename ValueType, typename KeyEnum>
HOST
size_t
TensorMap<ValueType, KeyEnum>::nbTensors() const
{
    return KeyEnum::NbValues;
}

template <typename ValueType, typename KeyEnum>
HOST
const AbstractTensor<ValueType>&
TensorMap<ValueType, KeyEnum>::get(size_t index) const
{
    return get(static_cast<KeyEnum>(index));
}

template <typename ValueType, typename KeyEnum>
HOST
AbstractTensor<ValueType>&
TensorMap<ValueType, KeyEnum>::get(size_t index)
{
    return const_cast<AbstractTensor<ValueType>&>(static_cast<const TensorMap<ValueType, KeyEnum>*>(this)->get(index));
}

template <typename ValueType, typename KeyEnum>
HOST
const AbstractTensor<ValueType>&
TensorMap<ValueType, KeyEnum>::get(KeyEnum key) const
{
    return *m_tensors[static_cast<size_t>(key)];
}

template <typename ValueType, typename KeyEnum>
HOST
AbstractTensor<ValueType>&
TensorMap<ValueType, KeyEnum>::get(KeyEnum key)
{
    return const_cast<AbstractTensor<ValueType>&>(static_cast<const TensorMap<ValueType, KeyEnum>*>(this)->get(key));
}
