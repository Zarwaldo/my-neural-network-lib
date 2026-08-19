#pragma once

template <typename ScalarType>
struct TrainingState
{
    ScalarType currentCost;
    ScalarType currentCostOnTestingData;
    double timeElapsed;
};
