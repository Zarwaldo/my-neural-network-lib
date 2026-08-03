#pragma once

#include <tensor/AbstractTensorIndex.h>

#include <rtti/AbstractRtti.h>
#include <rtti/Rtti.h>

#include <tensor/RawTensorIndex.h>

#include <initializer_list>

template <size_t Dimension>
class TensorIndex : public AbstractTensorIndex
{
public:
    HOST DEVICE TensorIndex();
    HOST DEVICE TensorIndex(bool isValid);
    HOST DEVICE TensorIndex(const size_t values[Max<Dimension, 1>]);
    HOST DEVICE TensorIndex(const std::initializer_list<size_t>& list);
    HOST DEVICE TensorIndex(const AbstractTensorIndex& other);
    HOST DEVICE TensorIndex(const RawTensorIndex<Dimension>& rawTensorIndex);

    HOST DEVICE virtual ~TensorIndex() override;

    HOST DEVICE const RawTensorIndex<Dimension>& getRawTensorIndex() const;
    HOST DEVICE RawTensorIndex<Dimension>& getRawTensorIndex();

    HOST DEVICE virtual size_t dim() const override;

    HOST DEVICE virtual bool isValid() const override;
    HOST DEVICE virtual void setValid(bool valid) override;

    HOST DEVICE AbstractTensorIndex& operator=(const AbstractTensorIndex& other);
    HOST DEVICE TensorIndex operator+(const AbstractTensorIndex& other) const;
    HOST DEVICE virtual AbstractTensorIndex& operator+=(const AbstractTensorIndex& other) override;
    HOST DEVICE TensorIndex operator-(const AbstractTensorIndex& other) const;
    HOST DEVICE virtual AbstractTensorIndex& operator-=(const AbstractTensorIndex& other) override;
    template <size_t OtherDimension>
    HOST DEVICE TensorIndex<Dimension + OtherDimension> operator*(const TensorIndex<OtherDimension>& other) const;

    HOST DEVICE virtual size_t dot(const AbstractTensorIndex& other) const override;
    HOST DEVICE virtual size_t nbInferiorIndices() const override;

    HOST DEVICE virtual bool operator==(const AbstractTensorIndex& other) const override;
    HOST DEVICE virtual bool operator!=(const AbstractTensorIndex& other) const override;
    HOST DEVICE virtual bool operator>=(const AbstractTensorIndex& other) const override;
    HOST DEVICE virtual bool operator>(const AbstractTensorIndex& other) const override;
    HOST DEVICE virtual bool operator<=(const AbstractTensorIndex& other) const override;
    HOST DEVICE virtual bool operator<(const AbstractTensorIndex& other) const override;

    HOST DEVICE virtual const size_t& operator[](const size_t index) const override;
    HOST DEVICE virtual size_t& operator[](const size_t index) override;

    template <long long int From, long long int To>
    using TensorIndexRange = TensorIndex<DimensionOfRange<Dimension, From, To>::result>;
    template <long long int From, long long int To>
    HOST DEVICE TensorIndexRange<From, To> range() const;

    HOST DEVICE virtual const size_t* begin() const override;
    HOST DEVICE virtual const size_t* end() const override;
    HOST DEVICE virtual size_t* begin() override;
    HOST DEVICE virtual size_t* end() override;

    DECLARE_RTTI(AbstractTensorIndex)

private:
    RawTensorIndex<Dimension> m_rawTensorIndex;
};

#include <tensor/TensorIndex.impl.h>
