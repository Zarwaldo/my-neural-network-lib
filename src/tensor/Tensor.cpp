#pragma once

#include <tensor/Tensor.h>

#include <tensor/api.h>

#include <stdexcept>

template <typename ScalarType>
HOST
AbstractTensor<ScalarType>::~AbstractTensor()
{}

template <typename ScalarType, size_t Dimension>
HOST
Tensor<ScalarType, Dimension>::Tensor(ManagedMemorySharedPtr<ScalarType>& data, const RawTensor<ScalarType, Dimension>& rawTensor)
    : m_data(data)
    , m_rawTensor(rawTensor)
    , m_sizes(TensorIndex<Dimension>(m_rawTensor.m_sizes))
{}

template <typename ScalarType, size_t Dimension>
HOST
Tensor<ScalarType, Dimension>::Tensor(ManagedMemorySharedPtr<ScalarType>&& data, const RawTensor<ScalarType, Dimension>& rawTensor)
    : m_data(std::move(data))
    , m_rawTensor(rawTensor)
    , m_sizes(TensorIndex<Dimension>(m_rawTensor.m_sizes))
{}

template <typename ScalarType, size_t Dimension>
HOST
Tensor<ScalarType, Dimension>::Tensor(const Tensor<ScalarType, Dimension>& other)
    : m_data(other.m_data)
    , m_rawTensor(other.m_rawTensor)
    , m_sizes(other.m_sizes)
{}

template <typename ScalarType, size_t Dimension>
HOST
Tensor<ScalarType, Dimension>::Tensor(Tensor<ScalarType, Dimension>&& other)
    : m_data(std::move(other.m_data))
    , m_rawTensor(other.m_rawTensor)
    , m_sizes(other.m_sizes)
{}

template <typename ScalarType, size_t Dimension>
HOST
Tensor<ScalarType, Dimension>*
Tensor<ScalarType, Dimension>::create(const TensorIndex<Dimension>& sizes)
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

    ManagedMemorySharedPtr<ScalarType> data(sizes.nbInferiorIndices());

    RawTensor<ScalarType, Dimension> rawTensor(sizes.getRawTensorIndex(), RawTensorIndex<Dimension>(strides), data.getPtr());
    return new Tensor<ScalarType, Dimension>(std::move(data), rawTensor);
}

template <typename ScalarType, size_t Dimension>
HOST
Tensor<ScalarType, Dimension>::~Tensor()
{}

template <typename ScalarType, size_t Dimension>
HOST
Tensor<ScalarType, Dimension>&
Tensor<ScalarType, Dimension>::operator=(const Tensor<ScalarType, Dimension>& other)
{
    m_data = other.m_data;
    m_rawTensor = other.m_rawTensor;
    m_sizes = other.m_sizes;

    return *this;
}

template <typename ScalarType, size_t Dimension>
HOST
Tensor<ScalarType, Dimension>&
Tensor<ScalarType, Dimension>::operator=(Tensor<ScalarType, Dimension>&& other)
{
    m_data = std::move(other.m_data);
    m_rawTensor = std::move(other.m_rawTensor);
    m_sizes = other.m_sizes;

    return *this;
}

template <typename ScalarType, size_t Dimension>
const RawTensor<ScalarType, Dimension>&
Tensor<ScalarType, Dimension>::getRawTensor() const
{
    return m_rawTensor;
}

template <typename ScalarType, size_t Dimension>
RawTensor<ScalarType, Dimension>&
Tensor<ScalarType, Dimension>::getRawTensor()
{
    return const_cast<RawTensor<ScalarType, Dimension>&>(static_cast<const Tensor<ScalarType, Dimension>*>(this)->getRawTensor());
}

template <typename ScalarType, size_t Dimension>
HOST
size_t
Tensor<ScalarType, Dimension>::dim() const
{
    return m_rawTensor.dim();
}

template <typename ScalarType, size_t Dimension>
HOST
const AbstractTensorIndex&
Tensor<ScalarType, Dimension>::sizes() const
{
    return m_sizes;
}

template <typename ScalarType, size_t Dimension>
HOST
size_t
Tensor<ScalarType, Dimension>::nbElements() const
{
    return m_rawTensor.nbElements();
}

template <typename ScalarType, size_t Dimension>
HOST
size_t
Tensor<ScalarType, Dimension>::memorySize() const
{
    return m_rawTensor.memorySize();
}

template <typename ScalarType, size_t Dimension>
HOST
const ScalarType&
Tensor<ScalarType, Dimension>::operator*() const
{
    return *m_rawTensor;
}

template <typename ScalarType, size_t Dimension>
HOST
ScalarType&
Tensor<ScalarType, Dimension>::operator*()
{
    return const_cast<ScalarType&>(*static_cast<const Tensor<ScalarType, Dimension>&>(*this));
}

template <typename ScalarType, size_t Dimension>
HOST
const Tensor<ScalarType, Dimension>
Tensor<ScalarType, Dimension>::subtensor(const TensorIndex<Dimension>& minIndex, const TensorIndex<Dimension>& maxIndex) const
{
    const RawTensorIndex<Dimension>& rawMinIndex = static_cast<const TensorIndex<Dimension>&>(minIndex).getRawTensorIndex();
    const RawTensorIndex<Dimension>& rawMaxIndex = static_cast<const TensorIndex<Dimension>&>(maxIndex).getRawTensorIndex();
    return Tensor<ScalarType, Dimension>(const_cast<ManagedMemorySharedPtr<ScalarType>&>(m_data), m_rawTensor.subtensor(rawMinIndex, rawMaxIndex));
}

template <typename ScalarType, size_t Dimension>
HOST
Tensor<ScalarType, Dimension>
Tensor<ScalarType, Dimension>::subtensor(const TensorIndex<Dimension>& minIndex, const TensorIndex<Dimension>& maxIndex)
{
    return static_cast<const Tensor<ScalarType, Dimension>*>(this)->subtensor(minIndex, maxIndex);
}

template <typename ScalarType, size_t Dimension>
HOST
const ScalarType&
Tensor<ScalarType, Dimension>::operator[](const AbstractTensorIndex& index) const
{
    return m_rawTensor[static_cast<const TensorIndex<Dimension>&>(index).getRawTensorIndex()];
}

template <typename ScalarType, size_t Dimension>
HOST
ScalarType&
Tensor<ScalarType, Dimension>::operator[](const AbstractTensorIndex& index)
{
    return const_cast<ScalarType&>(static_cast<const Tensor<ScalarType, Dimension>&>(*this)[index]);
}

template <typename ScalarType, size_t Dimension>
HOST
typename RawTensor<ScalarType, Dimension>::Iterator
Tensor<ScalarType, Dimension>::iterator(const TensorIndex<Dimension>& index) const
{
    return m_rawTensor.iterator(static_cast<const TensorIndex<Dimension>&>(index).getRawTensorIndex());
}

template <typename ScalarType, size_t Dimension>
HOST
typename RawTensor<ScalarType, Dimension>::Iterator
Tensor<ScalarType, Dimension>::begin() const
{
    return m_rawTensor.begin();
}

template <typename ScalarType, size_t Dimension>
HOST
typename RawTensor<ScalarType, Dimension>::Iterator
Tensor<ScalarType, Dimension>::end() const
{
    return m_rawTensor.end();
}

IMPLEMENT_RTTI(Tensor, TensorBase, PACK(typename, size_t), PACK(ScalarType, Dimension))

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
