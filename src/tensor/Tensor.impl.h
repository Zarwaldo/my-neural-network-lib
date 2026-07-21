#pragma once

#include <tensor/Tensor.h>

#include <stdexcept>

template <typename ValueType>
HOST
AbstractTensor<ValueType>::~AbstractTensor()
{}

template <typename ValueType, size_t Dimension>
HOST DEVICE
RawTensor<ValueType, Dimension>::RawTensor(const RawTensorIndex<Dimension>& sizes, const RawTensorIndex<Dimension>& strides, ValueType* const data)
    : m_sizes(sizes)
    , m_strides(strides)
    , m_data(data)
{}

template <typename ValueType, size_t Dimension>
HOST DEVICE
RawTensor<ValueType, Dimension>::RawTensor(const RawTensor<ValueType, Dimension>& other)
    : m_sizes(other.m_sizes)
    , m_strides(other.m_strides)
    , m_data(other.m_data)
{}

template <typename ValueType, size_t Dimension>
HOST DEVICE
RawTensor<ValueType, Dimension>::~RawTensor()
{}

template <typename ValueType, size_t Dimension>
HOST DEVICE
RawTensor<ValueType, Dimension>&
RawTensor<ValueType, Dimension>::operator=(const RawTensor<ValueType, Dimension>& other)
{
    m_sizes = other.m_sizes;
    m_strides = other.m_strides;
    m_data = other.m_data;

    return *this;
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
size_t
RawTensor<ValueType, Dimension>::dim() const
{
    return Dimension;
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
const RawTensorIndex<Dimension>&
RawTensor<ValueType, Dimension>::sizes() const
{
    return m_sizes;
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
size_t
RawTensor<ValueType, Dimension>::nbElements() const
{
    return m_sizes.nbInferiorIndices();
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
size_t
RawTensor<ValueType, Dimension>::memorySize() const
{
    return sizeof(ValueType) * nbElements();
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
const ValueType&
RawTensor<ValueType, Dimension>::operator*() const
{
    return *m_data;
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
ValueType&
RawTensor<ValueType, Dimension>::operator*()
{
    return const_cast<ValueType&>(*static_cast<const Tensor<ValueType, Dimension>&>(*this));
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
const RawTensor<ValueType, Dimension>
RawTensor<ValueType, Dimension>::subtensor(const RawTensorIndex<Dimension>& minIndex, const RawTensorIndex<Dimension>& maxIndex) const
{
    return RawTensor<ValueType, Dimension>(maxIndex - minIndex, m_strides, ptrAtIndex(minIndex));
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
RawTensor<ValueType, Dimension>
RawTensor<ValueType, Dimension>::subtensor(const RawTensorIndex<Dimension>& minIndex, const RawTensorIndex<Dimension>& maxIndex)
{
    return static_cast<const RawTensor<ValueType, Dimension>*>(this)->subtensor(minIndex, maxIndex);
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
const ValueType&
RawTensor<ValueType, Dimension>::operator[](const RawTensorIndex<Dimension>& index) const
{
    return *ptrAtIndex(index);
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
ValueType&
RawTensor<ValueType, Dimension>::operator[](const RawTensorIndex<Dimension>& index)
{
    return const_cast<ValueType&>(static_cast<const RawTensor<ValueType, Dimension>&>(*this)[index]);
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
RawTensor<ValueType, Dimension>::Iterator::Iterator(const RawTensorIndex<Dimension>& index, const RawTensorIndex<Dimension>& sizes)
    : m_index(index)
    , m_sizes(sizes)
{}

template <typename ValueType, size_t Dimension>
HOST DEVICE
RawTensor<ValueType, Dimension>::Iterator::~Iterator()
{}

template <typename ValueType, size_t Dimension>
HOST DEVICE
const RawTensorIndex<Dimension>&
RawTensor<ValueType, Dimension>::Iterator::operator*() const
{
    return m_index;
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
typename RawTensor<ValueType, Dimension>::Iterator&
RawTensor<ValueType, Dimension>::Iterator::operator++()
{
    for (size_t dim = Dimension - 1; dim != static_cast<size_t>(-1); --dim)
    {
        if (m_index[dim] < m_sizes[dim] - 1)
        {
            m_index[dim]++;
            return *this;
        }

        m_index[dim] = 0;
    }

    m_index = RawTensorIndex<Dimension>();
    return *this;
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
bool
RawTensor<ValueType, Dimension>::Iterator::operator==(const RawTensor<ValueType, Dimension>::Iterator& other) const
{
    return (m_index == other.m_index) && (m_sizes == other.m_sizes);
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
bool
RawTensor<ValueType, Dimension>::Iterator::operator!=(const RawTensor<ValueType, Dimension>::Iterator& other) const
{
    return !(*this == other);
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
typename RawTensor<ValueType, Dimension>::Iterator
RawTensor<ValueType, Dimension>::iterator(const RawTensorIndex<Dimension>& index) const
{
    return RawTensor<ValueType, Dimension>::Iterator(index, m_sizes);
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
typename RawTensor<ValueType, Dimension>::Iterator
RawTensor<ValueType, Dimension>::begin() const
{
    if (nbElements() == 0)
        return end();

    size_t array[Dimension];
    for (size_t i = 0; i < Dimension; ++i)
        array[i] = 0;

    return iterator(RawTensorIndex<Dimension>(array));
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
typename RawTensor<ValueType, Dimension>::Iterator
RawTensor<ValueType, Dimension>::end() const
{
    return iterator(RawTensorIndex<Dimension>());
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
const ValueType*
RawTensor<ValueType, Dimension>::getDataPtr() const
{
    return m_data;
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
ValueType*
RawTensor<ValueType, Dimension>::getDataPtr()
{
    return const_cast<ValueType*>(static_cast<const RawTensor<ValueType, Dimension>*>(this)->getDataPtr());
}

template <typename ValueType, size_t Dimension>
HOST DEVICE
ValueType*
RawTensor<ValueType, Dimension>::ptrAtIndex(const RawTensorIndex<Dimension>& index) const
{
    return m_data + index.dot(m_strides);
}

template <typename ValueType, size_t Dimension>
HOST
Tensor<ValueType, Dimension>::Tensor(ManagedMemorySharedPtr<ValueType>& data, const RawTensor<ValueType, Dimension>& rawTensor)
    : m_data(data)
    , m_rawTensor(rawTensor)
    , m_sizes(TensorIndex<Dimension>(m_rawTensor.m_sizes))
{}

template <typename ValueType, size_t Dimension>
HOST
Tensor<ValueType, Dimension>::Tensor(ManagedMemorySharedPtr<ValueType>&& data, const RawTensor<ValueType, Dimension>& rawTensor)
    : m_data(std::move(data))
    , m_rawTensor(rawTensor)
    , m_sizes(TensorIndex<Dimension>(m_rawTensor.m_sizes))
{}

template <typename ValueType, size_t Dimension>
HOST
Tensor<ValueType, Dimension>::Tensor(const Tensor<ValueType, Dimension>& other)
    : m_data(other.m_data)
    , m_rawTensor(other.m_rawTensor)
    , m_sizes(other.m_sizes)
{}

template <typename ValueType, size_t Dimension>
HOST
Tensor<ValueType, Dimension>::Tensor(Tensor<ValueType, Dimension>&& other)
    : m_data(std::move(other.m_data))
    , m_rawTensor(other.m_rawTensor)
    , m_sizes(other.m_sizes)
{}

template <typename ValueType, size_t Dimension>
HOST
Tensor<ValueType, Dimension>*
Tensor<ValueType, Dimension>::create(const TensorIndex<Dimension>& sizes)
{
    size_t strides[Max<Dimension, 1>];
    strides[0] = 1;
    if constexpr (Dimension > 0)
    {
        for (size_t i = 1; i < Dimension; ++i)
        {
            strides[i] = strides[i-1] * sizes[i-1];
        }
    }

    ManagedMemorySharedPtr<ValueType> data(sizes.nbInferiorIndices());

    RawTensor<ValueType, Dimension> rawTensor(sizes.getRawTensorIndex(), RawTensorIndex<Dimension>(strides), data.getPtr());
    return new Tensor<ValueType, Dimension>(std::move(data), rawTensor);
}

template <typename ValueType, size_t Dimension>
HOST
Tensor<ValueType, Dimension>::~Tensor()
{}

template <typename ValueType, size_t Dimension>
HOST
Tensor<ValueType, Dimension>&
Tensor<ValueType, Dimension>::operator=(const Tensor<ValueType, Dimension>& other)
{
    m_data = other.m_data;
    m_rawTensor = other.m_rawTensor;
    m_sizes = other.m_sizes;

    return *this;
}

template <typename ValueType, size_t Dimension>
HOST
Tensor<ValueType, Dimension>&
Tensor<ValueType, Dimension>::operator=(Tensor<ValueType, Dimension>&& other)
{
    m_data = std::move(other.m_data);
    m_rawTensor = std::move(other.m_rawTensor);
    m_sizes = other.m_sizes;

    return *this;
}

template <typename ValueType, size_t Dimension>
const RawTensor<ValueType, Dimension>&
Tensor<ValueType, Dimension>::getRawTensor() const
{
    return m_rawTensor;
}

template <typename ValueType, size_t Dimension>
RawTensor<ValueType, Dimension>&
Tensor<ValueType, Dimension>::getRawTensor()
{
    return const_cast<RawTensor<ValueType, Dimension>&>(static_cast<const Tensor<ValueType, Dimension>*>(this)->getRawTensor());
}

template <typename ValueType, size_t Dimension>
HOST
size_t
Tensor<ValueType, Dimension>::dim() const
{
    return m_rawTensor.dim();
}

template <typename ValueType, size_t Dimension>
HOST
const AbstractTensorIndex&
Tensor<ValueType, Dimension>::sizes() const
{
    return m_sizes;
}

template <typename ValueType, size_t Dimension>
HOST
size_t
Tensor<ValueType, Dimension>::nbElements() const
{
    return m_rawTensor.nbElements();
}

template <typename ValueType, size_t Dimension>
HOST
size_t
Tensor<ValueType, Dimension>::memorySize() const
{
    return m_rawTensor.memorySize();
}

template <typename ValueType, size_t Dimension>
HOST
const ValueType&
Tensor<ValueType, Dimension>::operator*() const
{
    return *m_rawTensor;
}

template <typename ValueType, size_t Dimension>
HOST
ValueType&
Tensor<ValueType, Dimension>::operator*()
{
    return const_cast<ValueType&>(*static_cast<const Tensor<ValueType, Dimension>&>(*this));
}

template <typename ValueType, size_t Dimension>
HOST
const Tensor<ValueType, Dimension>
Tensor<ValueType, Dimension>::subtensor(const TensorIndex<Dimension>& minIndex, const TensorIndex<Dimension>& maxIndex) const
{
    const RawTensorIndex<Dimension>& rawMinIndex = static_cast<const TensorIndex<Dimension>&>(minIndex).getRawTensorIndex();
    const RawTensorIndex<Dimension>& rawMaxIndex = static_cast<const TensorIndex<Dimension>&>(maxIndex).getRawTensorIndex();
    return Tensor<ValueType, Dimension>(const_cast<ManagedMemorySharedPtr<ValueType>&>(m_data), m_rawTensor.subtensor(rawMinIndex, rawMaxIndex));
}

template <typename ValueType, size_t Dimension>
HOST
Tensor<ValueType, Dimension>
Tensor<ValueType, Dimension>::subtensor(const TensorIndex<Dimension>& minIndex, const TensorIndex<Dimension>& maxIndex)
{
    return static_cast<const Tensor<ValueType, Dimension>*>(this)->subtensor(minIndex, maxIndex);
}

template <typename ValueType, size_t Dimension>
HOST
const ValueType&
Tensor<ValueType, Dimension>::operator[](const AbstractTensorIndex& index) const
{
    return m_rawTensor[static_cast<const TensorIndex<Dimension>&>(index).getRawTensorIndex()];
}

template <typename ValueType, size_t Dimension>
HOST
ValueType&
Tensor<ValueType, Dimension>::operator[](const AbstractTensorIndex& index)
{
    return const_cast<ValueType&>(static_cast<const Tensor<ValueType, Dimension>&>(*this)[index]);
}

template <typename ValueType, size_t Dimension>
HOST
typename RawTensor<ValueType, Dimension>::Iterator
Tensor<ValueType, Dimension>::iterator(const TensorIndex<Dimension>& index) const
{
    return m_rawTensor.iterator(static_cast<const TensorIndex<Dimension>&>(index).getRawTensorIndex());
}

template <typename ValueType, size_t Dimension>
HOST
typename RawTensor<ValueType, Dimension>::Iterator
Tensor<ValueType, Dimension>::begin() const
{
    return m_rawTensor.begin();
}

template <typename ValueType, size_t Dimension>
HOST
typename RawTensor<ValueType, Dimension>::Iterator
Tensor<ValueType, Dimension>::end() const
{
    return m_rawTensor.end();
}

IMPLEMENT_RTTI(Tensor, TensorBase, PACK(typename, size_t), PACK(ValueType, Dimension))
