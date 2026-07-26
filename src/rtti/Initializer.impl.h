#pragma once

#include <rtti/Initializer.h>

#include <utility>

AbstractInitializer::~AbstractInitializer()
{}

template <typename... ParamTypes>
Initializer<ParamTypes...>::Initializer(ParamTypes&&... args)
    : m_args(makePolymorphicTuple(std::forward<ParamTypes>(args)...))
{}

template <typename... ParamTypes>
Initializer<ParamTypes...>::~Initializer()
{}

template <typename... ParamTypes>
template <typename FunctionType>
auto
Initializer<ParamTypes...>::apply(FunctionType&& function) const
{
    return m_args.hostApply(std::move(function));
}

namespace InitializerDetails
{
    template <typename Enable, typename... ParamTypes>
    struct Equal
    {
        static constexpr bool doIt(const Initializer<ParamTypes...>* self, const AbstractInitializer& other)
        {
            return false;
        }
    };

    template <typename... ParamTypes>
    struct Equal<std::enable_if_t<(true && ... && std::is_same_v<decltype(std::declval<ParamTypes>() == std::declval<ParamTypes>()), bool>)>, ParamTypes...>
    {
        static constexpr bool doIt(const Initializer<ParamTypes...>* self, const AbstractInitializer& other)
        {
            const Initializer<ParamTypes...>* concreteOtherPtr = dynamic_cast<const Initializer<ParamTypes...>*>(&other);
            if (concreteOtherPtr == nullptr)
            {
                return false;
            }

            return self->apply([concreteOtherPtr](const ParamTypes&... selfValues) -> bool {
                return concreteOtherPtr->apply([&selfValues...](const ParamTypes&... otherValues) -> bool {
                    return (true && ... && (selfValues == otherValues));
                });
            });
        }
    };
}

template <typename... ParamTypes>
bool
Initializer<ParamTypes...>::operator==(const AbstractInitializer& other) const
{
    return InitializerDetails::Equal<void, ParamTypes...>::doIt(this, other);
}
