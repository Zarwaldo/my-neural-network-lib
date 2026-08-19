#pragma once

#include <tensor/TensorMap.h>

#include <tensor/Tensor.h>

#include <stdexcept>

template <typename ScalarType, typename KeyEnum>
HOST
TensorMap<ScalarType, KeyEnum>::TensorMap(AbstractTensor<ScalarType>* const tensors[KeyEnum::NbValues])
{
    for (size_t i = 0; i < KeyEnum::NbValues; ++i)
    {
        m_tensors[i] = tensors[i];
    }
}

template <typename ScalarType, typename KeyEnum>
HOST
TensorMap<ScalarType, KeyEnum>::TensorMap(const std::initializer_list<AbstractTensor<ScalarType>*>& list)
{
    if (list.size() != KeyEnum::NbValues)
    {
        throw std::runtime_error("TensorMap: wrong number of tensors in initializer list");
    }

    size_t index = 0;
    for (AbstractTensor<ScalarType>* tensor : list)
    {
        m_tensors[index] = tensor;
        ++index;
    }
}

template <typename ScalarType, typename KeyEnum>
HOST
TensorMap<ScalarType, KeyEnum>::TensorMap(const TensorMap& other)
{
    for (size_t i = 0; i < KeyEnum::NbValues; ++i)
    {
        m_tensors[i] = other.m_tensors[i];
    }
}

template <typename ScalarType, typename KeyEnum>
HOST
TensorMap<ScalarType, KeyEnum>::~TensorMap()
{}

template <typename ScalarType, typename KeyEnum>
HOST
size_t
TensorMap<ScalarType, KeyEnum>::nbTensors() const
{
    return KeyEnum::NbValues;
}

template <typename ScalarType, typename KeyEnum>
HOST
const AbstractTensor<ScalarType>&
TensorMap<ScalarType, KeyEnum>::get(size_t index) const
{
    return get(static_cast<KeyEnum>(index));
}

template <typename ScalarType, typename KeyEnum>
HOST
AbstractTensor<ScalarType>&
TensorMap<ScalarType, KeyEnum>::get(size_t index)
{
    return const_cast<AbstractTensor<ScalarType>&>(static_cast<const TensorMap<ScalarType, KeyEnum>*>(this)->get(index));
}

template <typename ScalarType, typename KeyEnum>
HOST
const AbstractTensor<ScalarType>&
TensorMap<ScalarType, KeyEnum>::get(KeyEnum key) const
{
    return *m_tensors[static_cast<size_t>(key)];
}

template <typename ScalarType, typename KeyEnum>
HOST
AbstractTensor<ScalarType>&
TensorMap<ScalarType, KeyEnum>::get(KeyEnum key)
{
    return const_cast<AbstractTensor<ScalarType>&>(static_cast<const TensorMap<ScalarType, KeyEnum>*>(this)->get(key));
}

IMPLEMENT_RTTI(TensorMap, AbstractTensorMap<ScalarType>, PACK(typename, typename), PACK(ScalarType, KeyEnum))
