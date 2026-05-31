#pragma once

#include <tensor/TensorBase.h>

#include <CommonMacros.h>

#include <tensor/TensorIndex.h>
#include <tensor/ManagedMemorySharedPtr.h>

template <typename ValueType>
class AbstractTensor : public TensorBase
{
public:
    HOST virtual inline ~AbstractTensor();

    HOST virtual inline size_t dim() const = 0;

    HOST virtual inline const AbstractTensorIndex& sizes() const = 0;

    HOST virtual inline size_t nbElements() const = 0;
    HOST virtual inline size_t memorySize() const = 0;

    HOST virtual inline const ValueType& operator*() const = 0;
    HOST virtual inline ValueType& operator*() = 0;

    HOST virtual inline const ValueType& operator[](const AbstractTensorIndex& index) const = 0;
    HOST virtual inline ValueType& operator[](const AbstractTensorIndex& index) = 0;
};

template <typename ValueType, size_t Dimension>
class RawTensor
{
public:
    HOST DEVICE inline RawTensor(const RawTensor& other);

    HOST DEVICE inline ~RawTensor();

    HOST DEVICE inline RawTensor& operator=(const RawTensor& other);

    HOST DEVICE inline size_t dim() const;

    HOST DEVICE inline const RawTensorIndex<Dimension>& sizes() const;

    HOST DEVICE inline size_t nbElements() const;
    HOST DEVICE inline size_t memorySize() const;

    HOST DEVICE inline const ValueType& operator*() const;
    HOST DEVICE inline ValueType& operator*();

    HOST DEVICE inline const RawTensor subtensor(const RawTensorIndex<Dimension>& minIndex, const RawTensorIndex<Dimension>& maxIndex) const;
    HOST DEVICE inline RawTensor subtensor(const RawTensorIndex<Dimension>& minIndex, const RawTensorIndex<Dimension>& maxIndex);

    HOST DEVICE inline const ValueType& operator[](const RawTensorIndex<Dimension>& index) const;
    HOST DEVICE inline ValueType& operator[](const RawTensorIndex<Dimension>& index);

    class Iterator
    {
    public:
        HOST DEVICE inline ~Iterator();

        HOST DEVICE inline const RawTensorIndex<Dimension>& operator*() const;
        HOST DEVICE inline Iterator& operator++();
        HOST DEVICE inline bool operator==(const Iterator& other) const;
        HOST DEVICE inline bool operator!=(const Iterator& other) const;

    private:
        HOST DEVICE inline Iterator(const RawTensorIndex<Dimension>& index, const RawTensorIndex<Dimension>& sizes);

        RawTensorIndex<Dimension> m_index;
        const RawTensorIndex<Dimension> m_sizes;

        friend class RawTensor;
    };

    HOST DEVICE inline Iterator iterator(const RawTensorIndex<Dimension>& index) const;

    HOST DEVICE inline Iterator begin() const;
    HOST DEVICE inline Iterator end() const;

private:
    HOST DEVICE inline RawTensor(const RawTensorIndex<Dimension>& sizes, const RawTensorIndex<Dimension>& strides, ValueType* const data);

    HOST DEVICE inline const ValueType* getDataPtr() const;

    HOST DEVICE inline ValueType* getDataPtr();

    HOST DEVICE inline ValueType* ptrAtIndex(const RawTensorIndex<Dimension>& index) const;

    RawTensorIndex<Dimension> m_sizes;
    RawTensorIndex<Dimension> m_strides;
    ValueType* m_data;

    template <typename ValueType, size_t Dimension>
    friend class Tensor;
};

template <typename ValueType, size_t Dimension>
class Tensor : public AbstractTensor<ValueType>
{
public:
    HOST inline Tensor(const Tensor& other);
    HOST inline Tensor(Tensor&& other);
    HOST static inline Tensor* create(const TensorIndex<Dimension>& sizes);

    HOST virtual inline ~Tensor();

    HOST virtual inline Tensor& operator=(const Tensor& other);
    HOST virtual inline Tensor& operator=(Tensor&& other);

    HOST inline const RawTensor<ValueType, Dimension>& getRawTensor() const;

    HOST inline RawTensor<ValueType, Dimension>& getRawTensor();

    HOST virtual inline size_t dim() const override;

    HOST virtual inline const AbstractTensorIndex& sizes() const override;

    HOST virtual inline size_t nbElements() const override;
    HOST virtual inline size_t memorySize() const override;

    HOST virtual inline const ValueType& operator*() const override;
    HOST virtual inline ValueType& operator*() override;

    HOST inline const Tensor subtensor(const TensorIndex<Dimension>& minIndex, const TensorIndex<Dimension>& maxIndex) const;
    HOST inline Tensor subtensor(const TensorIndex<Dimension>& minIndex, const TensorIndex<Dimension>& maxIndex);

    HOST virtual inline const ValueType& operator[](const AbstractTensorIndex& index) const override;
    HOST virtual inline ValueType& operator[](const AbstractTensorIndex& index) override;

    HOST inline typename RawTensor<ValueType, Dimension>::Iterator iterator(const TensorIndex<Dimension>& index) const;

    HOST inline typename RawTensor<ValueType, Dimension>::Iterator begin() const;
    HOST inline typename RawTensor<ValueType, Dimension>::Iterator end() const;

private:
    HOST inline Tensor(ManagedMemorySharedPtr<ValueType>& data, const RawTensor<ValueType, Dimension>& rawTensor);
    HOST inline Tensor(ManagedMemorySharedPtr<ValueType>&& data, const RawTensor<ValueType, Dimension>& rawTensor);

    ManagedMemorySharedPtr<ValueType> m_data;
    RawTensor<ValueType, Dimension> m_rawTensor;
    TensorIndex<Dimension> m_sizes;

    DECLARE_RTTI(TensorBase)
};

#include <tensor/Tensor.impl.h>
