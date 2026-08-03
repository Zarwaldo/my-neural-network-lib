#pragma once

#include <tensor/TensorIndex.h>

#include <tensor/api.h>

#include <stdexcept>

template <size_t Dimension>
HOST DEVICE
TensorIndex<Dimension>::TensorIndex()
    : m_rawTensorIndex()
{}

template <size_t Dimension>
HOST DEVICE
TensorIndex<Dimension>::TensorIndex(bool isValid)
    : m_rawTensorIndex(isValid)
{}

template <size_t Dimension>
HOST DEVICE
TensorIndex<Dimension>::TensorIndex(const size_t values[Max<Dimension, 1>])
    : m_rawTensorIndex(values)
{}

template <size_t Dimension>
HOST DEVICE
TensorIndex<Dimension>::TensorIndex(const std::initializer_list<size_t>& list)
    : m_rawTensorIndex(list)
{}

template <size_t Dimension>
HOST DEVICE
TensorIndex<Dimension>::TensorIndex(const AbstractTensorIndex& other)
    : m_rawTensorIndex(static_cast<const TensorIndex<Dimension>&>(other).m_rawTensorIndex)
{}

template <size_t Dimension>
HOST DEVICE
TensorIndex<Dimension>::TensorIndex(const RawTensorIndex<Dimension>& rawTensorIndex)
    : m_rawTensorIndex(rawTensorIndex)
{}

template <size_t Dimension>
HOST DEVICE
TensorIndex<Dimension>::~TensorIndex()
{}

template <size_t Dimension>
HOST DEVICE
const RawTensorIndex<Dimension>&
TensorIndex<Dimension>::getRawTensorIndex() const
{
    return m_rawTensorIndex;
}

template <size_t Dimension>
HOST DEVICE
RawTensorIndex<Dimension>&
TensorIndex<Dimension>::getRawTensorIndex()
{
    return const_cast<RawTensorIndex<Dimension>&>(static_cast<const TensorIndex<Dimension>*>(this)->getRawTensorIndex());
}

template <size_t Dimension>
HOST DEVICE
size_t
TensorIndex<Dimension>::dim() const
{
    return m_rawTensorIndex.dim();
}

template <size_t Dimension>
HOST DEVICE
bool
TensorIndex<Dimension>::isValid() const
{
    return m_rawTensorIndex.isValid();
}

template <size_t Dimension>
HOST DEVICE
void
TensorIndex<Dimension>::setValid(bool valid)
{
    m_rawTensorIndex.setValid(valid);
}

template <size_t Dimension>
HOST DEVICE
AbstractTensorIndex&
TensorIndex<Dimension>::operator=(const AbstractTensorIndex& other)
{
    m_rawTensorIndex = static_cast<const TensorIndex<Dimension>&>(other).m_rawTensorIndex;
    return *this;
}

template <size_t Dimension>
HOST DEVICE
TensorIndex<Dimension>
TensorIndex<Dimension>::operator+(const AbstractTensorIndex& other) const
{
    return TensorIndex(m_rawTensorIndex + static_cast<const TensorIndex<Dimension>&>(other).m_rawTensorIndex);
}

template <size_t Dimension>
HOST DEVICE
AbstractTensorIndex&
TensorIndex<Dimension>::operator+=(const AbstractTensorIndex& other)
{
    m_rawTensorIndex += static_cast<const TensorIndex<Dimension>&>(other).m_rawTensorIndex;
    return *this;
}

template <size_t Dimension>
HOST DEVICE
TensorIndex<Dimension>
TensorIndex<Dimension>::operator-(const AbstractTensorIndex& other) const
{
    return TensorIndex(m_rawTensorIndex - static_cast<const TensorIndex<Dimension>&>(other).m_rawTensorIndex);
}

template <size_t Dimension>
HOST DEVICE
AbstractTensorIndex&
TensorIndex<Dimension>::operator-=(const AbstractTensorIndex& other)
{
    m_rawTensorIndex -= static_cast<const TensorIndex<Dimension>&>(other).m_rawTensorIndex;
    return *this;
}

template <size_t Dimension>
HOST DEVICE
size_t
TensorIndex<Dimension>::dot(const AbstractTensorIndex& other) const
{
    return m_rawTensorIndex.dot(static_cast<const TensorIndex<Dimension>&>(other).m_rawTensorIndex);
}

template <size_t Dimension>
HOST DEVICE
size_t
TensorIndex<Dimension>::nbInferiorIndices() const
{
    return m_rawTensorIndex.nbInferiorIndices();
}

template <size_t Dimension>
HOST DEVICE
bool
TensorIndex<Dimension>::operator==(const AbstractTensorIndex& other) const
{
    return m_rawTensorIndex == static_cast<const TensorIndex<Dimension>&>(other).m_rawTensorIndex;
}

template <size_t Dimension>
HOST DEVICE
bool
TensorIndex<Dimension>::operator!=(const AbstractTensorIndex& other) const
{
    return m_rawTensorIndex != static_cast<const TensorIndex<Dimension>&>(other).m_rawTensorIndex;
}

template <size_t Dimension>
HOST DEVICE
bool
TensorIndex<Dimension>::operator>=(const AbstractTensorIndex& other) const
{
    return m_rawTensorIndex >= static_cast<const TensorIndex<Dimension>&>(other).m_rawTensorIndex;
}

template <size_t Dimension>
HOST DEVICE
bool
TensorIndex<Dimension>::operator>(const AbstractTensorIndex& other) const
{
    return m_rawTensorIndex > static_cast<const TensorIndex<Dimension>&>(other).m_rawTensorIndex;
}

template <size_t Dimension>
HOST DEVICE
bool
TensorIndex<Dimension>::operator<=(const AbstractTensorIndex& other) const
{
    return m_rawTensorIndex <= static_cast<const TensorIndex<Dimension>&>(other).m_rawTensorIndex;
}

template <size_t Dimension>
HOST DEVICE
bool
TensorIndex<Dimension>::operator<(const AbstractTensorIndex& other) const
{
    return m_rawTensorIndex < static_cast<const TensorIndex<Dimension>&>(other).m_rawTensorIndex;
}

template <size_t Dimension>
HOST DEVICE
const size_t&
TensorIndex<Dimension>::operator[](const size_t index) const
{
    return m_rawTensorIndex[index];
}

template <size_t Dimension>
HOST DEVICE
size_t&
TensorIndex<Dimension>::operator[](const size_t index)
{
    return const_cast<size_t&>(static_cast<const TensorIndex<Dimension>&>(*this)[index]);
}

template <size_t Dimension>
HOST DEVICE
const size_t*
TensorIndex<Dimension>::begin() const
{
    return m_rawTensorIndex.begin();
}

template <size_t Dimension>
HOST DEVICE
const size_t*
TensorIndex<Dimension>::end() const
{
    return m_rawTensorIndex.end();
}

template <size_t Dimension>
HOST DEVICE
size_t*
TensorIndex<Dimension>::begin()
{
    return const_cast<size_t*>(static_cast<const TensorIndex<Dimension>*>(this)->begin());
}

template <size_t Dimension>
HOST DEVICE
size_t*
TensorIndex<Dimension>::end()
{
    return const_cast<size_t*>(static_cast<const TensorIndex<Dimension>*>(this)->end());
}

IMPLEMENT_RTTI(TensorIndex, AbstractTensorIndex, PACK(size_t), PACK(Dimension))

template class MY_NEURAL_NETWORK_LIB__TENSOR__API TensorIndex<0>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API TensorIndex<1>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API TensorIndex<2>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API TensorIndex<3>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API TensorIndex<4>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API TensorIndex<5>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API TensorIndex<6>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API TensorIndex<7>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API TensorIndex<8>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__API TensorIndex<9>;
