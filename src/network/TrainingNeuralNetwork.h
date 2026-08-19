#pragma once

#include <network/NeuralNetwork.h>

template <typename ScalarType>
class AbstractNetworkBuilder;
template <typename ScalarType>
class LearningMethod;
template <typename ScalarType>
class TrainingMonitor;
template <typename ScalarType>
class TrainingNotifier;

template <typename ScalarType>
struct TrainingNeuralNetworkPimpl;

template <typename ScalarType>
class TrainingNeuralNetwork : public NeuralNetwork<ScalarType>
{
public:
    TrainingNeuralNetwork(size_t thickness, const AbstractRtti<AbstractNetworkBuilder<ScalarType>>& costNetworkBuilderRtti, const LearningMethod<ScalarType>& learningMethod);
    TrainingNeuralNetwork(const TrainingNeuralNetwork& other) = delete;
    TrainingNeuralNetwork(TrainingNeuralNetwork&& other);

    virtual ~TrainingNeuralNetwork();

    TrainingNeuralNetwork& operator=(const TrainingNeuralNetwork& other) = delete;
    TrainingNeuralNetwork& operator=(TrainingNeuralNetwork&& other);

    virtual void setOutput(AbstractTensorMap<ScalarType>* map) override;

    bool canTrain() const;
    void train(TrainingMonitor<ScalarType>& monitor, TrainingNotifier<ScalarType>& notifier);

private:
    TrainingNeuralNetworkPimpl<ScalarType>* m_pimpl;
};
