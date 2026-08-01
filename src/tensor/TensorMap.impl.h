#pragma once

#include <tensor/TensorMap.h>

#include <tensor/Tensor.h>

#include <stdexcept>

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

IMPLEMENT_RTTI(TensorMap, AbstractTensorMap<ValueType>, PACK(typename, typename), PACK(ValueType, KeyEnum))
