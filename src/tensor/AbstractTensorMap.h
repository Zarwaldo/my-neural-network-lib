#pragma once

#include <rtti/Rtti.h>

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

    HOST IteratorConst begin() const;
    HOST Iterator begin();

    HOST IteratorConst end() const;
    HOST Iterator end();

    DECLARE_AS_RTTI_BASE_TYPE(AbstractTensorMap<ValueType>)
};

#include <tensor/AbstractTensorMap.impl.h>
