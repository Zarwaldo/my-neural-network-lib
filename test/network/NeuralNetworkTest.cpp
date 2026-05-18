#include <gtest/gtest.h>

#include <network/NeuralNetwork.h>
#include <network/ParamTensorFiller.h>
#include <network/InputProvider.h>
#include <network/OutputReceiver.h>

#include <helpers/RawTuple.h>

#include <module/MainTensorMapKeyEnums.h>

#include <plugincore/ResourcesContainer.h>

#include <pluginloader/PluginLoader.h>

#include <plugins/nativemodules/AdditionModule.h>

#include <tensor/TensorIndex.h>
#include <tensor/TensorMap.h>

#include <filesystem>

class SampleParamTensorFiller : public ParamTensorFiller<float>
{
public:
    SampleParamTensorFiller(float value0, float value1, float value2, float value3, float value4)
    {
        m_values[0] = value0;
        m_values[1] = value1;
        m_values[2] = value2;
        m_values[3] = value3;
        m_values[4] = value4;
    }

    virtual ~SampleParamTensorFiller()
    {}

    virtual void fill(AbstractTensorMap<float>& tensorMap) const override
    {
        TensorMap<float, TensorSingleton>* concreteTensorMap = dynamic_cast<TensorMap<float, TensorSingleton>*>(&tensorMap);
        if (concreteTensorMap == nullptr)
            return;

        AbstractTensor<float>& abstractTensor = concreteTensorMap->get(SINGLE_TENSOR);
        Tensor<float, 1>* concreteTensor = dynamic_cast<Tensor<float, 1>*>(&abstractTensor);
        if (concreteTensor == nullptr)
            return;

        for (const TensorIndex<1>& index : *concreteTensor)
        {
            (*concreteTensor)[index] = m_values[index[0]];
        }
    }

    DECLARE_RTTI(ParamTensorFiller<float>)

private:
    float m_values[5];
};

IMPLEMENT_RTTI(SampleParamTensorFiller, ParamTensorFiller<float>, PACK(), PACK())

class SampleInputProvider : public InputProvider<float>
{
public:
    SampleInputProvider(const std::vector<std::tuple<float, float, float, float, float>>& valuesStack)
        : m_valuesStack(valuesStack)
    {}

    virtual ~SampleInputProvider() override
    {}

    virtual void getNewInput(AbstractTensorMap<float>& inputMap) override
    {
        TensorMap<float, TensorSingleton>* concreteInputMap = dynamic_cast<TensorMap<float, TensorSingleton>*>(&inputMap);
        if (concreteInputMap == nullptr)
            return;

        Tensor<float, 2>* concreteInputTensor = dynamic_cast<Tensor<float, 2>*>(&concreteInputMap->get(SINGLE_TENSOR));
        if (concreteInputTensor == nullptr)
            return;

        const size_t nbInputs = concreteInputTensor->sizes()[0];
        if (!hasMoreThan(nbInputs))
            return;

        for (size_t inputsIndex = 0; inputsIndex < nbInputs; ++inputsIndex)
        {
            const std::tuple<float, float, float, float, float>& values = m_valuesStack.front();

            (*concreteInputTensor)[TensorIndex<2>{inputsIndex, 0}] = std::get<0>(values);
            (*concreteInputTensor)[TensorIndex<2>{inputsIndex, 1}] = std::get<1>(values);
            (*concreteInputTensor)[TensorIndex<2>{inputsIndex, 2}] = std::get<2>(values);
            (*concreteInputTensor)[TensorIndex<2>{inputsIndex, 3}] = std::get<3>(values);
            (*concreteInputTensor)[TensorIndex<2>{inputsIndex, 4}] = std::get<4>(values);

            m_valuesStack.erase(m_valuesStack.begin());
        }
    }

    virtual bool hasMoreThan(size_t nbInputs) const override
    {
        return m_valuesStack.size() >= nbInputs;
    }

    DECLARE_RTTI(InputProvider<float>)

private:
    std::vector<std::tuple<float, float, float, float, float>> m_valuesStack;
};

IMPLEMENT_RTTI(SampleInputProvider, InputProvider<float>, PACK(), PACK())

class SampleOutputReceiver : public OutputReceiver<float>
{
public:
    SampleOutputReceiver()
        : m_results()
    {}

    virtual ~SampleOutputReceiver() override
    {}

    virtual void sendOutput(const AbstractTensorMap<float>& outputMap)
    {
        const TensorMap<float, TensorSingleton>* concreteOutputMap = dynamic_cast<const TensorMap<float, TensorSingleton>*>(&outputMap);
        if (concreteOutputMap == nullptr)
            return;

        const Tensor<float, 2>* concreteOutputTensor = dynamic_cast<const Tensor<float, 2>*>(&concreteOutputMap->get(SINGLE_TENSOR));
        if (concreteOutputTensor == nullptr)
            return;

        const size_t nbResults =  concreteOutputTensor->sizes()[0];
        for (size_t resultIndex = 0; resultIndex < nbResults; ++resultIndex)
        {
            m_results.push_back(
                std::make_tuple<float, float, float, float, float>(
                    std::move(float{(*concreteOutputTensor)[TensorIndex<2>{resultIndex, 0}]}),
                    std::move(float{(*concreteOutputTensor)[TensorIndex<2>{resultIndex, 1}]}),
                    std::move(float{(*concreteOutputTensor)[TensorIndex<2>{resultIndex, 2}]}),
                    std::move(float{(*concreteOutputTensor)[TensorIndex<2>{resultIndex, 3}]}),
                    std::move(float{(*concreteOutputTensor)[TensorIndex<2>{resultIndex, 4}]})
                )
            );
        }
    }

    const std::vector<std::tuple<float, float, float, float, float>>& getResults() const
    {
        return m_results;
    }

    DECLARE_RTTI(OutputReceiver<float>)

private:
    std::vector<std::tuple<float, float, float, float, float>> m_results;
};

IMPLEMENT_RTTI(SampleOutputReceiver, OutputReceiver<float>, PACK(), PACK())

class NeuralNetworkShould : public ::testing::Test
{
protected:
    void SetUp() override
    {
        pluginLoader = new PluginLoader();
        const std::filesystem::path libPath = PluginLoader::getExecutableDir() / "nativemodules.dll";
        pluginLoader->loadPluginFromPath(libPath);

        inputProvider = new SampleInputProvider({
            std::make_tuple(0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
            std::make_tuple(1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
            std::make_tuple(2.0f, 2.0f, 2.0f, 2.0f, 2.0f),
            std::make_tuple(3.0f, 3.0f, 3.0f, 3.0f, 3.0f),
            std::make_tuple(4.0f, 4.0f, 4.0f, 4.0f, 4.0f),
            std::make_tuple(5.0f, 5.0f, 5.0f, 5.0f, 5.0f),
            std::make_tuple(6.0f, 6.0f, 6.0f, 6.0f, 6.0f),
            std::make_tuple(7.0f, 7.0f, 7.0f, 7.0f, 7.0f),
            std::make_tuple(8.0f, 8.0f, 8.0f, 8.0f, 8.0f),
            std::make_tuple(9.0f, 9.0f, 9.0f, 9.0f, 9.0f),
            std::make_tuple(10.0f, 10.0f, 10.0f, 10.0f, 10.0f)
        });

        outputReceiver = new SampleOutputReceiver();
    }

    void TearDown() override
    {
        delete pluginLoader;
        delete inputProvider;
        delete outputReceiver;
    }

    NeuralNetwork<float> buildSampleNetwork(size_t thickness, bool addSecondModule = true, bool setInputMap = true, bool setOutputMap = true)
    {
        NeuralNetwork<float> network(thickness);

        AbstractTensor<float>& inputTensor = network.addTensor(TensorIndex<1>{5});
        AbstractTensorMap<float>& inputTensorMap = network.addTensorMap(new TensorMap<float, TensorSingleton>({&inputTensor}));

        AbstractTensor<float>& middleTensor = network.addTensor(TensorIndex<1>{5});
        AbstractTensorMap<float>& middleTensorMap = network.addTensorMap(new TensorMap<float, TensorSingleton>({&middleTensor}));

        AbstractTensor<float>& outputTensor = network.addTensor(TensorIndex<1>{5});
        AbstractTensorMap<float>& outputTensorMap = network.addTensorMap(new TensorMap<float, TensorSingleton>({&outputTensor}));

        const AbstractRtti<Module<float>>* additionModuleRtti = AdditionModule<float, 1>::getRtti();

        const SampleParamTensorFiller firstParamTensorFiller(1.0f, 2.0f, 3.0f, 4.0f, 5.0f);
        network.addModule(*additionModuleRtti, Initializer<const RawTuple<const TensorIndex<1>&>&>(makeRawTuple(TensorIndex<1>{5})), inputTensorMap, middleTensorMap, firstParamTensorFiller);

        if (addSecondModule)
        {
            const SampleParamTensorFiller secondParamTensorFiller(10.0f, 8.0f, 6.0f, 4.0f, 2.0f);
            network.addModule(*additionModuleRtti, Initializer<const RawTuple<const TensorIndex<1>&>&>(makeRawTuple(TensorIndex<1>{5})), middleTensorMap, outputTensorMap, secondParamTensorFiller);
        }

        if (setInputMap)
        {
            network.setInput(inputTensorMap);
        }

        if (setOutputMap)
        {
            network.setOutput(outputTensorMap);
        }

        return network;
    }

protected:
    PluginLoader* pluginLoader;
    SampleInputProvider* inputProvider;
    SampleOutputReceiver* outputReceiver;
};

TEST_F(NeuralNetworkShould, notBeExecutableIfModulesAreMissing) {
    // Given a neural network missing a module.
    const NeuralNetwork<float> network = buildSampleNetwork(3, false, true, true);

    // When looking whether the network can be executed.
    const bool result = network.canExecute();

    // Then the result is false.
    EXPECT_FALSE(result);
}

TEST_F(NeuralNetworkShould, notBeExecutableIfInputMapIsMissing) {
    // Given a neural network missing its input tensor map.
    const NeuralNetwork<float> network = buildSampleNetwork(3, true, false, true);

    // When looking whether the network can be executed.
    const bool result = network.canExecute();

    // Then the result is false.
    EXPECT_FALSE(result);
}

TEST_F(NeuralNetworkShould, notBeExecutableIfOutputMapIsMissing) {
    // Given a neural network missing its output tensor map.
    const NeuralNetwork<float> network = buildSampleNetwork(3, true, true, false);

    // When looking whether the network can be executed.
    const bool result = network.canExecute();

    // Then the result is false.
    EXPECT_FALSE(result);
}

TEST_F(NeuralNetworkShould, beExecutableIfComplete) {
    // Given a complete neural network.
    const NeuralNetwork<float> network = buildSampleNetwork(3, true, true, true);

    // When looking whether the network can be executed.
    const bool result = network.canExecute();

    // Then the result is true.
    EXPECT_TRUE(result);
}

TEST_F(NeuralNetworkShould, executeComputationsOnOneIteration) {
    // Given a complete neural network, an input provider and an output receiver.
    const NeuralNetwork<float> network = buildSampleNetwork(3);

    // When executing the computation once.
    network.execute(*inputProvider, *outputReceiver);

    // Then the output receives results corresponding to the first inputs from the input provider.
    // The network should add 11, 10, 9, 8 and 7 to each float of a given input respectively.
    // The number of outputs received should correspond to the thickness of the neural network.
    const std::vector<std::tuple<float, float, float, float, float>>& results = outputReceiver->getResults();
    ASSERT_EQ(results.size(), 3);
    EXPECT_EQ(results[0], std::make_tuple(11.0f, 10.0f, 9.0f, 8.0f, 7.0f));
    EXPECT_EQ(results[1], std::make_tuple(12.0f, 11.0f, 10.0f, 9.0f, 8.0f));
    EXPECT_EQ(results[2], std::make_tuple(13.0f, 12.0f, 11.0f, 10.0f, 9.0f));
}

TEST_F(NeuralNetworkShould, executeAllAvailableComputations) {
    // Given a complete neural network, an input provider and an output receiver.
    const NeuralNetwork<float> network = buildSampleNetwork(3);

    // When executing the computation once.
    network.executeAll(*inputProvider, *outputReceiver);

    // Then the output receives results corresponding to all inputs from the input provider
    // until there is less than 3 inputs remaining (thickness).
    // The number of outputs received should correspond to the multiple of 3 (thickless) right
    // below the total amount of inputs in the input provider (11).
    // The network should add 11, 10, 9, 8 and 7 to each float of a given input respectively.
    const std::vector<std::tuple<float, float, float, float, float>>& results = outputReceiver->getResults();
    ASSERT_EQ(results.size(), 9);
    EXPECT_EQ(results[0], std::make_tuple(11.0f, 10.0f, 9.0f, 8.0f, 7.0f));
    EXPECT_EQ(results[1], std::make_tuple(12.0f, 11.0f, 10.0f, 9.0f, 8.0f));
    EXPECT_EQ(results[2], std::make_tuple(13.0f, 12.0f, 11.0f, 10.0f, 9.0f));
    EXPECT_EQ(results[3], std::make_tuple(14.0f, 13.0f, 12.0f, 11.0f, 10.0f));
    EXPECT_EQ(results[4], std::make_tuple(15.0f, 14.0f, 13.0f, 12.0f, 11.0f));
    EXPECT_EQ(results[5], std::make_tuple(16.0f, 15.0f, 14.0f, 13.0f, 12.0f));
    EXPECT_EQ(results[6], std::make_tuple(17.0f, 16.0f, 15.0f, 14.0f, 13.0f));
    EXPECT_EQ(results[7], std::make_tuple(18.0f, 17.0f, 16.0f, 15.0f, 14.0f));
    EXPECT_EQ(results[8], std::make_tuple(19.0f, 18.0f, 17.0f, 16.0f, 15.0f));
}
