#pragma once

#include <rtti/Rtti.h>

template <typename ScalarType>
struct TrainingState;

template <typename ScalarType>
class TrainingNotifier
{
public:
    virtual ~TrainingNotifier() = default;

    virtual void notify(const TrainingState<ScalarType>& state) = 0;

    DECLARE_AS_RTTI_BASE_TYPE(TrainingNotifier<ScalarType>)
};
