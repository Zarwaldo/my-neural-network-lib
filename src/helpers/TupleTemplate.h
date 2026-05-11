#pragma once

#include <CommonMacros.h>

#include <type_traits>

template <template <typename T> class TupleElementContainer, typename... Types>
class TupleTemplate
{};

template <template <typename T> class TupleElementContainer>
class TupleTemplate<TupleElementContainer>
{
public:
    HOST DEVICE TupleTemplate();

#define TUPLE_TMPL__EMPTY_TUPLE_APPLY_METHOD_PROTOTYPE(applyMethod, cudaQualifiers, constQualifier) \
    template <typename ApplyFunction>                                                               \
    cudaQualifiers auto applyMethod(ApplyFunction&& function) constQualifier;

    TUPLE_TMPL__EMPTY_TUPLE_APPLY_METHOD_PROTOTYPE(deviceApply, DEVICE, const)
    TUPLE_TMPL__EMPTY_TUPLE_APPLY_METHOD_PROTOTYPE(deviceApply, DEVICE, )
    TUPLE_TMPL__EMPTY_TUPLE_APPLY_METHOD_PROTOTYPE(hostApply, HOST, const)
    TUPLE_TMPL__EMPTY_TUPLE_APPLY_METHOD_PROTOTYPE(hostApply, HOST, )
    TUPLE_TMPL__EMPTY_TUPLE_APPLY_METHOD_PROTOTYPE(apply, HOST DEVICE, const)
    TUPLE_TMPL__EMPTY_TUPLE_APPLY_METHOD_PROTOTYPE(apply, HOST DEVICE, )

#define TUPLE_TMPL__EMPTY_TUPLE_MAP_METHOD_PROTOTYPE(mapMethod, cudaQualifiers, constQualifier)                 \
    template <template <typename InputType> class MapFunction, typename... ContextArgs>                         \
    cudaQualifiers TupleTemplate<TupleElementContainer> mapMethod(ContextArgs&&... contextArgs) constQualifier;

    TUPLE_TMPL__EMPTY_TUPLE_MAP_METHOD_PROTOTYPE(deviceMap, DEVICE, const)
    TUPLE_TMPL__EMPTY_TUPLE_MAP_METHOD_PROTOTYPE(deviceMap, DEVICE, )
    TUPLE_TMPL__EMPTY_TUPLE_MAP_METHOD_PROTOTYPE(hostMap, HOST, const)
    TUPLE_TMPL__EMPTY_TUPLE_MAP_METHOD_PROTOTYPE(hostMap, HOST, )
    TUPLE_TMPL__EMPTY_TUPLE_MAP_METHOD_PROTOTYPE(map, HOST DEVICE, const)
    TUPLE_TMPL__EMPTY_TUPLE_MAP_METHOD_PROTOTYPE(map, HOST DEVICE, )

    HOST DEVICE TupleTemplate<TupleElementContainer> getPointers() const;

    HOST DEVICE TupleTemplate<TupleElementContainer> getPointers();
};

template <template <typename T> class TupleElementContainer, typename FirstType, typename... NextTypes>
class TupleTemplate<TupleElementContainer, FirstType, NextTypes...>
{
public:
    HOST DEVICE TupleTemplate(FirstType&& firstObject, NextTypes&&... nextObjects);

    template <size_t N>
    HOST DEVICE const auto& get() const;

    template <size_t N>
    HOST DEVICE auto& get();

#define TUPLE_TMPL__NONEMPTY_TUPLE_APPLY_METHOD_PROTOTYPE(applyMethod, cudaQualifiers, constQualifier) \
    template <typename ApplyFunction>                                                                  \
    cudaQualifiers auto applyMethod(ApplyFunction&& function) constQualifier;

    TUPLE_TMPL__NONEMPTY_TUPLE_APPLY_METHOD_PROTOTYPE(deviceApply, DEVICE, const)
    TUPLE_TMPL__NONEMPTY_TUPLE_APPLY_METHOD_PROTOTYPE(deviceApply, DEVICE, )
    TUPLE_TMPL__NONEMPTY_TUPLE_APPLY_METHOD_PROTOTYPE(hostApply, HOST, const)
    TUPLE_TMPL__NONEMPTY_TUPLE_APPLY_METHOD_PROTOTYPE(hostApply, HOST, )
    TUPLE_TMPL__NONEMPTY_TUPLE_APPLY_METHOD_PROTOTYPE(apply, HOST DEVICE, const)
    TUPLE_TMPL__NONEMPTY_TUPLE_APPLY_METHOD_PROTOTYPE(apply, HOST DEVICE, )

#define TUPLE_TMPL__NONEMPTY_TUPLE_MAP_METHOD_PROTOTYPE(mapMethod, cudaQualifiers, constQualifier)  \
    template <template <typename InputType> class MapFunction, typename... ContextArgs>             \
    cudaQualifiers auto mapMethod(ContextArgs&&... contextArgs) constQualifier;

    TUPLE_TMPL__NONEMPTY_TUPLE_MAP_METHOD_PROTOTYPE(deviceMap, DEVICE, const)
    TUPLE_TMPL__NONEMPTY_TUPLE_MAP_METHOD_PROTOTYPE(deviceMap, DEVICE, )
    TUPLE_TMPL__NONEMPTY_TUPLE_MAP_METHOD_PROTOTYPE(hostMap, HOST, const)
    TUPLE_TMPL__NONEMPTY_TUPLE_MAP_METHOD_PROTOTYPE(hostMap, HOST, )
    TUPLE_TMPL__NONEMPTY_TUPLE_MAP_METHOD_PROTOTYPE(map, HOST DEVICE, const)
    TUPLE_TMPL__NONEMPTY_TUPLE_MAP_METHOD_PROTOTYPE(map, HOST DEVICE, )

    template <typename FirstConvertedType, typename... NextConvertedTypes>
    HOST DEVICE operator TupleTemplate<TupleElementContainer, FirstConvertedType, NextConvertedTypes...>() const;

    HOST DEVICE auto getPointers() const;

    HOST DEVICE auto getPointers();

private:
    TupleElementContainer<FirstType> m_firstElement;
    TupleTemplate<TupleElementContainer, NextTypes...> m_nextElements;
};

template <template <typename T> class TupleElementContainer>
struct MakeTuple
{
    template <typename... Types>
    HOST DEVICE static TupleTemplate<TupleElementContainer, Types...> doIt(Types&&... elements);
};

#include <helpers/TupleTemplate.impl.h>