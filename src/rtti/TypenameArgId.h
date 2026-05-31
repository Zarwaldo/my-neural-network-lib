#pragma once

using TypenameArgId = size_t;

namespace __DECLARE_AS_TYPENAME_ARG
{
    inline constexpr size_t __WRAPPERS_MASK =           0xffffffff00000000;
    inline constexpr size_t __POINTER_WRAPPER_ID =      0x0000000100000000;
    inline constexpr size_t __LVALUE_WRAPPER_ID =       0x0000000200000000;
    inline constexpr size_t __RVALUE_WRAPPER_ID =       0x0000000300000000;
    inline constexpr size_t __CONST_WRAPPER_ID =        0x0000000400000000;
    inline constexpr size_t __FIRST_WRAPPER_MASK =      0x0000000f00000000;
    inline constexpr size_t __RAW_TYPE_MASK =           0x00000000ffffffff;
    inline constexpr size_t __INVALID_TYPENAME_ARG_ID = 0x0000000000000000;
    inline constexpr size_t __FIRST_WRAPPER_BIT = 32;
    inline constexpr size_t __WRAPPER_SHIFT = 4;

    template <size_t TypenameArgId, size_t WrapperId>
    struct __PushWrapper
    {
        static constexpr size_t result = (((TypenameArgId & __WRAPPERS_MASK) << __WRAPPER_SHIFT) & __WRAPPERS_MASK)
                                         | WrapperId
                                         | (TypenameArgId & __RAW_TYPE_MASK);
    };

    template <size_t TypenameArgId>
    struct __PopWrapper
    {
        static constexpr size_t result = (((TypenameArgId & __WRAPPERS_MASK) >> __WRAPPER_SHIFT) & __WRAPPERS_MASK)
                                         | (TypenameArgId & __RAW_TYPE_MASK);
    };

    template <class Class>
    static constexpr TypenameArgId __TypenameArgIdOfImpl = __INVALID_TYPENAME_ARG_ID;

    template <typename Type>
    constexpr TypenameArgId __TypenameArgIdOfImpl<Type*> = __PushWrapper<__TypenameArgIdOfImpl<Type>, __POINTER_WRAPPER_ID>::result;
    template <typename Type>
    constexpr TypenameArgId __TypenameArgIdOfImpl<Type&> = __PushWrapper<__TypenameArgIdOfImpl<Type>, __LVALUE_WRAPPER_ID>::result;
    template <typename Type>
    constexpr TypenameArgId __TypenameArgIdOfImpl<Type&&> = __PushWrapper<__TypenameArgIdOfImpl<Type>, __RVALUE_WRAPPER_ID>::result;
    template <typename Type>
    constexpr TypenameArgId __TypenameArgIdOfImpl<const Type> = __PushWrapper<__TypenameArgIdOfImpl<Type>, __CONST_WRAPPER_ID>::result;

    template <TypenameArgId Id, typename Enable = void>
    struct __TypeFromTypenameArgIdImpl
    {};

    template <TypenameArgId Id>
    struct __TypeFromTypenameArgIdImpl<Id, std::enable_if_t<((Id & __FIRST_WRAPPER_MASK) == __POINTER_WRAPPER_ID)>>
    {
        using type = typename __TypeFromTypenameArgIdImpl<__PopWrapper<Id>::result>::type*;
    };

    template <TypenameArgId Id>
    struct __TypeFromTypenameArgIdImpl<Id, std::enable_if_t<((Id & __FIRST_WRAPPER_MASK) == __LVALUE_WRAPPER_ID)>>
    {
        using type = typename __TypeFromTypenameArgIdImpl<__PopWrapper<Id>::result>::type&;
    };

    template <TypenameArgId Id>
    struct __TypeFromTypenameArgIdImpl<Id, std::enable_if_t<((Id & __FIRST_WRAPPER_MASK) == __RVALUE_WRAPPER_ID)>>
    {
        using type = typename __TypeFromTypenameArgIdImpl<__PopWrapper<Id>::result>::type&&;
    };

    template <TypenameArgId Id>
    struct __TypeFromTypenameArgIdImpl<Id, std::enable_if_t<((Id & __FIRST_WRAPPER_MASK) == __CONST_WRAPPER_ID)>>
    {
        using type = const typename __TypeFromTypenameArgIdImpl<__PopWrapper<Id>::result>::type;
    };
}

template <class Class>
static constexpr TypenameArgId TypenameArgIdOf = __DECLARE_AS_TYPENAME_ARG::__TypenameArgIdOfImpl<Class>;

template <TypenameArgId Id>
using TypeFromTypenameArgId = typename __DECLARE_AS_TYPENAME_ARG::__TypeFromTypenameArgIdImpl<Id>::type;

#define DECLARE_AS_TYPENAME_ARG(Typename, Id)                                \
    namespace __DECLARE_AS_TYPENAME_ARG                                      \
    {                                                                        \
        static_assert(                                                       \
            (                                                                \
                !std::is_pointer_v<Typename>                                 \
                && !std::is_lvalue_reference_v<Typename>                     \
                && !std::is_rvalue_reference_v<Typename>                     \
                && !std::is_const_v<Typename>                                \
            ),                                                               \
            "DECLARE_AS_TYPENAME_ARG: Invalid type, only raw types allowed." \
        );                                                                   \
                                                                             \
        static_assert(                                                       \
            (                                                                \
                (Id != __INVALID_TYPENAME_ARG_ID)                            \
                && (Id & ~__RAW_TYPE_MASK) == 0                              \
            ),                                                               \
            "Invalid typename argument ID"                                   \
        );                                                                   \
                                                                             \
        template <>                                                          \
        constexpr TypenameArgId __TypenameArgIdOfImpl<Typename> = Id;        \
                                                                             \
        template <>                                                          \
        struct __TypeFromTypenameArgIdImpl<__TypenameArgIdOfImpl<Typename>>  \
        {                                                                    \
            using type = Typename;                                           \
        };                                                                   \
    }

DECLARE_AS_TYPENAME_ARG(void, 0x00000001)
DECLARE_AS_TYPENAME_ARG(std::nullptr_t, 0x00000002)
DECLARE_AS_TYPENAME_ARG(bool, 0x00000003)
DECLARE_AS_TYPENAME_ARG(char, 0x00000004)
DECLARE_AS_TYPENAME_ARG(short, 0x00000005)
DECLARE_AS_TYPENAME_ARG(int, 0x00000006)
DECLARE_AS_TYPENAME_ARG(long, 0x00000007)
DECLARE_AS_TYPENAME_ARG(float, 0x00000008)
DECLARE_AS_TYPENAME_ARG(double, 0x00000009)
