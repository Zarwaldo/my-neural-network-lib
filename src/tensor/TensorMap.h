#pragma once

#include <tensor/TensorMapKeyEnum.h>

#include <initializer_list>

template <typename ValueType>
class AbstractTensor;

template <typename ValueType>
class AbstractTensorMap
{
public:
    HOST virtual ~AbstractTensorMap();

    HOST virtual size_t nbTensors() const = 0;

    HOST virtual const AbstractTensor<ValueType>& get(size_t index) const = 0;
    HOST virtual AbstractTensor<ValueType>& get(size_t index) = 0;

#define __TENSOR_MAP__ITERATOR_DECLARATION(IteratorName, constQualifier)                    \
    class IteratorName                                                                      \
    {                                                                                       \
    public:                                                                                 \
        IteratorName(constQualifier AbstractTensorMap<ValueType>& tensorMap, size_t index); \
                                                                                            \
        bool operator==(const IteratorName& other) const;                                   \
        bool operator!=(const IteratorName& other) const;                                   \
                                                                                            \
        IteratorName& operator++();                                                         \
                                                                                            \
        const AbstractTensor<ValueType>& operator*() const;                                 \
        constQualifier AbstractTensor<ValueType>& operator*();                              \
                                                                                            \
    private:                                                                                \
        constQualifier AbstractTensorMap<ValueType>& m_tensorMap;                           \
        size_t m_index;                                                                     \
    };

    __TENSOR_MAP__ITERATOR_DECLARATION(IteratorConst, const)
    __TENSOR_MAP__ITERATOR_DECLARATION(Iterator, )

    IteratorConst begin() const;
    Iterator begin();

    IteratorConst end() const;
    Iterator end();
};

template <typename ValueType, typename KeyEnum>
class TensorMap : public AbstractTensorMap<ValueType>
{
public:
    HOST TensorMap(AbstractTensor<ValueType>* const tensors[KeyEnum::NbValues]);
    HOST TensorMap(const std::initializer_list<AbstractTensor<ValueType>*>& list);
    HOST TensorMap(const TensorMap& other);
    HOST virtual ~TensorMap();

    HOST virtual size_t nbTensors() const override;

    HOST virtual const AbstractTensor<ValueType>& get(size_t index) const override;
    HOST virtual AbstractTensor<ValueType>& get(size_t index) override;

    HOST const AbstractTensor<ValueType>& get(KeyEnum key) const;
    HOST AbstractTensor<ValueType>& get(KeyEnum key);

private:
    AbstractTensor<ValueType>* m_tensors[KeyEnum::NbValues];
};

#include <tensor/TensorMap.impl.h>