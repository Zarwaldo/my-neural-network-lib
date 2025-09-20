#pragma once

#include <type_traits>

namespace BuildTimeList
{
    template <typename ValueType, ValueType... Values>
    struct List
    {};

    template <typename... Values>
    struct TypeList
    {};

    template <typename List>
    struct Size
    {};

    template <typename List, size_t Index, typename Enable>
    struct GetImpl
    {};

    template <typename List, size_t Index>
    using Get = GetImpl<List, Index, void>;

    template <typename List>
    struct ReverseImpl
    {};

    template <typename List>
    using Reverse = typename ReverseImpl<List>::list;

    template <typename List, typename ValueType, ValueType Element>
    struct PushRight
    {};

    template <typename List, typename ValueType, ValueType Element>
    struct PushLeft
    {};

    template <typename List>
    struct PopRight
    {};

    template <typename List>
    struct PopLeft
    {};

    template <typename TypeList, typename Element>
    struct PushTypeRight
    {};

    template <typename TypeList, typename Element>
    struct PushTypeLeft
    {};

    template <typename TypeList>
    struct PopTypeRight
    {};

    template <typename TypeList>
    struct PopTypeLeft
    {};

    template <size_t... Values>
    using IntegerList = List<size_t, Values...>;

    template <size_t N, typename Enable = void>
    struct RangeImpl
    {};

    template <size_t N>
    using Range = typename RangeImpl<N, void>::list;

    template <auto...>
    struct Tuple
    {};

    template <typename Key, typename Value>
    struct MapEntry
    {};

    template <typename... Entries>
    struct Map
    {};

    template <typename Map, typename Key>
    struct MapGet
    {};
}

#include <helpers/BuildTimeList.impl.h>
