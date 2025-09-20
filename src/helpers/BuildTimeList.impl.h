#pragma once

#include <helpers/BuildTimeList.h>

namespace BuildTimeList
{
    template <typename ValueType, ValueType... Values>
    struct Size<List<ValueType, Values...>>
    {
        static constexpr size_t result = sizeof...(Values);
    };

    template <typename... Types>
    struct Size<TypeList<Types...>>
    {
        static constexpr size_t result = sizeof...(Types);
    };

    template <typename ValueType, ValueType FirstValue, ValueType... NextValues, typename Enable>
    struct GetImpl<List<ValueType, FirstValue, NextValues...>, 0, Enable>
    {
        static constexpr ValueType result = FirstValue;
    };

    template <typename ValueType, ValueType FirstValue, ValueType... NextValues, size_t Index>
    struct GetImpl<List<ValueType, FirstValue, NextValues...>, Index, std::enable_if_t<(Index > 0)>>
    {
        static constexpr ValueType result = GetImpl<List<ValueType, NextValues...>, (Index - 1), void>::result;
    };

    template <typename FirstType, typename... NextTypes, typename Enable>
    struct GetImpl<TypeList<FirstType, NextTypes...>, 0, Enable>
    {
        using result = FirstType;
    };

    template <typename FirstType, typename... NextTypes, size_t Index>
    struct GetImpl<TypeList<FirstType, NextTypes...>, Index, std::enable_if_t<(Index > 0)>>
    {
        using result = typename GetImpl<TypeList<NextTypes...>, (Index - 1), void>::result;
    };

    template <typename ValueType>
    struct ReverseImpl<List<ValueType>>
    {
        using list = List<ValueType>;
    };

    template <typename ValueType, ValueType FirstValue, ValueType... NextValues>
    struct ReverseImpl<List<ValueType, FirstValue, NextValues...>>
    {
        using list = typename PushRight<typename ReverseImpl<List<ValueType, NextValues...>>::list, ValueType, FirstValue>::list;
    };

    template <>
    struct ReverseImpl<TypeList<>>
    {
        using list = TypeList<>;
    };

    template <typename FirstValue, typename... NextValues>
    struct ReverseImpl<TypeList<FirstValue, NextValues...>>
    {
        using list = typename PushTypeRight<typename ReverseImpl<TypeList<NextValues...>>::list, FirstValue>::list;
    };

    template <typename ValueType, ValueType... Values, ValueType Element>
    struct PushRight<List<ValueType, Values...>, ValueType, Element>
    {
        using list = List<ValueType, Values..., Element>;
    };

    template <typename ValueType, ValueType... Values, ValueType Element>
    struct PushLeft<List<ValueType, Values...>, ValueType, Element>
    {
        using list = List<ValueType, Element, Values...>;
    };

    template <typename ValueType, ValueType... Values>
    struct PopRight<List<ValueType, Values...>>
    {
        static constexpr ValueType element = PopLeft<Reverse<List<ValueType, Values...>>>::element;
        using list = Reverse<typename PopLeft<Reverse<List<ValueType, Values...>>>::list>;
    };

    template <typename ValueType, ValueType... Values, ValueType Element>
    struct PopLeft<List<ValueType, Element, Values...>>
    {
        static constexpr ValueType element = Element;
        using list = List<ValueType, Values...>;
    };

    template <typename... Types, typename Element>
    struct PushTypeRight<TypeList<Types...>, Element>
    {
        using list = TypeList<Types..., Element>;
    };

    template <typename... Types, typename Element>
    struct PushTypeLeft<TypeList<Types...>, Element>
    {
        using list = TypeList<Element, Types...>;
    };

    template <typename... Types>
    struct PopTypeRight<TypeList<Types...>>
    {
        using element = typename PopTypeLeft<Reverse<TypeList<Types...>>>::element;
        using list = Reverse<typename PopTypeLeft<Reverse<TypeList<Types...>>>::list>;
    };

    template <typename Element, typename... Types>
    struct PopTypeLeft<TypeList<Element, Types...>>
    {
        using element = Element;
        using list = TypeList<Types...>;
    };

    template <typename Enable>
    struct RangeImpl<0, Enable>
    {
        using list = IntegerList<>;
    };

    template <size_t N>
    struct RangeImpl<N, std::enable_if_t<(N > 0)>>
    {
        using list = typename PushRight<typename RangeImpl<(N - 1)>::list, size_t, (N - 1)>::list;
    };

    template <typename Key>
    struct MapGet<Map<>, Key>
    {};

    template <typename FirstKey, typename FirstValue, typename... NextMapEntries, typename Key>
    struct MapGet<Map<MapEntry<FirstKey, FirstValue>, NextMapEntries...>, Key>
    {
        using result = typename MapGet<Map<NextMapEntries...>, Key>::result;
    };

    template <typename FirstKey, typename FirstValue, typename... NextMapEntries>
    struct MapGet<Map<MapEntry<FirstKey, FirstValue>, NextMapEntries...>, FirstKey>
    {
        using result = FirstValue;
    };
}
