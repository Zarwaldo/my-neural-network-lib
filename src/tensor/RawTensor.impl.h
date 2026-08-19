#pragma once

#include <tensor/RawTensor.h>

template <typename ScalarType, size_t Dimension>
HOST DEVICE
RawTensor<ScalarType, Dimension>::RawTensor(const RawTensorIndex<Dimension>& sizes, const RawTensorIndex<Dimension>& strides, ScalarType* const data)
    : m_sizes(sizes)
    , m_strides(strides)
    , m_data(data)
{}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
RawTensor<ScalarType, Dimension>::RawTensor(const RawTensor<ScalarType, Dimension>& other)
    : m_sizes(other.m_sizes)
    , m_strides(other.m_strides)
    , m_data(other.m_data)
{}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
RawTensor<ScalarType, Dimension>::~RawTensor()
{}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
RawTensor<ScalarType, Dimension>&
RawTensor<ScalarType, Dimension>::operator=(const RawTensor<ScalarType, Dimension>& other)
{
    m_sizes = other.m_sizes;
    m_strides = other.m_strides;
    m_data = other.m_data;

    return *this;
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
size_t
RawTensor<ScalarType, Dimension>::dim() const
{
    return Dimension;
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
const RawTensorIndex<Dimension>&
RawTensor<ScalarType, Dimension>::sizes() const
{
    return m_sizes;
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
size_t
RawTensor<ScalarType, Dimension>::nbElements() const
{
    return m_sizes.nbInferiorIndices();
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
size_t
RawTensor<ScalarType, Dimension>::memorySize() const
{
    return sizeof(ScalarType) * nbElements();
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
const ScalarType&
RawTensor<ScalarType, Dimension>::operator*() const
{
    return *m_data;
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
ScalarType&
RawTensor<ScalarType, Dimension>::operator*()
{
    return const_cast<ScalarType&>(*static_cast<const Tensor<ScalarType, Dimension>&>(*this));
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
const RawTensor<ScalarType, Dimension>
RawTensor<ScalarType, Dimension>::subtensor(const RawTensorIndex<Dimension>& minIndex, const RawTensorIndex<Dimension>& maxIndex) const
{
    return RawTensor<ScalarType, Dimension>(maxIndex - minIndex, m_strides, ptrAtIndex(minIndex));
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
RawTensor<ScalarType, Dimension>
RawTensor<ScalarType, Dimension>::subtensor(const RawTensorIndex<Dimension>& minIndex, const RawTensorIndex<Dimension>& maxIndex)
{
    return static_cast<const RawTensor<ScalarType, Dimension>*>(this)->subtensor(minIndex, maxIndex);
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
const ScalarType&
RawTensor<ScalarType, Dimension>::operator[](const RawTensorIndex<Dimension>& index) const
{
    return *ptrAtIndex(index);
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
ScalarType&
RawTensor<ScalarType, Dimension>::operator[](const RawTensorIndex<Dimension>& index)
{
    return const_cast<ScalarType&>(static_cast<const RawTensor<ScalarType, Dimension>&>(*this)[index]);
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
RawTensor<ScalarType, Dimension>::Iterator::Iterator(const RawTensorIndex<Dimension>& index, const RawTensorIndex<Dimension>& sizes)
    : m_index(index)
    , m_sizes(sizes)
{}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
RawTensor<ScalarType, Dimension>::Iterator::~Iterator()
{}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
const RawTensorIndex<Dimension>&
RawTensor<ScalarType, Dimension>::Iterator::operator*() const
{
    return m_index;
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
typename RawTensor<ScalarType, Dimension>::Iterator&
RawTensor<ScalarType, Dimension>::Iterator::operator++()
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

template <typename ScalarType, size_t Dimension>
HOST DEVICE
bool
RawTensor<ScalarType, Dimension>::Iterator::operator==(const RawTensor<ScalarType, Dimension>::Iterator& other) const
{
    return (m_index == other.m_index) && (m_sizes == other.m_sizes);
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
bool
RawTensor<ScalarType, Dimension>::Iterator::operator!=(const RawTensor<ScalarType, Dimension>::Iterator& other) const
{
    return !(*this == other);
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
typename RawTensor<ScalarType, Dimension>::Iterator
RawTensor<ScalarType, Dimension>::iterator(const RawTensorIndex<Dimension>& index) const
{
    return RawTensor<ScalarType, Dimension>::Iterator(index, m_sizes);
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
typename RawTensor<ScalarType, Dimension>::Iterator
RawTensor<ScalarType, Dimension>::begin() const
{
    if (nbElements() == 0)
        return end();

    size_t array[Max<Dimension, 1>];
    for (size_t i = 0; i < Dimension; ++i)
        array[i] = 0;

    return iterator(RawTensorIndex<Dimension>(array));
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
typename RawTensor<ScalarType, Dimension>::Iterator
RawTensor<ScalarType, Dimension>::end() const
{
    return iterator(RawTensorIndex<Dimension>());
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
const ScalarType*
RawTensor<ScalarType, Dimension>::getDataPtr() const
{
    return m_data;
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
ScalarType*
RawTensor<ScalarType, Dimension>::getDataPtr()
{
    return const_cast<ScalarType*>(static_cast<const RawTensor<ScalarType, Dimension>*>(this)->getDataPtr());
}

template <typename ScalarType, size_t Dimension>
HOST DEVICE
ScalarType*
RawTensor<ScalarType, Dimension>::ptrAtIndex(const RawTensorIndex<Dimension>& index) const
{
    return m_data + index.dot(m_strides);
}
