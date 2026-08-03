#pragma once

#include <network/NeuralNetwork.h>

template <typename ValueType>
class AbstractNetworkBuilder;
template <typename ValueType>
class LearningMethod;
template <typename ValueType>
class TrainingMonitor;
template <typename ValueType>
class TrainingNotifier;

template <typename ValueType>
struct TrainingNeuralNetworkPimpl;

template <typename ValueType>
class TrainingNeuralNetwork : public NeuralNetwork<ValueType>
{
public:
    TrainingNeuralNetwork(size_t thickness, const AbstractRtti<AbstractNetworkBuilder<ValueType>>& costNetworkBuilderRtti, const LearningMethod<ValueType>& learningMethod);
    TrainingNeuralNetwork(const TrainingNeuralNetwork& other) = delete;
    TrainingNeuralNetwork(TrainingNeuralNetwork&& other);

    virtual ~TrainingNeuralNetwork();

    TrainingNeuralNetwork& operator=(const TrainingNeuralNetwork& other) = delete;
    TrainingNeuralNetwork& operator=(TrainingNeuralNetwork&& other);

    virtual void setOutput(AbstractTensorMap<ValueType>* map) override;

    bool canTrain() const;
    void train(TrainingMonitor<ValueType>& monitor, TrainingNotifier<ValueType>& notifier);

private:
    TrainingNeuralNetworkPimpl<ValueType>* m_pimpl;
};
