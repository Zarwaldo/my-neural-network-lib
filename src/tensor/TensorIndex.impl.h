#pragma once

#include <tensor/TensorIndex.h>

template <size_t Dimension>
template <size_t OtherDimension>
HOST DEVICE
TensorIndex<Dimension + OtherDimension>
TensorIndex<Dimension>::operator*(const TensorIndex<OtherDimension>& other) const
{
    return TensorIndex<Dimension + OtherDimension>(m_rawTensorIndex * other.getRawTensorIndex());
}

template <size_t Dimension>
template <long long int From, long long int To>
HOST DEVICE
TensorIndex<Dimension>::TensorIndexRange<From, To>
TensorIndex<Dimension>::range() const
{
    return TensorIndex<Dimension>::TensorIndexRange<From, To>(m_rawTensorIndex.range<From, To>());
}
