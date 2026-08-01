#pragma once

#include <tensor/TensorMapKeyEnum.h>

#define __TENSOR_MAP_KEY_ENUM__INSTANTIATE_ELEMENT(valueName, EnumClassName) EnumClassName(EnumClassName::Values::valueName)

#define __TENSOR_MAP_KEY_ENUM__IMPLEMENT_ITERATOR(EnumClassName)                                                                                                             \
    __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName)::__TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName)(__TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NAME(EnumClassName) value) \
        : m_index(static_cast<size_t>(value))                                                                                                                                \
    {                                                                                                                                                                        \
    }                                                                                                                                                                        \
                                                                                                                                                                             \
    bool                                                                                                                                                                     \
    __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName)::operator==(const __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName)& other)                                        \
    {                                                                                                                                                                        \
        return m_index == other.m_index;                                                                                                                                     \
    }                                                                                                                                                                        \
                                                                                                                                                                             \
    bool                                                                                                                                                                     \
    __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName)::operator!=(const __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName)& other)                                        \
    {                                                                                                                                                                        \
        return !(*this == other);                                                                                                                                            \
    }                                                                                                                                                                        \
                                                                                                                                                                             \
    __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName)& __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName)::operator++()                                                   \
    {                                                                                                                                                                        \
        ++m_index;                                                                                                                                                           \
        return *this;                                                                                                                                                        \
    }                                                                                                                                                                        \
                                                                                                                                                                             \
    EnumClassName                                                                                                                                                            \
    __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName)::operator*()                                                                                                         \
    {                                                                                                                                                                        \
        return EnumClassName(static_cast<__TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NAME(EnumClassName)>(m_index));                                                                \
    }

#define __TENSOR_MAP_KEY_ENUM__IMPLEMENT_ITERATOR_PROVIDER(EnumClassName)                                                         \
    __TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName)::__TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName)()  \
    {                                                                                                                             \
    }                                                                                                                             \
                                                                                                                                  \
    __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName)                                                                           \
    __TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName)::begin() const                                                   \
    {                                                                                                                             \
        return __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName){__TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NAME(EnumClassName){}};   \
    }                                                                                                                             \
                                                                                                                                  \
    __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName)                                                                           \
    __TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName)::end() const                                                     \
    {                                                                                                                             \
        return __TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_MAX_VALUE(EnumClassName);                                                     \
    }

#define IMPLEMENT_TENSOR_MAP_KEY_ENUM(EnumClassName)                                                                 \
    __TENSOR_MAP_KEY_ENUM__IMPLEMENT_ITERATOR(EnumClassName)                                                         \
    __TENSOR_MAP_KEY_ENUM__IMPLEMENT_ITERATOR_PROVIDER(EnumClassName)                                                \
                                                                                                                     \
    EnumClassName::EnumClassName(const __TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NAME(EnumClassName)& value)              \
        : m_value(value)                                                                                             \
    {}                                                                                                               \
                                                                                                                     \
    EnumClassName::EnumClassName(size_t value)                                                                       \
        : EnumClassName(static_cast<__TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NAME(EnumClassName)>(value))                \
    {}                                                                                                               \
                                                                                                                     \
    EnumClassName::~EnumClassName()                                                                                  \
    {}                                                                                                               \
                                                                                                                     \
    EnumClassName::operator __TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NAME(EnumClassName)() const                         \
    {                                                                                                                \
        return m_value;                                                                                              \
    }                                                                                                                \
                                                                                                                     \
    EnumClassName::operator size_t() const                                                                           \
    {                                                                                                                \
        return static_cast<size_t>(this->operator __TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NAME(EnumClassName)());       \
    }                                                                                                                \
                                                                                                                     \
    bool                                                                                                             \
    EnumClassName::operator==(const AbstractTensorMapKeyEnum& other) const                                           \
    {                                                                                                                \
        const EnumClassName* otherPtr = dynamic_cast<const EnumClassName*>(&other);                                  \
        if (otherPtr == nullptr)                                                                                     \
            return false;                                                                                            \
                                                                                                                     \
        return m_value == otherPtr->m_value;                                                                         \
    }                                                                                                                \
                                                                                                                     \
    bool                                                                                                             \
    EnumClassName::operator!=(const AbstractTensorMapKeyEnum& other) const                                           \
    {                                                                                                                \
        return !(*this == other);                                                                                    \
    }                                                                                                                \
                                                                                                                     \
    namespace                                                                                                        \
    {                                                                                                                \
        const __TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName) s_##EnumClassName##IteratorProvider = {}; \
    }                                                                                                                \
                                                                                                                     \
    const __TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName)&                                              \
    EnumClassName::values()                                                                                          \
    {                                                                                                                \
        return s_##EnumClassName##IteratorProvider;                                                                  \
    }                                                                                                                \
                                                                                                                     \
    RawTuple<REPEAT(EnumClassName, TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NB_VALUES(EnumClassName))>                     \
    EnumClassName::getValuesTuple()                                                                                  \
    {                                                                                                                \
        return makeRawTuple(                                                                                         \
            FOR_EACH(                                                                                                \
                __TENSOR_MAP_KEY_ENUM__INSTANTIATE_ELEMENT,                                                          \
                FORWARD(EnumClassName),                                                                              \
                TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(EnumClassName)                                             \
            )                                                                                                        \
        );                                                                                                           \
    }                                                                                                                \
                                                                                                                     \
    IMPLEMENT_RTTI(EnumClassName, AbstractTensorMapKeyEnum, PACK(), PACK())
