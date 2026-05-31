#pragma once

#include <helpers/Macros.h>
#include <helpers/RawTuple.h>

#define __TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NAME(EnumClassName) __##EnumClassName##InternalEnum
#define __TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_MAX_VALUE(EnumClassName) __##EnumClassName##MaximumValue

#define __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName) __##EnumClassName##Iterator
#define __TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName) __##EnumClassName##IteratorProvider

#define TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES_NOEXPAND(EnumClassName) TENSOR_MAP_KEY_ENUM__VALUES_OF_##EnumClassName
#define TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(EnumClassName) TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES_NOEXPAND(EnumClassName)
#define TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NB_VALUES(EnumClassName) NARG(TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(EnumClassName))
#define TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_GET_VALUE_NAME(EnumClassName, index) GET_FROM_INDEX(index, TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(EnumClassName))

#define __TENSOR_MAP_KEY_ENUM__DEFINE_INTERNAL_ENUM(EnumClassName)    \
    enum __TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NAME(EnumClassName)     \
    {                                                                 \
        TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(EnumClassName),     \
        __TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_MAX_VALUE(EnumClassName) \
    };

#define __TENSOR_MAP_KEY_ENUM__DEFINE_ITERATOR(EnumClassName, DECLSPEC)                                                      \
    class EnumClassName;                                                                                                     \
    enum __TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NAME(EnumClassName);                                                           \
                                                                                                                             \
    class DECLSPEC __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName)                                                       \
    {                                                                                                                        \
    public:                                                                                                                  \
        __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName)(__TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NAME(EnumClassName) value); \
                                                                                                                             \
        bool operator==(const __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName)& other);                                   \
        bool operator!=(const __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName)& other);                                   \
        __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName)& operator++();                                                   \
        EnumClassName operator*();                                                                                           \
                                                                                                                             \
    private:                                                                                                                 \
        size_t m_index;                                                                                                      \
    };

#define __TENSOR_MAP_KEY_ENUM__DEFINE_ITERATOR_PROVIDER(EnumClassName, DECLSPEC)                                                                                     \
    class __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName);                                                                                                       \
                                                                                                                                                                     \
    class DECLSPEC __TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName)                                                                                      \
    {                                                                                                                                                                \
    public:                                                                                                                                                          \
        __TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName)();                                                                                              \
        __TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName)(const __TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName)& other) = delete;            \
        __TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName)(__TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName)&& other) = delete;                 \
                                                                                                                                                                     \
        __TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName)& operator=(const __TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName)& other) = delete; \
        __TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName)& operator=(__TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName)&& other) = delete;      \
                                                                                                                                                                     \
        __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName) begin() const;                                                                                           \
        __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName) end() const;                                                                                             \
    };

#define DEFINE_TENSOR_MAP_KEY_ENUM(EnumClassName, DECLSPEC)                                                                   \
    __TENSOR_MAP_KEY_ENUM__DEFINE_INTERNAL_ENUM(EnumClassName)                                                                \
    __TENSOR_MAP_KEY_ENUM__DEFINE_ITERATOR(EnumClassName, DECLSPEC)                                                           \
    __TENSOR_MAP_KEY_ENUM__DEFINE_ITERATOR_PROVIDER(EnumClassName, DECLSPEC)                                                  \
                                                                                                                              \
    class DECLSPEC EnumClassName                                                                                              \
    {                                                                                                                         \
    public:                                                                                                                   \
        EnumClassName(const __TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NAME(EnumClassName)& value);                                 \
        EnumClassName(const size_t& value);                                                                                   \
                                                                                                                              \
        operator __TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NAME(EnumClassName)() const;                                            \
        operator size_t() const;                                                                                              \
        bool operator==(const EnumClassName& other) const;                                                                    \
        bool operator!=(const EnumClassName& other) const;                                                                    \
                                                                                                                              \
        using Iterator = __TENSOR_MAP_KEY_ENUM__ITERATOR_NAME(EnumClassName);                                                 \
        using IteratorProvider = __TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName);                                \
        using Values = __TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NAME(EnumClassName);                                              \
        static constexpr size_t NbValues = TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NB_VALUES(EnumClassName);                       \
        static const __TENSOR_MAP_KEY_ENUM__ITERATOR_PROVIDER_NAME(EnumClassName)& values();                                  \
        static RawTuple<REPEAT(EnumClassName, TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NB_VALUES(EnumClassName))> getValuesTuple(); \
                                                                                                                              \
    private:                                                                                                                  \
        __TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NAME(EnumClassName) m_value;                                                     \
    };

#include <tensor/TensorMapKeyEnum.impl.h>
