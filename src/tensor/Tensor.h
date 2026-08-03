#pragma once

#include <tensor/TensorBase.h>

#include <CommonMacros.h>

#include <tensor/ManagedMemorySharedPtr.h>
#include <tensor/RawTensor.h>
#include <tensor/TensorIndex.h>

template <typename ValueType>
class AbstractTensor : public TensorBase
{
public:
    HOST virtual ~AbstractTensor();

    HOST virtual size_t dim() const = 0;

    HOST virtual const AbstractTensorIndex& sizes() const = 0;

    HOST virtual size_t nbElements() const = 0;
    HOST virtual size_t memorySize() const = 0;

    HOST virtual const ValueType& operator*() const = 0;
    HOST virtual ValueType& operator*() = 0;

    HOST virtual const ValueType& operator[](const AbstractTensorIndex& index) const = 0;
    HOST virtual ValueType& operator[](const AbstractTensorIndex& index) = 0;
};

template <typename ValueType, size_t Dimension>
class Tensor : public AbstractTensor<ValueType>
{
public:
    HOST Tensor(const Tensor& other);
    HOST Tensor(Tensor&& other);
    HOST static Tensor* create(const TensorIndex<Dimension>& sizes);

    HOST virtual ~Tensor();

    HOST virtual Tensor& operator=(const Tensor& other);
    HOST virtual Tensor& operator=(Tensor&& other);

    HOST const RawTensor<ValueType, Dimension>& getRawTensor() const;

    HOST RawTensor<ValueType, Dimension>& getRawTensor();

    HOST virtual size_t dim() const override;

    HOST virtual const AbstractTensorIndex& sizes() const override;

    HOST virtual size_t nbElements() const override;
    HOST virtual size_t memorySize() const override;

    HOST virtual const ValueType& operator*() const override;
    HOST virtual ValueType& operator*() override;

    HOST const Tensor subtensor(const TensorIndex<Dimension>& minIndex, const TensorIndex<Dimension>& maxIndex) const;
    HOST Tensor subtensor(const TensorIndex<Dimension>& minIndex, const TensorIndex<Dimension>& maxIndex);

    HOST virtual const ValueType& operator[](const AbstractTensorIndex& index) const override;
    HOST virtual ValueType& operator[](const AbstractTensorIndex& index) override;

    HOST typename RawTensor<ValueType, Dimension>::Iterator iterator(const TensorIndex<Dimension>& index) const;

    HOST typename RawTensor<ValueType, Dimension>::Iterator begin() const;
    HOST typename RawTensor<ValueType, Dimension>::Iterator end() const;

private:
    HOST Tensor(ManagedMemorySharedPtr<ValueType>& data, const RawTensor<ValueType, Dimension>& rawTensor);
    HOST Tensor(ManagedMemorySharedPtr<ValueType>&& data, const RawTensor<ValueType, Dimension>& rawTensor);

    ManagedMemorySharedPtr<ValueType> m_data;
    RawTensor<ValueType, Dimension> m_rawTensor;
    TensorIndex<Dimension> m_sizes;

    DECLARE_RTTI(TensorBase)
};
