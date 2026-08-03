#pragma once

#include <rtti/Rtti.h>

template <typename ValueType>
class OneInputProvider;
template <typename ValueType>
struct TrainingState;

enum TrainingIterationStatus
{
    TRAINING,
    TESTING,
    INVALID
};

template <typename ValueType>
class TrainingMonitor
{
public:
    virtual ~TrainingMonitor() = default;

    virtual bool isTrainingOver(const TrainingState<ValueType>& state) const = 0;
    virtual TrainingIterationStatus trainingIteration(size_t thickness, AbstractTensorMap<ValueType>& input, AbstractTensorMap<ValueType>& expected) = 0;

    DECLARE_AS_RTTI_BASE_TYPE(TrainingMonitor<ValueType>)
};
