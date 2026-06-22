#pragma once

#include <tensor/TensorIndex.h>

#include <stdexcept>

template <size_t Dimension>
HOST DEVICE
RawTensorIndex<Dimension>::RawTensorIndex()
    : m_isValid(false)
{}

template <size_t Dimension>
HOST DEVICE
RawTensorIndex<Dimension>::RawTensorIndex(const size_t values[Max<Dimension, 1>])
    : m_isValid(true)
{
    if constexpr (Dimension > 0)
    {
        for (size_t i = 0; i < Dimension; ++i)
        {
            m_values[i] = values[i];
        }
    }
}

template <size_t Dimension>
HOST DEVICE
RawTensorIndex<Dimension>::RawTensorIndex(const std::initializer_list<size_t>& list)
    : RawTensorIndex<Dimension>(list.begin())
{
    if (list.size() != Dimension)
    {
        m_isValid = false;
    }
}

template <size_t Dimension>
HOST DEVICE
RawTensorIndex<Dimension>::RawTensorIndex(const RawTensorIndex<Dimension>& other)
    : m_isValid(other.m_isValid)
{
    if constexpr (Dimension > 0)
    {
        for (size_t i = 0; i < Dimension; ++i)
        {
            m_values[i] = other.m_values[i];
        }
    }
}

template <size_t Dimension>
HOST DEVICE
RawTensorIndex<Dimension>::~RawTensorIndex()
{}

template <size_t Dimension>
HOST DEVICE
size_t
RawTensorIndex<Dimension>::dim() const
{
    return Dimension;
}

template <size_t Dimension>
HOST DEVICE
bool
RawTensorIndex<Dimension>::isValid() const
{
    return m_isValid;
}

template <size_t Dimension>
HOST DEVICE
void
RawTensorIndex<Dimension>::setValid(bool isValid)
{
    m_isValid = isValid;
}

template <size_t Dimension>
HOST DEVICE
RawTensorIndex<Dimension>&
RawTensorIndex<Dimension>::operator=(const RawTensorIndex<Dimension>& other)
{
    m_isValid = static_cast<const RawTensorIndex<Dimension>&>(other).m_isValid;
    if constexpr (Dimension > 0)
    {
        for (size_t i = 0; i < Dimension; ++i)
        {
            m_values[i] = other.m_values[i];
        }
    }
    return *this;
}

template <size_t Dimension>
HOST DEVICE
RawTensorIndex<Dimension>
RawTensorIndex<Dimension>::operator+(const RawTensorIndex<Dimension>& other) const
{
    if (!isValid() || !other.isValid())
    {
        return RawTensorIndex<Dimension>();
    }

    size_t result[Dimension];
    if constexpr (Dimension > 0)
    {
        for (size_t i = 0; i < Dimension; ++i)
        {
            result[i] = m_values[i] + other.m_values[i];
        }
    }

    return RawTensorIndex<Dimension>(result);
}

template <size_t Dimension>
HOST DEVICE
RawTensorIndex<Dimension>&
RawTensorIndex<Dimension>::operator+=(const RawTensorIndex<Dimension>& other)
{
    if (!isValid() || !other.isValid())
    {
        m_isValid = false;
        return *this;
    }

    if constexpr (Dimension > 0)
    {
        for (size_t i = 0; i < Dimension; ++i)
        {
            m_values[i] += other.m_values[i];
        }
    }

    return *this;
}

template <size_t Dimension>
HOST DEVICE
RawTensorIndex<Dimension>
RawTensorIndex<Dimension>::operator-(const RawTensorIndex<Dimension>& other) const
{
    if (!isValid() || !other.isValid())
    {
        return RawTensorIndex<Dimension>();
    }

    if (!(*this >= other))
    {
        return RawTensorIndex<Dimension>();
    }

    size_t result[Dimension];
    if constexpr (Dimension > 0)
    {
        for (size_t i = 0; i < Dimension; ++i)
        {
            result[i] = m_values[i] - other.m_values[i];
        }
    }

    return RawTensorIndex<Dimension>(result);
}

template <size_t Dimension>
HOST DEVICE
RawTensorIndex<Dimension>&
RawTensorIndex<Dimension>::operator-=(const RawTensorIndex<Dimension>& other)
{
    if (!isValid() || !other.isValid())
    {
        m_isValid = false;
        return *this;
    }

    if (!(*this >= other))
    {
        m_isValid = false;
        return *this;
    }

    if constexpr (Dimension > 0)
    {
        for (size_t i = 0; i < Dimension; ++i)
        {
            m_values[i] -= other.m_values[i];
        }
    }

    return *this;
}

template <size_t Dimension>
template <size_t OtherDimension>
HOST DEVICE
RawTensorIndex<Dimension + OtherDimension>
RawTensorIndex<Dimension>::operator*(const RawTensorIndex<OtherDimension>& other) const
{
    RawTensorIndex<Dimension + OtherDimension> result;

    if constexpr (Dimension > 0)
    {
        for (size_t i = 0; i < Dimension; ++i)
        {
            result[i] = (*this)[i];
        }
    }

    if constexpr (OtherDimension > 0)
    {
        for (size_t i = 0; i < OtherDimension; ++i)
        {
            result[Dimension + i] = other[i];
        }
    }

    result.setValid(isValid() && other.isValid());

    return result;
}

template <size_t Dimension>
HOST DEVICE
size_t
RawTensorIndex<Dimension>::dot(const RawTensorIndex<Dimension>& other) const
{
    if (!isValid() || !other.isValid())
    {
        return static_cast<size_t>(-1);
    }

    size_t result = 0;
    if constexpr (Dimension > 0)
    {
        for (size_t i = 0; i < Dimension; ++i)
        {
            result += m_values[i] * other.m_values[i];
        }
    }

    return result;
}

template <size_t Dimension>
HOST DEVICE
size_t
RawTensorIndex<Dimension>::nbInferiorIndices() const
{
    if (!isValid())
    {
        return static_cast<size_t>(-1);
    }

    size_t result = 1;
    if constexpr (Dimension > 0)
    {
        for (size_t i = 0; i < Dimension; ++i)
        {
            result *= m_values[i];
        }
    }
    return result;
}

template <size_t Dimension>
HOST DEVICE
bool
RawTensorIndex<Dimension>::operator==(const RawTensorIndex<Dimension>& other) const
{
    if (!isValid() && !other.isValid())
    {
        return true;
    }

    if (!isValid() || !other.isValid())
    {
        return false;
    }

    if constexpr (Dimension > 0)
    {
        for (size_t i = 0; i < Dimension; ++i)
        {
            if (m_values[i] != other.m_values[i])
            {
                return false;
            }
        }
    }

    return true;
}

template <size_t Dimension>
HOST DEVICE
bool
RawTensorIndex<Dimension>::operator!=(const RawTensorIndex<Dimension>& other) const
{
    return !(*this == other);
}

template <size_t Dimension>
HOST DEVICE
bool
RawTensorIndex<Dimension>::operator>=(const RawTensorIndex<Dimension>& other) const
{
    if (!isValid() || !other.isValid())
    {
        return false;
    }

    if constexpr (Dimension > 0)
    {
        for (size_t i = 0; i < Dimension; ++i)
        {
            if (m_values[i] < other.m_values[i])
            {
                return false;
            }
        }
    }

    return true;
}

template <size_t Dimension>
HOST DEVICE
bool
RawTensorIndex<Dimension>::operator>(const RawTensorIndex<Dimension>& other) const
{
    if (!isValid() || !other.isValid())
    {
        return false;
    }

    if constexpr (Dimension > 0)
    {
        for (size_t i = 0; i < Dimension; ++i)
        {
            if (m_values[i] <= other.m_values[i])
            {
                return false;
            }
        }
    }

    return true;
}

template <size_t Dimension>
HOST DEVICE
bool
RawTensorIndex<Dimension>::operator<=(const RawTensorIndex<Dimension>& other) const
{
    if (!isValid() || !other.isValid())
    {
        return false;
    }

    return other >= *this;
}

template <size_t Dimension>
HOST DEVICE
bool
RawTensorIndex<Dimension>::operator<(const RawTensorIndex<Dimension>& other) const
{
    if (!isValid() || !other.isValid())
    {
        return false;
    }

    return other > *this;
}

template <size_t Dimension>
HOST DEVICE
const size_t&
RawTensorIndex<Dimension>::operator[](const size_t index) const
{
    return m_values[index];
}

template <size_t Dimension>
HOST DEVICE
size_t&
RawTensorIndex<Dimension>::operator[](const size_t index)
{
    return const_cast<size_t&>(static_cast<const RawTensorIndex<Dimension>&>(*this)[index]);
}

template <long long int Dimension, long long int From, long long int To>
struct DimensionOfRange<Dimension, From, To, std::enable_if_t<(Dimension >= 0) && (0 <= From) && (From < Dimension) && (0 <= To) && (To < Dimension) && (From <= To)>>
{
    static constexpr size_t result = To - From + 1;
};

template <long long int Dimension, long long int From, long long int To>
struct DimensionOfRange<Dimension, From, To, std::enable_if_t<(Dimension >= 0) && (0 <= From) && (From < Dimension) && (0 <= To) && (To < Dimension) && (From > To)>>
{
    static constexpr size_t result = 0;
};

template <long long int Dimension, long long int From, long long int To>
struct DimensionOfRange<Dimension, From, To, std::enable_if_t<(Dimension >= 0) && (0 <= From) && (From < Dimension) && (To < 0)>>
{
    static constexpr size_t result = DimensionOfRange<Dimension, From, To + Dimension>::result;
};

template <long long int Dimension, long long int From, long long int To>
struct DimensionOfRange<Dimension, From, To, std::enable_if_t<(Dimension >= 0) && (0 <= From) && (From < Dimension) && (Dimension <= To)>>
{
    static constexpr size_t result = DimensionOfRange<Dimension, From, To - Dimension>::result;
};

template <long long int Dimension, long long int From, long long int To>
struct DimensionOfRange<Dimension, From, To, std::enable_if_t<(Dimension >= 0) && (From < 0)>>
{
    static constexpr size_t result = DimensionOfRange<Dimension, From + Dimension, To>::result;
};

template <long long int Dimension, long long int From, long long int To>
struct DimensionOfRange<Dimension, From, To, std::enable_if_t<(Dimension >= 0) && (Dimension <= From)>>
{
    static constexpr size_t result = DimensionOfRange<Dimension, From - Dimension, To>::result;
};

template <size_t Dimension>
template <long long int From, long long int To>
HOST DEVICE
RawTensorIndex<Dimension>::RawTensorIndexRange<From, To>
RawTensorIndex<Dimension>::range() const
{
    RawTensorIndexRange<From, To> result;
    for (size_t index = 0; index < result.dim(); ++index)
        result[index] = (*this)[From + index];
    result.setValid(isValid());

    return result;
}

template <size_t Dimension>
HOST DEVICE
const size_t*
RawTensorIndex<Dimension>::begin() const
{
    return m_values;
}

template <size_t Dimension>
HOST DEVICE
const size_t*
RawTensorIndex<Dimension>::end() const
{
    return m_values + Dimension;
}

template <size_t Dimension>
HOST DEVICE
size_t*
RawTensorIndex<Dimension>::begin()
{
    return m_values;
}

template <size_t Dimension>
HOST DEVICE
size_t*
RawTensorIndex<Dimension>::end()
{
    return m_values + Dimension;
}

template <size_t Dimension>
HOST DEVICE
TensorIndex<Dimension>::TensorIndex()
    : m_rawTensorIndex()
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
template <size_t OtherDimension>
HOST DEVICE
TensorIndex<Dimension + OtherDimension>
TensorIndex<Dimension>::operator*(const TensorIndex<OtherDimension>& other) const
{
    return TensorIndex<Dimension + OtherDimension>(m_rawTensorIndex * other.getRawTensorIndex());
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
template <long long int From, long long int To>
HOST DEVICE
TensorIndex<Dimension>::TensorIndexRange<From, To>
TensorIndex<Dimension>::range() const
{
    return TensorIndex<Dimension>::TensorIndexRange<From, To>(m_rawTensorIndex.range<From, To>());
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
