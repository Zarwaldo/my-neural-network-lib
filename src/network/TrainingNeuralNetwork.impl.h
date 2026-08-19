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

template <typename ScalarType>
struct TrainingNeuralNetworkPimpl
{
    TrainingNeuralNetworkPimpl(const AbstractRtti<AbstractNetworkBuilder<ScalarType>>& costNetworkBuilderRtti, const LearningMethod<ScalarType>& learningMethod, AbstractTensor<ScalarType>& costTensor, const AbstractNetworkBuilder<ScalarType>* expectedTensorMapNetworkBuilder, AbstractTensorMap<ScalarType>* expectedTensorMap, AbstractNetworkBuilder<ScalarType>* costNetworkBuilder)
        : m_costNetworkBuilderRtti(costNetworkBuilderRtti)
        , m_learningMethod(learningMethod)
        , m_costTensor(costTensor)
        , m_expectedTensorMapNetworkBuilder(expectedTensorMapNetworkBuilder)
        , m_expectedTensorMap(expectedTensorMap)
        , m_costNetworkBuilder(costNetworkBuilder)
    {}

    const AbstractRtti<AbstractNetworkBuilder<ScalarType>>& m_costNetworkBuilderRtti;
    const LearningMethod<ScalarType>& m_learningMethod;
    AbstractTensor<ScalarType>& m_costTensor;
    const AbstractNetworkBuilder<ScalarType>* m_expectedTensorMapNetworkBuilder;
    AbstractTensorMap<ScalarType>* m_expectedTensorMap;
    const AbstractNetworkBuilder<ScalarType>* m_costNetworkBuilder;
};

template <typename ScalarType>
TrainingNeuralNetwork<ScalarType>::TrainingNeuralNetwork(size_t thickness, const AbstractRtti<AbstractNetworkBuilder<ScalarType>>& costNetworkBuilderRtti, const LearningMethod<ScalarType>& learningMethod)
    : NeuralNetwork<ScalarType>(thickness)
    , m_pimpl(new TrainingNeuralNetworkPimpl<ScalarType>(costNetworkBuilderRtti, learningMethod, addTensor(TensorIndex<0>(true), false), nullptr, nullptr, nullptr))
{}

template <typename ScalarType>
TrainingNeuralNetwork<ScalarType>::TrainingNeuralNetwork(TrainingNeuralNetwork&& other)
    : NeuralNetwork<ScalarType>(std::move(other))
    , m_pimpl(other.m_pimpl)
{
    other.m_pimpl = nullptr;
}

template <typename ScalarType>
TrainingNeuralNetwork<ScalarType>::~TrainingNeuralNetwork()
{
    delete m_pimpl;
}

template <typename ScalarType>
TrainingNeuralNetwork<ScalarType>&
TrainingNeuralNetwork<ScalarType>::operator=(TrainingNeuralNetwork&& other)
{
    NeuralNetwork<ScalarType>::operator=(std::move(other));
    std::swap(m_pimpl, other.m_pimpl);
    return *this;
}

template <typename ScalarType>
void
TrainingNeuralNetwork<ScalarType>::setOutput(AbstractTensorMap<ScalarType>* map)
{
    NeuralNetwork<ScalarType>::setOutput(map);

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
        *DuplicateTensorMapBuilder<ScalarType>::getRtti(),
        Initializer<
            NeuralNetwork<ScalarType>&,
            const AbstractTensorMap<ScalarType>&,
            const std::map<const AbstractTensor<ScalarType>*, AbstractTensor<ScalarType>*>&
        >(
            *this,
            *map,
            {}
        ),
        &m_pimpl->m_expectedTensorMapNetworkBuilder
    );
    m_pimpl->m_expectedTensorMap = static_cast<AbstractTensorMap<ScalarType>*>(expectedTensorMapBuildingResult[DuplicateTensorMapBuilder<ScalarType>::addedTensorMapKey]);

    build(
        m_pimpl->m_costNetworkBuilderRtti,
        Initializer<
            NeuralNetwork<ScalarType>&,
            AbstractTensorMap<ScalarType>&,
            AbstractTensorMap<ScalarType>&,
            AbstractTensor<ScalarType>&
        >(
            *this,
            *map,
            *m_pimpl->m_expectedTensorMap,
            m_pimpl->m_costTensor
        ),
        &m_pimpl->m_costNetworkBuilder
    );
}

template <typename ScalarType>
bool
TrainingNeuralNetwork<ScalarType>::canTrain() const
{
    return (m_pimpl->m_expectedTensorMapNetworkBuilder != nullptr) && (m_pimpl->m_costNetworkBuilder != nullptr) && canExecute();
}

template <typename ScalarType>
void
TrainingNeuralNetwork<ScalarType>::train(TrainingMonitor<ScalarType>& monitor, TrainingNotifier<ScalarType>& notifier)
{
    if (!canTrain())
        throw std::runtime_error("TrainingNeuralNetwork::train: The network cannot yet train.");

    // Initialize training data
    const double startingTime = std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
    TrainingState<ScalarType> trainingState = { std::numeric_limits<ScalarType>::max(), std::numeric_limits<ScalarType>::max(), 0.0f};

    // Allocate backpropagation network
    NeuralNetwork<ScalarType> backPropagationNetwork(0);

    // Allocate backpropagation tensors in the backpropagation network
    std::map<const AbstractTensor<ScalarType>*, AbstractTensor<ScalarType>*> backpropagationTensors;
    for (AbstractTensor<ScalarType>* tensor : getTensors())
    {
        backpropagationTensors[tensor] = &backPropagationNetwork.addTensor(tensor->sizes(), false);
    }
    for (Module<ScalarType>* module : getModules())
    {
        for (AbstractTensor<ScalarType>& tensor : module->getParameterAbstractTensorMap())
        {
            backpropagationTensors[&tensor] = &backPropagationNetwork.addTensor(tensor.sizes(), false);
        }
    }

    // Allocate backpropagation tensor maps in the backpropagation network
    std::map<const AbstractTensorMap<ScalarType>*, AbstractTensorMap<ScalarType>*> backpropagationTensorMaps;
    for (const AbstractTensorMap<ScalarType>* tensorMap : getTensorMaps())
    {
        std::map<std::string, void*> tensorMapDuplicationResult = backPropagationNetwork.build(
            *DuplicateTensorMapBuilder<ScalarType>::getRtti(),
            Initializer<
                NeuralNetwork<ScalarType>&,
                const AbstractTensorMap<ScalarType>&,
                const std::map<const AbstractTensor<ScalarType>*, AbstractTensor<ScalarType>*>&
            >(
                backPropagationNetwork,
                *tensorMap,
                backpropagationTensors
            )
        );
        backpropagationTensorMaps[tensorMap] = static_cast<AbstractTensorMap<ScalarType>*>(tensorMapDuplicationResult[DuplicateTensorMapBuilder<ScalarType>::addedTensorMapKey]);
    }
    for (Module<ScalarType>* module : getModules())
    {
        const AbstractTensorMap<ScalarType>& parameterTensorMap = module->getParameterAbstractTensorMap();
        std::map<std::string, void*> tensorMapDuplicationResult = backPropagationNetwork.build(
            *DuplicateTensorMapBuilder<ScalarType>::getRtti(),
            Initializer<
                NeuralNetwork<ScalarType>&,
                const AbstractTensorMap<ScalarType>&,
                const std::map<const AbstractTensor<ScalarType>*, AbstractTensor<ScalarType>*>&
            >(
                backPropagationNetwork,
                parameterTensorMap,
                backpropagationTensors
            )
        );
        backpropagationTensorMaps[&parameterTensorMap] = static_cast<AbstractTensorMap<ScalarType>*>(tensorMapDuplicationResult[DuplicateTensorMapBuilder<ScalarType>::addedTensorMapKey]);
    }

    // Iteration loops
    while (!monitor.isTrainingOver(trainingState))
    {
        // Initialize iteration
        TrainingIterationStatus iterationStatus = monitor.trainingIteration(thickness(), *getInput(), *m_pimpl->m_expectedTensorMap);

        // Compute iteration
        NullInputProvider<ScalarType> inputProvider;
        NullOutputReceiver<ScalarType> outputReceiver;
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
            Tensor<ScalarType, 0>& costBackpropagationTensor = static_cast<Tensor<ScalarType, 0>&>(*backpropagationTensors[&m_pimpl->m_costTensor]);
            costBackpropagationTensor[TensorIndex<0>(true)] = static_cast<ScalarType>(1.0f);

            // Backpropagate all modules to compute all cost's partial derivatives
            const std::vector<Module<ScalarType>*>& modules = getModules();
            for (long long int moduleIndex = modules.size() - 1; moduleIndex >= 0; moduleIndex--)
            {
                Module<ScalarType>* module = modules[moduleIndex];

                module->backpropagate(
                    *backpropagationTensorMaps[module->getInputAbstractTensorMap()],
                    *backpropagationTensorMaps[&module->getParameterAbstractTensorMap()],
                    *backpropagationTensorMaps[module->getOutputAbstractTensorMap()]
                );
            }

            // Learn: adapt all modules' parameters according to the cost's partial derivatives
            for (Module<ScalarType>* module : getModules())
            {
                AbstractTensorMap<ScalarType>& parameterTensorMap = module->getParameterAbstractTensorMap();
                for (AbstractTensor<ScalarType>& parameterTensor : parameterTensorMap)
                {
                    m_pimpl->m_learningMethod.learn(parameterTensor, *backpropagationTensors[&parameterTensor]);
                }
            }
        }
    }
}
