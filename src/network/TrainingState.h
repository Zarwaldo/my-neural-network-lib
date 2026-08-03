#pragma once

template <typename ValueType>
struct TrainingState
{
    ValueType currentCost;
    ValueType currentCostOnTestingData;
    double timeElapsed;
};
