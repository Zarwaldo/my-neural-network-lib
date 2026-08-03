#pragma once

#include <CommonMacros.h>

#include <initializer_list>

template <size_t First, size_t Second, typename Enable>
struct MaxImpl
{};

template <size_t First, size_t Second>
struct MaxImpl<First, Second, std::enable_if_t<(First >= Second)>>
{
    static constexpr size_t result = First;
};

template <size_t First, size_t Second>
struct MaxImpl<First, Second, std::enable_if_t<(First < Second)>>
{
    static constexpr size_t result = Second;
};

template <size_t First, size_t Second>
constexpr size_t Max = MaxImpl<First, Second, void>::result;

template <long long int Dimension, long long int From, long long int To, typename Enable = void>
struct DimensionOfRange
{};

template <size_t Dimension>
class RawTensorIndex
{
public:
    HOST DEVICE inline RawTensorIndex();
    HOST DEVICE inline RawTensorIndex(bool isValid);
    HOST DEVICE inline RawTensorIndex(const size_t values[Max<Dimension, 1>]);
    HOST DEVICE inline RawTensorIndex(const std::initializer_list<size_t>& list);
    HOST DEVICE inline RawTensorIndex(const RawTensorIndex& other);

    HOST DEVICE inline ~RawTensorIndex();

    HOST DEVICE inline size_t dim() const;

    HOST DEVICE inline bool isValid() const;
    HOST DEVICE inline void setValid(bool isValid);

    HOST DEVICE inline RawTensorIndex& operator=(const RawTensorIndex& other);
    HOST DEVICE inline RawTensorIndex operator+(const RawTensorIndex& other) const;
    HOST DEVICE inline RawTensorIndex& operator+=(const RawTensorIndex& other);
    HOST DEVICE inline RawTensorIndex operator-(const RawTensorIndex& other) const;
    HOST DEVICE inline RawTensorIndex& operator-=(const RawTensorIndex& other);
    template <size_t OtherDimension>
    HOST DEVICE inline RawTensorIndex<Dimension + OtherDimension> operator*(const RawTensorIndex<OtherDimension>& other) const;

    HOST DEVICE inline size_t dot(const RawTensorIndex& other) const;
    HOST DEVICE inline size_t nbInferiorIndices() const;

    HOST DEVICE inline bool operator==(const RawTensorIndex& other) const;
    HOST DEVICE inline bool operator!=(const RawTensorIndex& other) const;
    HOST DEVICE inline bool operator>=(const RawTensorIndex& other) const;
    HOST DEVICE inline bool operator>(const RawTensorIndex& other) const;
    HOST DEVICE inline bool operator<=(const RawTensorIndex& other) const;
    HOST DEVICE inline bool operator<(const RawTensorIndex& other) const;

    HOST DEVICE inline const size_t& operator[](const size_t index) const;
    HOST DEVICE inline size_t& operator[](const size_t index);

    template <long long int From, long long int To>
    using RawTensorIndexRange = RawTensorIndex<DimensionOfRange<Dimension, From, To>::result>;
    template <long long int From, long long int To>
    HOST DEVICE inline RawTensorIndexRange<From, To> range() const;

    HOST DEVICE inline const size_t* begin() const;
    HOST DEVICE inline const size_t* end() const;
    HOST DEVICE inline size_t* begin();
    HOST DEVICE inline size_t* end();

private:
    bool m_isValid;
    size_t m_values[Max<Dimension, 1>];
};

#include <tensor/RawTensorIndex.impl.h>
