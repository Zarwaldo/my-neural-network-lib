#define _USE_MATH_DEFINES

#include <gtest/gtest.h>

#include <network/LearningMethod.h>
#include <network/NullParamTensorFiller.h>
#include <network/TrainingNeuralNetwork.h>
#include <network/TrainingMonitor.h>
#include <network/TrainingNotifier.h>
#include <network/TrainingState.h>

#include <pluginloader/PluginLoader.h>

#include <plugins/natives/modules/perceptronmodule/PerceptronModule.h>
#include <plugins/natives/networkbuilders/EuclidianDistanceCostBuilder.h>
#include <plugins/natives/tensormaps/TensorMapKeyEnums.h>

#include <rtti/Rtti.h>

#include <tensor/Tensor.h>
#include <tensor/TensorMap.h>

#include <cmath>
#include <filesystem>

template <typename ScalarType, size_t Dimension>
class SampleTrainingMonitor : public TrainingMonitor<ScalarType>
{
public:
    SampleTrainingMonitor(ScalarType factor, ScalarType offset)
        : m_factor(factor)
        , m_offset(offset)
        , m_step(0)
    {}

    virtual ~SampleTrainingMonitor()
    {}

    virtual bool isTrainingOver(const TrainingState<ScalarType>& state) const override
    {
        // Give the training 30s to reach a training cost of 10^-5
        return state.currentCostOnTestingData < static_cast<ScalarType>(1.0E-5f) || state.timeElapsed > 30.0f;
    }

    virtual TrainingIterationStatus trainingIteration(size_t thickness, AbstractTensorMap<ScalarType>& input, AbstractTensorMap<ScalarType>& expected) override
    {
        Tensor<ScalarType, Dimension>& inputTensor = dynamic_cast<Tensor<ScalarType, Dimension>&>(input.get(0));
        Tensor<ScalarType, Dimension>& expectedTensor = dynamic_cast<Tensor<ScalarType, Dimension>&>(expected.get(0));

        for (const RawTensorIndex<Dimension>& index : inputTensor)
        {
            const float randomInput = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            const float expectedOutput = m_factor * randomInput + m_offset;

            inputTensor[TensorIndex<Dimension>(index)] = randomInput;
            expectedTensor[TensorIndex<Dimension>(index)] = expectedOutput;
        }

        m_step = (++m_step) % 10;
        return (m_step == 0) ? TESTING : TRAINING;
    }

private:
    ScalarType m_factor;
    ScalarType m_offset;
    size_t m_step;

    DECLARE_RTTI(TrainingMonitor<ScalarType>)
};

IMPLEMENT_RTTI(SampleTrainingMonitor, TrainingMonitor<ScalarType>, PACK(typename, size_t), PACK(ScalarType, Dimension))

template <typename ScalarType>
class SampleTrainingNotifier : public TrainingNotifier<ScalarType>
{
public:
    SampleTrainingNotifier()
    {}

    virtual ~SampleTrainingNotifier()
    {}

    virtual void notify(const TrainingState<ScalarType>& state) override
    {
        m_lastTrainingState = state;
    }

    const TrainingState<ScalarType>& getLastTrainingState() const
    {
        return m_lastTrainingState;
    }

    TrainingState<ScalarType> m_lastTrainingState;

    DECLARE_RTTI(TrainingNotifier<ScalarType>)
};

IMPLEMENT_RTTI(SampleTrainingNotifier, TrainingNotifier<ScalarType>, PACK(typename), PACK(ScalarType))

template <typename ScalarType, size_t Dimension>
class SampleLearningMethod : public LearningMethod<ScalarType>
{
public:
    SampleLearningMethod(ScalarType learningRate)
        : m_learningRate(-learningRate)
    {
        if (learningRate < ScalarType{})
        {
            throw std::runtime_error("SampleLearningMethod: The learning rate must be non-negative.");
        }
    }

    virtual ~SampleLearningMethod()
    {}

    virtual void learn(AbstractTensor<ScalarType>& parameterTensor, const AbstractTensor<ScalarType>& costPartDerivWRTParamTensor) const override
    {
        if (parameterTensor.sizes() != costPartDerivWRTParamTensor.sizes())
        {
            throw std::runtime_error("SampleLearningMethod::learn: Both input tensors must have the same size.");
        }

        RawTensor<ScalarType, Dimension>& concreteParameterTensor = dynamic_cast<Tensor<ScalarType, Dimension>&>(parameterTensor).getRawTensor();
        const RawTensor<ScalarType, Dimension>& concreteCostPartDerivWRTParamTensor = dynamic_cast<const Tensor<ScalarType, Dimension>&>(costPartDerivWRTParamTensor).getRawTensor();
        for (const RawTensorIndex<Dimension>& index : concreteParameterTensor)
        {
            concreteParameterTensor[index] += m_learningRate * concreteCostPartDerivWRTParamTensor[index];
        }
    }

private:
    ScalarType m_learningRate;

    DECLARE_RTTI(LearningMethod<ScalarType>)
};

IMPLEMENT_RTTI(SampleLearningMethod, LearningMethod<ScalarType>, PACK(typename, size_t), PACK(ScalarType, Dimension))

class TrainingNeuralNetworkShould : public ::testing::Test
{
protected:
    void SetUp() override
    {
        pluginLoader = new PluginLoader();

        trainingMonitor = new SampleTrainingMonitor<float, 1>(M_PI, M_E);
        trainingNotifier = new SampleTrainingNotifier<float>;
    }

    void TearDown() override
    {
        delete pluginLoader;
        delete trainingMonitor;
        delete trainingNotifier;
    }

protected:
    PluginLoader* pluginLoader;
    SampleTrainingMonitor<float, 1>* trainingMonitor;
    SampleTrainingNotifier<float>* trainingNotifier;
};

TEST_F(TrainingNeuralNetworkShould, performElementaryLinearRegression) {
    // Given a training neural network
    SampleLearningMethod<float, 0> learningMethod(.05f);
    TrainingNeuralNetwork<float> trainingNeuralNetwork(10, *EuclidianDistanceCostBuilder<float, 0>::getRtti(), learningMethod);

    AbstractTensor<float>& inputTensor = trainingNeuralNetwork.addTensor(TensorIndex<0>(true));
    AbstractTensorMap<float>& inputTensorMap = trainingNeuralNetwork.addTensorMap(new TensorMap<float, TensorSingleton>{&inputTensor});
    AbstractTensor<float>& outputTensor = trainingNeuralNetwork.addTensor(TensorIndex<0>(true));
    AbstractTensorMap<float>& outputTensorMap = trainingNeuralNetwork.addTensorMap(new TensorMap<float, TensorSingleton>{&outputTensor});

    Module<float>& perceptron = trainingNeuralNetwork.addModule(
        *PerceptronModule<float, 0, 0>::getRtti(),
        Initializer<const RawTuple<const TensorIndex<0>&, const TensorIndex<0>&>&>(makeRawTuple(TensorIndex<0>(true), TensorIndex<0>(true))),
        inputTensorMap,
        outputTensorMap,
        NullParamTensorFiller<float>{}
    );

    trainingNeuralNetwork.setInput(&inputTensorMap);
    trainingNeuralNetwork.setOutput(&outputTensorMap);

    // When training the neural network
    trainingNeuralNetwork.train(*trainingMonitor, *trainingNotifier);

    // Then the training reached its objective of 10^-5 of training cost
    const TrainingState<float>& trainingState = trainingNotifier->getLastTrainingState();
    EXPECT_LE(trainingState.currentCostOnTestingData, 1.0E-5f);
    // And the parameters of the network have converged to get close to the training data
    const TensorMap<float, PerceptronParamsKeyEnum>& parameters = static_cast<const TensorMap<float, PerceptronParamsKeyEnum>&>(perceptron.getParameterAbstractTensorMap());
    const Tensor<float, 0>& weights = static_cast<const Tensor<float, 0>&>(parameters.get(PerceptronParamsKeyEnum::Values::WEIGHTS));
    const Tensor<float, 0>& biases = static_cast<const Tensor<float, 0>&>(parameters.get(PerceptronParamsKeyEnum::Values::BIASES));
    // Training data followed the equation y = pi * x + e
    EXPECT_NEAR(weights[TensorIndex<0>(true)], M_PI, M_PI * 0.05f);
    EXPECT_NEAR(biases[TensorIndex<0>(true)], M_E, M_E * 0.05f);
}
