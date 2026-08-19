#pragma once

#include <CommonMacros.h>

#include <tensor/TensorIndex.h>

template <typename ScalarType, size_t Dimension>
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

    HOST DEVICE inline const ScalarType& operator*() const;
    HOST DEVICE inline ScalarType& operator*();

    HOST DEVICE inline const RawTensor subtensor(const RawTensorIndex<Dimension>& minIndex, const RawTensorIndex<Dimension>& maxIndex) const;
    HOST DEVICE inline RawTensor subtensor(const RawTensorIndex<Dimension>& minIndex, const RawTensorIndex<Dimension>& maxIndex);

    HOST DEVICE inline const ScalarType& operator[](const RawTensorIndex<Dimension>& index) const;
    HOST DEVICE inline ScalarType& operator[](const RawTensorIndex<Dimension>& index);

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
    HOST DEVICE inline RawTensor(const RawTensorIndex<Dimension>& sizes, const RawTensorIndex<Dimension>& strides, ScalarType* const data);

    HOST DEVICE inline const ScalarType* getDataPtr() const;

    HOST DEVICE inline ScalarType* getDataPtr();

    HOST DEVICE inline ScalarType* ptrAtIndex(const RawTensorIndex<Dimension>& index) const;

    RawTensorIndex<Dimension> m_sizes;
    RawTensorIndex<Dimension> m_strides;
    ScalarType* m_data;

    template <typename ScalarType, size_t Dimension>
    friend class Tensor;
};

#include <tensor/RawTensor.impl.h>
