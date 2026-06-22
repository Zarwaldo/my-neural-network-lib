#pragma once

#include <tensor/AbstractTensorIndex.h>

#include <CommonMacros.h>

#include <rtti/AbstractRtti.h>
#include <rtti/Rtti.h>

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

template <size_t Dimension>
class TensorIndex : public AbstractTensorIndex
{
public:
    HOST DEVICE inline TensorIndex();
    HOST DEVICE inline TensorIndex(const size_t values[Max<Dimension, 1>]);
    HOST DEVICE inline TensorIndex(const std::initializer_list<size_t>& list);
    HOST DEVICE inline TensorIndex(const AbstractTensorIndex& other);
    HOST DEVICE inline TensorIndex(const RawTensorIndex<Dimension>& rawTensorIndex);

    HOST DEVICE virtual inline ~TensorIndex() override;

    HOST DEVICE inline const RawTensorIndex<Dimension>& getRawTensorIndex() const;
    HOST DEVICE inline RawTensorIndex<Dimension>& getRawTensorIndex();

    HOST DEVICE virtual inline size_t dim() const override;

    HOST DEVICE virtual inline bool isValid() const override;
    HOST DEVICE virtual inline void setValid(bool valid) override;

    HOST DEVICE inline AbstractTensorIndex& operator=(const AbstractTensorIndex& other);
    HOST DEVICE inline TensorIndex operator+(const AbstractTensorIndex& other) const;
    HOST DEVICE virtual inline AbstractTensorIndex& operator+=(const AbstractTensorIndex& other) override;
    HOST DEVICE inline TensorIndex operator-(const AbstractTensorIndex& other) const;
    HOST DEVICE virtual inline AbstractTensorIndex& operator-=(const AbstractTensorIndex& other) override;
    template <size_t OtherDimension>
    HOST DEVICE inline TensorIndex<Dimension + OtherDimension> operator*(const TensorIndex<OtherDimension>& other) const;

    HOST DEVICE virtual inline size_t dot(const AbstractTensorIndex& other) const override;
    HOST DEVICE virtual inline size_t nbInferiorIndices() const override;

    HOST DEVICE virtual inline bool operator==(const AbstractTensorIndex& other) const override;
    HOST DEVICE virtual inline bool operator!=(const AbstractTensorIndex& other) const override;
    HOST DEVICE virtual inline bool operator>=(const AbstractTensorIndex& other) const override;
    HOST DEVICE virtual inline bool operator>(const AbstractTensorIndex& other) const override;
    HOST DEVICE virtual inline bool operator<=(const AbstractTensorIndex& other) const override;
    HOST DEVICE virtual inline bool operator<(const AbstractTensorIndex& other) const override;

    HOST DEVICE virtual inline const size_t& operator[](const size_t index) const override;
    HOST DEVICE virtual inline size_t& operator[](const size_t index) override;

    template <long long int From, long long int To>
    using TensorIndexRange = TensorIndex<DimensionOfRange<Dimension, From, To>::result>;
    template <long long int From, long long int To>
    HOST DEVICE inline TensorIndexRange<From, To> range() const;

    HOST DEVICE virtual inline const size_t* begin() const override;
    HOST DEVICE virtual inline const size_t* end() const override;
    HOST DEVICE virtual inline size_t* begin() override;
    HOST DEVICE virtual inline size_t* end() override;

    DECLARE_RTTI(AbstractTensorIndex)

private:
    RawTensorIndex<Dimension> m_rawTensorIndex;
};

#include <tensor/TensorIndex.impl.h>