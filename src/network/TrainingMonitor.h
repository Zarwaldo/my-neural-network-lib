#pragma once

#include <rtti/Rtti.h>

template <typename ScalarType>
class OneInputProvider;
template <typename ScalarType>
struct TrainingState;

enum TrainingIterationStatus
{
    TRAINING,
    TESTING,
    INVALID
};

template <typename ScalarType>
class TrainingMonitor
{
public:
    virtual ~TrainingMonitor() = default;

    virtual bool isTrainingOver(const TrainingState<ScalarType>& state) const = 0;
    virtual TrainingIterationStatus trainingIteration(size_t thickness, AbstractTensorMap<ScalarType>& input, AbstractTensorMap<ScalarType>& expected) = 0;

    DECLARE_AS_RTTI_BASE_TYPE(TrainingMonitor<ScalarType>)
};
