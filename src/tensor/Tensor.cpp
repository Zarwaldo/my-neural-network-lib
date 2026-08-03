#pragma once

#include <tensor/Tensor.h>

#include <tensor/api.h>

#include <stdexcept>

template <typename ValueType>
HOST
AbstractTensor<ValueType>::~AbstractTensor()
{}

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
    if constexpr (Dimension > 0)
    {
        strides[0] = 1;
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

template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<float, 0>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<float, 1>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<float, 2>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<float, 3>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<float, 4>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<float, 5>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<float, 6>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<float, 7>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<float, 8>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<float, 9>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<double, 0>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<double, 1>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<double, 2>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<double, 3>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<double, 4>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<double, 5>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<double, 6>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<double, 7>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<double, 8>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<double, 9>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<bool, 0>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<bool, 1>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<bool, 2>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<bool, 3>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<bool, 4>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<bool, 5>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<bool, 6>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<bool, 7>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<bool, 8>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API Tensor<bool, 9>;
