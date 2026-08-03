#pragma once

#include <rtti/Rtti.h>

template <typename ValueType>
struct TrainingState;

template <typename ValueType>
class TrainingNotifier
{
public:
    virtual ~TrainingNotifier() = default;

    virtual void notify(const TrainingState<ValueType>& state) = 0;

    DECLARE_AS_RTTI_BASE_TYPE(TrainingNotifier<ValueType>)
};
