#pragma once

#include <network/TrainingNeuralNetwork.h>

#include <module/Module.h>

#include <network/AbstractNetworkBuilder.h>
#include <network/DuplicateTensorMapBuilder.h>
#include <network/LearningMethod.h>
#include <network/NullInputProvider.h>
#include <network/NullOutputReceiver.h>
#include <network/TrainingMonitor.h>
#include <network/TrainingNotifier.h>
#include <network/TrainingState.h>

#include <rtti/Rtti.h>

#include <tensor/Tensor.h>
#include <tensor/TensorMap.h>

#include <chrono>

template <typename ValueType>
struct TrainingNeuralNetworkPimpl
{
    TrainingNeuralNetworkPimpl(const AbstractRtti<AbstractNetworkBuilder<ValueType>>& costNetworkBuilderRtti, const LearningMethod<ValueType>& learningMethod, AbstractTensor<ValueType>& costTensor, const AbstractNetworkBuilder<ValueType>* expectedTensorMapNetworkBuilder, AbstractTensorMap<ValueType>* expectedTensorMap, AbstractNetworkBuilder<ValueType>* costNetworkBuilder)
        : m_costNetworkBuilderRtti(costNetworkBuilderRtti)
        , m_learningMethod(learningMethod)
        , m_costTensor(costTensor)
        , m_expectedTensorMapNetworkBuilder(expectedTensorMapNetworkBuilder)
        , m_expectedTensorMap(expectedTensorMap)
        , m_costNetworkBuilder(costNetworkBuilder)
    {}

    const AbstractRtti<AbstractNetworkBuilder<ValueType>>& m_costNetworkBuilderRtti;
    const LearningMethod<ValueType>& m_learningMethod;
    AbstractTensor<ValueType>& m_costTensor;
    const AbstractNetworkBuilder<ValueType>* m_expectedTensorMapNetworkBuilder;
    AbstractTensorMap<ValueType>* m_expectedTensorMap;
    const AbstractNetworkBuilder<ValueType>* m_costNetworkBuilder;
};

template <typename ValueType>
TrainingNeuralNetwork<ValueType>::TrainingNeuralNetwork(size_t thickness, const AbstractRtti<AbstractNetworkBuilder<ValueType>>& costNetworkBuilderRtti, const LearningMethod<ValueType>& learningMethod)
    : NeuralNetwork<ValueType>(thickness)
    , m_pimpl(new TrainingNeuralNetworkPimpl<ValueType>(costNetworkBuilderRtti, learningMethod, addTensor(TensorIndex<0>(true), false), nullptr, nullptr, nullptr))
{}

template <typename ValueType>
TrainingNeuralNetwork<ValueType>::TrainingNeuralNetwork(TrainingNeuralNetwork&& other)
    : NeuralNetwork<ValueType>(std::move(other))
    , m_pimpl(other.m_pimpl)
{
    other.m_pimpl = nullptr;
}

template <typename ValueType>
TrainingNeuralNetwork<ValueType>::~TrainingNeuralNetwork()
{
    delete m_pimpl;
}

template <typename ValueType>
TrainingNeuralNetwork<ValueType>&
TrainingNeuralNetwork<ValueType>::operator=(TrainingNeuralNetwork&& other)
{
    NeuralNetwork<ValueType>::operator=(std::move(other));
    std::swap(m_pimpl, other.m_pimpl);
    return *this;
}

template <typename ValueType>
void
TrainingNeuralNetwork<ValueType>::setOutput(AbstractTensorMap<ValueType>* map)
{
    NeuralNetwork<ValueType>::setOutput(map);

    if (m_pimpl->m_costNetworkBuilder != nullptr)
        unbuild(*m_pimpl->m_costNetworkBuilder);
    m_pimpl->m_costNetworkBuilder = nullptr;

    if (m_pimpl->m_expectedTensorMapNetworkBuilder != nullptr)
        unbuild(*m_pimpl->m_expectedTensorMapNetworkBuilder);
    m_pimpl->m_expectedTensorMapNetworkBuilder = nullptr;
    m_pimpl->m_expectedTensorMap = nullptr;

    if (map == nullptr)
        return;

    std::map<std::string, void*> expectedTensorMapBuildingResult = build(
        *DuplicateTensorMapBuilder<ValueType>::getRtti(),
        Initializer<
            NeuralNetwork<ValueType>&,
            const AbstractTensorMap<ValueType>&,
            const std::map<const AbstractTensor<ValueType>*, AbstractTensor<ValueType>*>&
        >(
            *this,
            *map,
            {}
        ),
        &m_pimpl->m_expectedTensorMapNetworkBuilder
    );
    m_pimpl->m_expectedTensorMap = static_cast<AbstractTensorMap<ValueType>*>(expectedTensorMapBuildingResult[DuplicateTensorMapBuilder<ValueType>::addedTensorMapKey]);

    build(
        m_pimpl->m_costNetworkBuilderRtti,
        Initializer<
            NeuralNetwork<ValueType>&,
            AbstractTensorMap<ValueType>&,
            AbstractTensorMap<ValueType>&,
            AbstractTensor<ValueType>&
        >(
            *this,
            *map,
            *m_pimpl->m_expectedTensorMap,
            m_pimpl->m_costTensor
        ),
        &m_pimpl->m_costNetworkBuilder
    );
}

template <typename ValueType>
bool
TrainingNeuralNetwork<ValueType>::canTrain() const
{
    return (m_pimpl->m_expectedTensorMapNetworkBuilder != nullptr) && (m_pimpl->m_costNetworkBuilder != nullptr) && canExecute();
}

template <typename ValueType>
void
TrainingNeuralNetwork<ValueType>::train(TrainingMonitor<ValueType>& monitor, TrainingNotifier<ValueType>& notifier)
{
    if (!canTrain())
        throw std::runtime_error("TrainingNeuralNetwork::train: The network cannot yet train.");

    // Initialize training data
    const double startingTime = std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
    TrainingState<ValueType> trainingState = { std::numeric_limits<ValueType>::max(), std::numeric_limits<ValueType>::max(), 0.0f};

    // Allocate backpropagation network
    NeuralNetwork<ValueType> backPropagationNetwork(0);

    // Allocate backpropagation tensors in the backpropagation network
    std::map<const AbstractTensor<ValueType>*, AbstractTensor<ValueType>*> backpropagationTensors;
    for (AbstractTensor<ValueType>* tensor : getTensors())
    {
        backpropagationTensors[tensor] = &backPropagationNetwork.addTensor(tensor->sizes(), false);
    }
    for (Module<ValueType>* module : getModules())
    {
        for (AbstractTensor<ValueType>& tensor : module->getParameterAbstractTensorMap())
        {
            backpropagationTensors[&tensor] = &backPropagationNetwork.addTensor(tensor.sizes(), false);
        }
    }

    // Allocate backpropagation tensor maps in the backpropagation network
    std::map<const AbstractTensorMap<ValueType>*, AbstractTensorMap<ValueType>*> backpropagationTensorMaps;
    for (const AbstractTensorMap<ValueType>* tensorMap : getTensorMaps())
    {
        std::map<std::string, void*> tensorMapDuplicationResult = backPropagationNetwork.build(
            *DuplicateTensorMapBuilder<ValueType>::getRtti(),
            Initializer<
                NeuralNetwork<ValueType>&,
                const AbstractTensorMap<ValueType>&,
                const std::map<const AbstractTensor<ValueType>*, AbstractTensor<ValueType>*>&
            >(
                backPropagationNetwork,
                *tensorMap,
                backpropagationTensors
            )
        );
        backpropagationTensorMaps[tensorMap] = static_cast<AbstractTensorMap<ValueType>*>(tensorMapDuplicationResult[DuplicateTensorMapBuilder<ValueType>::addedTensorMapKey]);
    }
    for (Module<ValueType>* module : getModules())
    {
        const AbstractTensorMap<ValueType>& parameterTensorMap = module->getParameterAbstractTensorMap();
        std::map<std::string, void*> tensorMapDuplicationResult = backPropagationNetwork.build(
            *DuplicateTensorMapBuilder<ValueType>::getRtti(),
            Initializer<
                NeuralNetwork<ValueType>&,
                const AbstractTensorMap<ValueType>&,
                const std::map<const AbstractTensor<ValueType>*, AbstractTensor<ValueType>*>&
            >(
                backPropagationNetwork,
                parameterTensorMap,
                backpropagationTensors
            )
        );
        backpropagationTensorMaps[&parameterTensorMap] = static_cast<AbstractTensorMap<ValueType>*>(tensorMapDuplicationResult[DuplicateTensorMapBuilder<ValueType>::addedTensorMapKey]);
    }

    // Iteration loops
    while (!monitor.isTrainingOver(trainingState))
    {
        // Initialize iteration
        TrainingIterationStatus iterationStatus = monitor.trainingIteration(thickness(), *getInput(), *m_pimpl->m_expectedTensorMap);

        // Compute iteration
        NullInputProvider<ValueType> inputProvider;
        NullOutputReceiver<ValueType> outputReceiver;
        execute(inputProvider, outputReceiver);

        // Update training data
        switch (iterationStatus)
        {
            case TRAINING:
                trainingState.currentCost = m_pimpl->m_costTensor[TensorIndex<0>(true)];
                break;
            case TESTING:
                trainingState.currentCostOnTestingData = m_pimpl->m_costTensor[TensorIndex<0>(true)];
                break;
            default:
                throw std::runtime_error("TrainingNeuralNetwork::train: Iteration of invalid status " + std::to_string(iterationStatus) + ".");
                break;
        }
        double currentTime = std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
        trainingState.timeElapsed = currentTime - startingTime;

        // Notify the training progress
        notifier.notify(trainingState);

        // Learning process in the training case
        if (iterationStatus == TRAINING)
        {
            // Partial derivative of the cost with respect to itself is always one (initial value for backpropagation)
            Tensor<ValueType, 0>& costBackpropagationTensor = static_cast<Tensor<ValueType, 0>&>(*backpropagationTensors[&m_pimpl->m_costTensor]);
            costBackpropagationTensor[TensorIndex<0>(true)] = static_cast<ValueType>(1.0f);

            // Backpropagate all modules to compute all cost's partial derivatives
            const std::vector<Module<ValueType>*>& modules = getModules();
            for (long long int moduleIndex = modules.size() - 1; moduleIndex >= 0; moduleIndex--)
            {
                Module<ValueType>* module = modules[moduleIndex];

                module->backpropagate(
                    *backpropagationTensorMaps[module->getInputAbstractTensorMap()],
                    *backpropagationTensorMaps[&module->getParameterAbstractTensorMap()],
                    *backpropagationTensorMaps[module->getOutputAbstractTensorMap()]
                );
            }

            // Learn: adapt all modules' parameters according to the cost's partial derivatives
            for (Module<ValueType>* module : getModules())
            {
                AbstractTensorMap<ValueType>& parameterTensorMap = module->getParameterAbstractTensorMap();
                for (AbstractTensor<ValueType>& parameterTensor : parameterTensorMap)
                {
                    m_pimpl->m_learningMethod.learn(parameterTensor, *backpropagationTensors[&parameterTensor]);
                }
            }
        }
    }
}
