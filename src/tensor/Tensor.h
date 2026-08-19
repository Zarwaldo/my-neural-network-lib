#pragma once

#include <tensor/TensorBase.h>

#include <CommonMacros.h>

#include <tensor/ManagedMemorySharedPtr.h>
#include <tensor/RawTensor.h>
#include <tensor/TensorIndex.h>

template <typename ScalarType>
class AbstractTensor : public TensorBase
{
public:
    HOST virtual ~AbstractTensor();

    HOST virtual size_t dim() const = 0;

    HOST virtual const AbstractTensorIndex& sizes() const = 0;

    HOST virtual size_t nbElements() const = 0;
    HOST virtual size_t memorySize() const = 0;

    HOST virtual const ScalarType& operator*() const = 0;
    HOST virtual ScalarType& operator*() = 0;

    HOST virtual const ScalarType& operator[](const AbstractTensorIndex& index) const = 0;
    HOST virtual ScalarType& operator[](const AbstractTensorIndex& index) = 0;
};

template <typename ScalarType, size_t Dimension>
class Tensor : public AbstractTensor<ScalarType>
{
public:
    HOST Tensor(const Tensor& other);
    HOST Tensor(Tensor&& other);
    HOST static Tensor* create(const TensorIndex<Dimension>& sizes);

    HOST virtual ~Tensor();

    HOST virtual Tensor& operator=(const Tensor& other);
    HOST virtual Tensor& operator=(Tensor&& other);

    HOST const RawTensor<ScalarType, Dimension>& getRawTensor() const;

    HOST RawTensor<ScalarType, Dimension>& getRawTensor();

    HOST virtual size_t dim() const override;

    HOST virtual const AbstractTensorIndex& sizes() const override;

    HOST virtual size_t nbElements() const override;
    HOST virtual size_t memorySize() const override;

    HOST virtual const ScalarType& operator*() const override;
    HOST virtual ScalarType& operator*() override;

    HOST const Tensor subtensor(const TensorIndex<Dimension>& minIndex, const TensorIndex<Dimension>& maxIndex) const;
    HOST Tensor subtensor(const TensorIndex<Dimension>& minIndex, const TensorIndex<Dimension>& maxIndex);

    HOST virtual const ScalarType& operator[](const AbstractTensorIndex& index) const override;
    HOST virtual ScalarType& operator[](const AbstractTensorIndex& index) override;

    HOST typename RawTensor<ScalarType, Dimension>::Iterator iterator(const TensorIndex<Dimension>& index) const;

    HOST typename RawTensor<ScalarType, Dimension>::Iterator begin() const;
    HOST typename RawTensor<ScalarType, Dimension>::Iterator end() const;

private:
    HOST Tensor(ManagedMemorySharedPtr<ScalarType>& data, const RawTensor<ScalarType, Dimension>& rawTensor);
    HOST Tensor(ManagedMemorySharedPtr<ScalarType>&& data, const RawTensor<ScalarType, Dimension>& rawTensor);

    ManagedMemorySharedPtr<ScalarType> m_data;
    RawTensor<ScalarType, Dimension> m_rawTensor;
    TensorIndex<Dimension> m_sizes;

    DECLARE_RTTI(TensorBase)
};
