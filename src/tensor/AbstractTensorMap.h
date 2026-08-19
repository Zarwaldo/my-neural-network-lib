#pragma once

#include <rtti/Rtti.h>

template <typename ScalarType>
class AbstractTensor;

template <typename ScalarType>
class AbstractTensorMap
{
public:
    HOST virtual ~AbstractTensorMap();

    HOST virtual size_t nbTensors() const = 0;

    HOST virtual const AbstractTensor<ScalarType>& get(size_t index) const = 0;
    HOST virtual AbstractTensor<ScalarType>& get(size_t index) = 0;

#define __TENSOR_MAP__ITERATOR_DECLARATION(IteratorName, constQualifier)                     \
    class IteratorName                                                                       \
    {                                                                                        \
    public:                                                                                  \
        IteratorName(constQualifier AbstractTensorMap<ScalarType>& tensorMap, size_t index); \
                                                                                             \
        bool operator==(const IteratorName& other) const;                                    \
        bool operator!=(const IteratorName& other) const;                                    \
                                                                                             \
        IteratorName& operator++();                                                          \
                                                                                             \
        const AbstractTensor<ScalarType>& operator*() const;                                 \
        constQualifier AbstractTensor<ScalarType>& operator*();                              \
                                                                                             \
    private:                                                                                 \
        constQualifier AbstractTensorMap<ScalarType>& m_tensorMap;                           \
        size_t m_index;                                                                      \
    };

    __TENSOR_MAP__ITERATOR_DECLARATION(IteratorConst, const)
    __TENSOR_MAP__ITERATOR_DECLARATION(Iterator, )

    HOST IteratorConst begin() const;
    HOST Iterator begin();

    HOST IteratorConst end() const;
    HOST Iterator end();

    DECLARE_AS_RTTI_BASE_TYPE(AbstractTensorMap<ScalarType>)
};

#include <tensor/AbstractTensorMap.impl.h>
