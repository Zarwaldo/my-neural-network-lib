#pragma once

#include <helpers/TupleTemplate.h>

#define TUPLE_TMPL__GETTER_RETURN_TYPE(constQualifier, ParamType) decltype(std::declval<constQualifier TupleElementContainer<ParamType>>().get())

template <template <typename T> class TupleElementContainer>
HOST DEVICE
TupleTemplate<TupleElementContainer>::TupleTemplate()
{}

#define TUPLE_TMPL__EMPTY_TUPLE_APPLY_METHOD_IMPLEMENTATION(applyMethod, cudaQualifiers, constQualifier) \
    template <template <typename T> class TupleElementContainer>                                         \
    template <typename ApplyFunction>                                                                    \
    cudaQualifiers                                                                                       \
    auto                                                                                                 \
    TupleTemplate<TupleElementContainer>::applyMethod(ApplyFunction&& function) constQualifier           \
    {                                                                                                    \
        return function();                                                                               \
    }

TUPLE_TMPL__EMPTY_TUPLE_APPLY_METHOD_IMPLEMENTATION(deviceApply, DEVICE, const)
TUPLE_TMPL__EMPTY_TUPLE_APPLY_METHOD_IMPLEMENTATION(deviceApply, DEVICE, )
TUPLE_TMPL__EMPTY_TUPLE_APPLY_METHOD_IMPLEMENTATION(hostApply, HOST, const)
TUPLE_TMPL__EMPTY_TUPLE_APPLY_METHOD_IMPLEMENTATION(hostApply, HOST, )

template <template <typename T> class TupleElementContainer>
template <typename ApplyFunction>
HOST DEVICE
auto
TupleTemplate<TupleElementContainer>::apply(ApplyFunction&& function) const
{
#if defined(__CUDA_ARCH__)
    return deviceApply(std::forward<ApplyFunction>(function));
#else
    return hostApply(std::forward<ApplyFunction>(function));
#endif
}

template <template <typename T> class TupleElementContainer>
template <typename ApplyFunction>
HOST DEVICE
auto
TupleTemplate<TupleElementContainer>::apply(ApplyFunction&& function)
{
#if defined(__CUDA_ARCH__)
    return deviceApply(std::forward<ApplyFunction>(function));
#else
    return hostApply(std::forward<ApplyFunction>(function));
#endif
}

#define TUPLE_TMPL__EMPTY_TUPLE_MAP_METHOD_IMPLEMENTATION(mapMethod, cudaQualifiers, constQualifier) \
    template <template <typename T> class TupleElementContainer>                                     \
    template <template <typename InputType> class MapFunction, typename... ContextArgs>              \
    cudaQualifiers                                                                                   \
    TupleTemplate<TupleElementContainer>                                                             \
    TupleTemplate<TupleElementContainer>::mapMethod(ContextArgs&&... contextArgs) constQualifier     \
    {                                                                                                \
        return MakeTuple<TupleElementContainer>::doIt();                                             \
    }

TUPLE_TMPL__EMPTY_TUPLE_MAP_METHOD_IMPLEMENTATION(deviceMap, DEVICE, const)
TUPLE_TMPL__EMPTY_TUPLE_MAP_METHOD_IMPLEMENTATION(deviceMap, DEVICE, )
TUPLE_TMPL__EMPTY_TUPLE_MAP_METHOD_IMPLEMENTATION(hostMap, HOST, const)
TUPLE_TMPL__EMPTY_TUPLE_MAP_METHOD_IMPLEMENTATION(hostMap, HOST, )
TUPLE_TMPL__EMPTY_TUPLE_MAP_METHOD_IMPLEMENTATION(map, HOST DEVICE, const)
TUPLE_TMPL__EMPTY_TUPLE_MAP_METHOD_IMPLEMENTATION(map, HOST DEVICE, )

template <template <typename T> class TupleElementContainer>
HOST DEVICE
TupleTemplate<TupleElementContainer>
TupleTemplate<TupleElementContainer>::getPointers() const
{
    return MakeTuple<TupleElementContainer>::doIt();
}

template <template <typename T> class TupleElementContainer>
HOST DEVICE
TupleTemplate<TupleElementContainer>
TupleTemplate<TupleElementContainer>::getPointers()
{
    return MakeTuple<TupleElementContainer>::doIt();
}

template <template <typename T> class TupleElementContainer, typename FirstType, typename... NextTypes>
HOST DEVICE
TupleTemplate<TupleElementContainer, FirstType, NextTypes...>::TupleTemplate(FirstType&& firstObject, NextTypes&&... nextObjects)
    : m_firstElement(TupleElementContainer<FirstType>(std::forward<FirstType>(firstObject)))
    , m_nextElements(TupleTemplate<TupleElementContainer, NextTypes...>(std::forward<NextTypes>(nextObjects)...))
{
}

template <template <typename T> class TupleElementContainer, typename FirstType, typename... NextTypes>
template <size_t N>
HOST DEVICE
const auto&
TupleTemplate<TupleElementContainer, FirstType, NextTypes...>::get() const
{
    if constexpr (N == 0)
    {
        return m_firstElement.get();
    }
    else
    {
        return m_nextElements.template get<(N - 1)>();
    }
}

template <template <typename T> class TupleElementContainer, typename FirstType, typename... NextTypes>
template <size_t N>
HOST DEVICE
auto&
TupleTemplate<TupleElementContainer, FirstType, NextTypes...>::get()
{
    if constexpr (N == 0)
    {
        return m_firstElement.get();
    }
    else
    {
        return m_nextElements.template get<(N - 1)>();
    }
}

#define TUPLE_TMPL__NONEMPTY_TUPLE_APPLY_METHOD_IMPLEMENTATION(applyMethod, cudaQualifiers, constQualifier)             \
    template <template <typename T> class TupleElementContainer, typename FirstType, typename... NextTypes>             \
    template <typename ApplyFunction>                                                                                   \
    cudaQualifiers                                                                                                      \
    auto                                                                                                                \
    TupleTemplate<TupleElementContainer, FirstType, NextTypes...>::applyMethod(ApplyFunction&& function) constQualifier \
    {                                                                                                                   \
        return m_nextElements.applyMethod(                                                                              \
            [this, &function](TUPLE_TMPL__GETTER_RETURN_TYPE(constQualifier, NextTypes)... nextElements)                \
            {                                                                                                           \
                return function(m_firstElement.get(), nextElements...);                                                 \
            }                                                                                                           \
        );                                                                                                              \
    }

TUPLE_TMPL__NONEMPTY_TUPLE_APPLY_METHOD_IMPLEMENTATION(deviceApply, DEVICE, const)
TUPLE_TMPL__NONEMPTY_TUPLE_APPLY_METHOD_IMPLEMENTATION(deviceApply, DEVICE, )
TUPLE_TMPL__NONEMPTY_TUPLE_APPLY_METHOD_IMPLEMENTATION(hostApply, HOST, const)
TUPLE_TMPL__NONEMPTY_TUPLE_APPLY_METHOD_IMPLEMENTATION(hostApply, HOST, )

template <template <typename T> class TupleElementContainer, typename FirstType, typename... NextTypes>
template <typename ApplyFunction>
HOST DEVICE
auto
TupleTemplate<TupleElementContainer, FirstType, NextTypes...>::apply(ApplyFunction&& function) const
{
#if defined(__CUDA_ARCH__)
    return deviceApply(std::forward<ApplyFunction>(function));
#else
    return hostApply(std::forward<ApplyFunction>(function));
#endif
}

template <template <typename T> class TupleElementContainer, typename FirstType, typename... NextTypes>
template <typename ApplyFunction>
HOST DEVICE
auto
TupleTemplate<TupleElementContainer, FirstType, NextTypes...>::apply(ApplyFunction&& function)
{
#if defined(__CUDA_ARCH__)
    return deviceApply(std::forward<ApplyFunction>(function));
#else
    return hostApply(std::forward<ApplyFunction>(function));
#endif
}

#define TUPLE_TMPL__NONEMPTY_TUPLE_MAP_METHOD_IMPLEMENTATION(mapMethod, applyMethod, cudaQualifiers, constQualifier)                                                                         \
    template <template <typename T> class TupleElementContainer, typename FirstType, typename... NextTypes>                                                                      \
    template <template <typename InputType> class MapFunction, typename... ContextArgs>                                                                                          \
    cudaQualifiers                                                                                                                                                               \
    auto                                                                                                                                                                         \
    TupleTemplate<TupleElementContainer, FirstType, NextTypes...>::mapMethod(ContextArgs&&... contextArgs) constQualifier                                                        \
    {                                                                                                                                                                            \
        return applyMethod(                                                                                                                                                      \
            [&contextArgs...](TUPLE_TMPL__GETTER_RETURN_TYPE(constQualifier, FirstType) firstElement, TUPLE_TMPL__GETTER_RETURN_TYPE(constQualifier, NextTypes)... nextElements) \
            {                                                                                                                                                                    \
                return MakeTuple<TupleElementContainer>::doIt(                                                                                                                   \
                    MapFunction<std::remove_const_t<std::remove_reference_t<FirstType>>>::doIt(firstElement, std::forward<ContextArgs>(contextArgs)...),                         \
                    MapFunction<std::remove_const_t<std::remove_reference_t<NextTypes>>>::doIt(nextElements, std::forward<ContextArgs>(contextArgs)...)...                       \
                );                                                                                                                                                               \
            }                                                                                                                                                                    \
        );                                                                                                                                                                       \
    }

TUPLE_TMPL__NONEMPTY_TUPLE_MAP_METHOD_IMPLEMENTATION(deviceMap, deviceApply, DEVICE, const)
TUPLE_TMPL__NONEMPTY_TUPLE_MAP_METHOD_IMPLEMENTATION(deviceMap, deviceApply, DEVICE, )
TUPLE_TMPL__NONEMPTY_TUPLE_MAP_METHOD_IMPLEMENTATION(hostMap, hostApply, HOST, const)
TUPLE_TMPL__NONEMPTY_TUPLE_MAP_METHOD_IMPLEMENTATION(hostMap, hostApply, HOST, )
TUPLE_TMPL__NONEMPTY_TUPLE_MAP_METHOD_IMPLEMENTATION(map, apply, HOST DEVICE, const)
TUPLE_TMPL__NONEMPTY_TUPLE_MAP_METHOD_IMPLEMENTATION(map, apply, HOST DEVICE, )

template <template <typename T> class TupleElementContainer, typename FirstType, typename... NextTypes>
template <typename FirstConvertedType, typename... NextConvertedTypes>
HOST DEVICE
TupleTemplate<TupleElementContainer, FirstType, NextTypes...>::operator TupleTemplate<TupleElementContainer, FirstConvertedType, NextConvertedTypes...>() const
{
    return apply(
        [](TUPLE_TMPL__GETTER_RETURN_TYPE(const, FirstType) firstElement, TUPLE_TMPL__GETTER_RETURN_TYPE(const, NextTypes)... nextElements)
        {
            return TupleTemplate<TupleElementContainer, FirstConvertedType, NextConvertedTypes...>(
                static_cast<FirstConvertedType>(firstElement),
                static_cast<NextConvertedTypes>(nextElements)...
            );
        }
    );
}

template <typename InputType>
struct ToPointerMap
{
    static const InputType* doIt(const InputType& value)
    {
        return &value;
    }

    static InputType* doIt(InputType& value)
    {
        return &value;
    }
};

template <template <typename T> class TupleElementContainer, typename FirstType, typename... NextTypes>
HOST DEVICE
auto
TupleTemplate<TupleElementContainer, FirstType, NextTypes...>::getPointers() const
{
    return map<ToPointerMap>();
}

template <template <typename T> class TupleElementContainer, typename FirstType, typename... NextTypes>
HOST DEVICE
auto
TupleTemplate<TupleElementContainer, FirstType, NextTypes...>::getPointers()
{
    return map<ToPointerMap>();
}

template <template <typename T> class TupleElementContainer>
template <typename... Types>
TupleTemplate<TupleElementContainer, Types...>
MakeTuple<TupleElementContainer>::doIt(Types&&... elements)
{
    return TupleTemplate<TupleElementContainer, Types...>(std::forward<Types>(elements)...);
}
