#include <gtest/gtest.h>

#include <network/AbstractNetworkBuilder.h>
#include <network/NeuralNetwork.h>
#include <network/ParamTensorFiller.h>
#include <network/InputProvider.h>
#include <network/NeuralNetworkPartHolder.h>
#include <network/NullParamTensorFiller.h>
#include <network/OutputReceiver.h>

#include <helpers/RawTuple.h>

#include <pluginloader/PluginLoader.h>

#include <plugins/natives/modules/additionmodule/AdditionModule.h>
#include <plugins/natives/tensormaps/TensorMapKeyEnums.h>

#include <rtti/RttiHolder.h>
#include <rtti/RttiHolder.impl.h>

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

template <typename ValueType>
class AddTensorBuilder : public AbstractNetworkBuilder<ValueType>
{
public:
    AddTensorBuilder(NeuralNetwork<ValueType>* network, const AbstractTensorIndex* size)
        : m_networkPartHolder(*network)
        , m_size(size)
    {}

    ~AddTensorBuilder()
    {
        delete m_size;
    }

    std::map<std::string, void*> build() override
    {
        std::map<std::string, void*> result;
        result["added_tensor"] = &m_networkPartHolder.addTensor(*m_size);
        return result;
    }

private:
    NeuralNetworkPartHolder<ValueType> m_networkPartHolder;
    const AbstractTensorIndex* m_size;

    DECLARE_RTTI(AbstractNetworkBuilder<ValueType>)
};

IMPLEMENT_RTTI(AddTensorBuilder, AbstractNetworkBuilder<ValueType>, PACK(typename), PACK(ValueType))

template <typename ValueType>
class AddSingleTensorMapBuilder : public AbstractNetworkBuilder<ValueType>
{
public:
    AddSingleTensorMapBuilder(NeuralNetwork<ValueType>* network, AbstractTensor<ValueType>* tensor)
        : m_networkPartHolder(*network)
        , m_tensor(*tensor)
    {}

    ~AddSingleTensorMapBuilder()
    {}

    std::map<std::string, void*> build() override
    {
        AbstractTensorMap<ValueType>& tensorMap = m_networkPartHolder.addTensorMap(new TensorMap<ValueType, TensorSingleton>{&m_tensor});

        std::map<std::string, void*> result;
        result["added_tensor_map"] = &tensorMap;
        return result;
    }

private:
    NeuralNetworkPartHolder<ValueType> m_networkPartHolder;
    AbstractTensor<ValueType>& m_tensor;

    DECLARE_RTTI(AbstractNetworkBuilder<ValueType>)
};

IMPLEMENT_RTTI(AddSingleTensorMapBuilder, AbstractNetworkBuilder<ValueType>, PACK(typename), PACK(ValueType))

template <typename ValueType>
class AddAdditionModuleBuilder : public AbstractNetworkBuilder<ValueType>
{
public:
    AddAdditionModuleBuilder(NeuralNetwork<ValueType>* network, AbstractTensorMap<ValueType>* inputTensorMap, AbstractTensorMap<ValueType>* outputTensorMap)
        : m_networkPartHolder(*network)
        , m_inputTensorMap(*inputTensorMap)
        , m_outputTensorMap(*outputTensorMap)
    {}

    ~AddAdditionModuleBuilder()
    {}

    std::map<std::string, void*> build() override
    {
        const AbstractRtti<Module<ValueType>>* moduleRtti = AdditionModule<ValueType, 1>::getRtti();
        NullParamTensorFiller<ValueType> paramTensorFiller;
        Module<float>& module = m_networkPartHolder.addModule(*moduleRtti, Initializer<const RawTuple<const TensorIndex<1>&>&>(makeRawTuple(TensorIndex<1>{5})), m_inputTensorMap, m_outputTensorMap, paramTensorFiller);

        std::map<std::string, void*> result;
        result["added_module"] = &module;
        return result;
    }

private:
    NeuralNetworkPartHolder<ValueType> m_networkPartHolder;
    AbstractTensorMap<ValueType>& m_inputTensorMap;
    AbstractTensorMap<ValueType>& m_outputTensorMap;

    DECLARE_RTTI(AbstractNetworkBuilder<ValueType>)
};

IMPLEMENT_RTTI(AddAdditionModuleBuilder, AbstractNetworkBuilder<ValueType>, PACK(typename), PACK(ValueType))

class NeuralNetworkShould : public ::testing::Test
{
protected:
    void SetUp() override
    {
        pluginLoader = new PluginLoader();
        const std::filesystem::path libPath = PluginLoader::getExecutableDir() / "natives.dll";
        pluginLoader->loadPluginFromPath(libPath);

        buildersRttiHolder = new RttiHolder<AbstractNetworkBuilder<float>>;

        buildersRttiToken = new RttiHolderToken(std::move(buildersRttiHolder->edit()));
        buildersRttiToken->subscribe(new Rtti<AbstractNetworkBuilder<float>, AddTensorBuilder<float>, BuildTimeList::TypeList<NeuralNetwork<float>*, const AbstractTensorIndex*>>("AddTensorBuilder<float>"));
        buildersRttiToken->subscribe(new Rtti<AbstractNetworkBuilder<float>, AddSingleTensorMapBuilder<float>, BuildTimeList::TypeList<NeuralNetwork<float>*, AbstractTensor<float>*>>("AddSingleTensorMapBuilder<float>"));
        buildersRttiToken->subscribe(new Rtti<AbstractNetworkBuilder<float>, AddAdditionModuleBuilder<float>, BuildTimeList::TypeList<NeuralNetwork<float>*, AbstractTensorMap<float>*, AbstractTensorMap<float>*>>("AddAdditionModuleBuilder<float>"));

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
        delete buildersRttiToken;
        delete buildersRttiHolder;
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
            network.setInput(&inputTensorMap);
        }

        if (setOutputMap)
        {
            network.setOutput(&outputTensorMap);
        }

        return network;
    }

protected:
    PluginLoader* pluginLoader;
    RttiHolder<AbstractNetworkBuilder<float>>* buildersRttiHolder;
    RttiHolderToken<AbstractNetworkBuilder<float>>* buildersRttiToken;
    SampleInputProvider* inputProvider;
    SampleOutputReceiver* outputReceiver;
};

TEST_F(NeuralNetworkShould, beEmptyByDefault) {
    // Given an empty neural network.
    NeuralNetwork<float> network(3);

    // When doing nothing.

    // Then the content of the network should be empty.
    std::vector<AbstractTensor<float>*> expectedTensors = {};
    EXPECT_EQ(network.getTensors(), expectedTensors);
    std::vector<AbstractTensorMap<float>*> expectedTensorMaps = {};
    EXPECT_EQ(network.getTensorMaps(), expectedTensorMaps);
    std::vector<Module<float>*> expectedModules = {};
    EXPECT_EQ(network.getModules(), expectedModules);
}

TEST_F(NeuralNetworkShould, addTensors) {
    // Given an empty neural network.
    NeuralNetwork<float> network(3);

    // When adding two tensors.
    AbstractTensor<float>& firstTensor = network.addTensor(TensorIndex<2>{4, 4});
    AbstractTensor<float>& secondTensor = network.addTensor(TensorIndex<2>{5, 5});

    // Then the created tensors shuld have the asked size,
    // with one extra dimension equal to the thickness of the network
    // in the first dimension.
    const TensorIndex<3> expectedFirstTensorSize = {3, 4, 4};
    EXPECT_EQ(firstTensor.sizes(), expectedFirstTensorSize);
    const TensorIndex<3> expectedSecondTensorSize = {3, 5, 5};
    EXPECT_EQ(secondTensor.sizes(), expectedSecondTensorSize);
    // And the network should have two tensors.
    const std::vector<AbstractTensor<float>*> expectedTensors = {&firstTensor, &secondTensor};
    EXPECT_EQ(network.getTensors(), expectedTensors);
    const std::vector<AbstractTensorMap<float>*> expectedTensorMaps = {};
    EXPECT_EQ(network.getTensorMaps(), expectedTensorMaps);
    const std::vector<Module<float>*> expectedModules = {};
    EXPECT_EQ(network.getModules(), expectedModules);
}

TEST_F(NeuralNetworkShould, addTensorMaps) {
    // Given a neural network with two tensors.
    NeuralNetwork<float> network(3);
    AbstractTensor<float>& firstTensor = network.addTensor(TensorIndex<2>{4, 4});
    AbstractTensor<float>& secondTensor = network.addTensor(TensorIndex<2>{5, 5});

    // When adding two tensor maps.
    AbstractTensorMap<float>& firstTensorMap = network.addTensorMap(new TensorMap<float, TensorSingleton>{&firstTensor});
    AbstractTensorMap<float>& secondTensorMap = network.addTensorMap(new TensorMap<float, TensorSingleton>{&secondTensor});

    // Then the created tensor maps should contain exactly their respective tensors
    ASSERT_EQ(firstTensorMap.nbTensors(), 1);
    EXPECT_EQ(&firstTensorMap.get(0), &firstTensor);
    ASSERT_EQ(secondTensorMap.nbTensors(), 1);
    EXPECT_EQ(&secondTensorMap.get(0), &secondTensor);
    // And the network should have two tensors and two tensor maps.
    const std::vector<AbstractTensor<float>*> expectedTensors = {&firstTensor, &secondTensor};
    EXPECT_EQ(network.getTensors(), expectedTensors);
    const std::vector<AbstractTensorMap<float>*> expectedTensorMaps = {&firstTensorMap, &secondTensorMap};
    EXPECT_EQ(network.getTensorMaps(), expectedTensorMaps);
    const std::vector<Module<float>*> expectedModules = {};
    EXPECT_EQ(network.getModules(), expectedModules);
}

TEST_F(NeuralNetworkShould, addModules) {
    // Given a neural network with two tensors and two tensor maps.
    NeuralNetwork<float> network(3);
    AbstractTensor<float>& firstTensor = network.addTensor(TensorIndex<1>{5});
    AbstractTensor<float>& secondTensor = network.addTensor(TensorIndex<1>{5});
    AbstractTensorMap<float>& firstTensorMap = network.addTensorMap(new TensorMap<float, TensorSingleton>{&firstTensor});
    AbstractTensorMap<float>& secondTensorMap = network.addTensorMap(new TensorMap<float, TensorSingleton>{&secondTensor});

    // When adding a module.
    const AbstractRtti<Module<float>>* additionModuleRtti = AdditionModule<float, 1>::getRtti();
    const SampleParamTensorFiller firstParamTensorFiller(1.0f, 2.0f, 3.0f, 4.0f, 5.0f);
    Module<float>& module = network.addModule(*additionModuleRtti, Initializer<const RawTuple<const TensorIndex<1>&>&>(makeRawTuple(TensorIndex<1>{5})), firstTensorMap, secondTensorMap, firstParamTensorFiller);

    // Then the created module should be connexted to the right tensor maps
    // And its parameter tensor map is well initialized
    EXPECT_EQ(module.getInputAbstractTensorMap(), &firstTensorMap);

    const AbstractTensorMap<float>& parameterTensorMap = module.getParameterAbstractTensorMap();
    ASSERT_EQ(parameterTensorMap.nbTensors(), 1);
    const AbstractTensor<float>& parameterTensor = parameterTensorMap.get(0);
    ASSERT_EQ(parameterTensor.dim(), 1);
    ASSERT_EQ(parameterTensor.sizes(), TensorIndex<1>{5});
    EXPECT_EQ(parameterTensor[TensorIndex<1>{0}], 1.0f);
    EXPECT_EQ(parameterTensor[TensorIndex<1>{1}], 2.0f);
    EXPECT_EQ(parameterTensor[TensorIndex<1>{2}], 3.0f);
    EXPECT_EQ(parameterTensor[TensorIndex<1>{3}], 4.0f);
    EXPECT_EQ(parameterTensor[TensorIndex<1>{4}], 5.0f);

    EXPECT_EQ(module.getOutputAbstractTensorMap(), &secondTensorMap);

    // And the network should have two tensors, two tensor maps and a module.
    const std::vector<AbstractTensor<float>*> expectedTensors = {&firstTensor, &secondTensor};
    EXPECT_EQ(network.getTensors(), expectedTensors);
    const std::vector<AbstractTensorMap<float>*> expectedTensorMaps = {&firstTensorMap, &secondTensorMap};
    EXPECT_EQ(network.getTensorMaps(), expectedTensorMaps);
    const std::vector<Module<float>*> expectedModules = {&module};
    EXPECT_EQ(network.getModules(), expectedModules);
}

TEST_F(NeuralNetworkShould, forbidRemovingUsedTensors) {
    // Given a neural network with a tensor and a tensor map.
    NeuralNetwork<float> network(3);
    AbstractTensor<float>& tensor = network.addTensor(TensorIndex<1>{5});
    AbstractTensorMap<float>& tensorMap = network.addTensorMap(new TensorMap<float, TensorSingleton>{&tensor});

    // When removing a used tensor.
    // Then an exception is thrown.
    EXPECT_THROW({ network.removeTensor(tensor); }, std::runtime_error);
}

TEST_F(NeuralNetworkShould, removingUnusedTensors) {
    // Given a neural network with an unused tensor.
    NeuralNetwork<float> network(3);
    AbstractTensor<float>& tensor = network.addTensor(TensorIndex<1>{5});

    // When removing the unused tensor.
    network.removeTensor(tensor);

    // Then the neural network is empty.
    const std::vector<AbstractTensor<float>*> expectedTensors = {};
    EXPECT_EQ(network.getTensors(), expectedTensors);
    const std::vector<AbstractTensorMap<float>*> expectedTensorMaps = {};
    EXPECT_EQ(network.getTensorMaps(), expectedTensorMaps);
    const std::vector<Module<float>*> expectedModules = {};
    EXPECT_EQ(network.getModules(), expectedModules);
}

TEST_F(NeuralNetworkShould, forbidRemovingUsedTensorMaps) {
    // Given a neural network with two tensors, two tensor maps and a module.
    NeuralNetwork<float> network(3);
    AbstractTensor<float>& firstTensor = network.addTensor(TensorIndex<1>{5});
    AbstractTensor<float>& secondTensor = network.addTensor(TensorIndex<1>{5});
    AbstractTensorMap<float>& firstTensorMap = network.addTensorMap(new TensorMap<float, TensorSingleton>{&firstTensor});
    AbstractTensorMap<float>& secondTensorMap = network.addTensorMap(new TensorMap<float, TensorSingleton>{&secondTensor});
    const AbstractRtti<Module<float>>* additionModuleRtti = AdditionModule<float, 1>::getRtti();
    const SampleParamTensorFiller firstParamTensorFiller(1.0f, 2.0f, 3.0f, 4.0f, 5.0f);
    Module<float>& module = network.addModule(*additionModuleRtti, Initializer<const RawTuple<const TensorIndex<1>&>&>(makeRawTuple(TensorIndex<1>{5})), firstTensorMap, secondTensorMap, firstParamTensorFiller);

    // When removing a used tensor map.
    // Then an exception is thrown.
    EXPECT_THROW({ network.removeTensorMap(firstTensorMap); }, std::runtime_error);
}

TEST_F(NeuralNetworkShould, removingUnusedTensorMaps) {
    // Given a neural network with a tensor and a tensor map.
    NeuralNetwork<float> network(3);
    AbstractTensor<float>& tensor = network.addTensor(TensorIndex<1>{5});
    AbstractTensorMap<float>& tensorMap = network.addTensorMap(new TensorMap<float, TensorSingleton>{&tensor});

    // When removing the unused tensor map.
    network.removeTensorMap(tensorMap);

    // Then the neural network only has one tensor.
    const std::vector<AbstractTensor<float>*> expectedTensors = {&tensor};
    EXPECT_EQ(network.getTensors(), expectedTensors);
    const std::vector<AbstractTensorMap<float>*> expectedTensorMaps = {};
    EXPECT_EQ(network.getTensorMaps(), expectedTensorMaps);
    const std::vector<Module<float>*> expectedModules = {};
    EXPECT_EQ(network.getModules(), expectedModules);
}

TEST_F(NeuralNetworkShould, removeModules) {
    // Given a neural network with two tensors, two tensor maps and a module.
    NeuralNetwork<float> network(3);
    AbstractTensor<float>& firstTensor = network.addTensor(TensorIndex<1>{5});
    AbstractTensor<float>& secondTensor = network.addTensor(TensorIndex<1>{5});
    AbstractTensorMap<float>& firstTensorMap = network.addTensorMap(new TensorMap<float, TensorSingleton>{&firstTensor});
    AbstractTensorMap<float>& secondTensorMap = network.addTensorMap(new TensorMap<float, TensorSingleton>{&secondTensor});
    const AbstractRtti<Module<float>>* additionModuleRtti = AdditionModule<float, 1>::getRtti();
    const SampleParamTensorFiller firstParamTensorFiller(1.0f, 2.0f, 3.0f, 4.0f, 5.0f);
    Module<float>& module = network.addModule(*additionModuleRtti, Initializer<const RawTuple<const TensorIndex<1>&>&>(makeRawTuple(TensorIndex<1>{5})), firstTensorMap, secondTensorMap, firstParamTensorFiller);

    // When removing the module.
    network.removeModule(module);

    // Then the network should have two tensors, two tensor maps only.
    const std::vector<AbstractTensor<float>*> expectedTensors = {&firstTensor, &secondTensor};
    EXPECT_EQ(network.getTensors(), expectedTensors);
    const std::vector<AbstractTensorMap<float>*> expectedTensorMaps = {&firstTensorMap, &secondTensorMap};
    EXPECT_EQ(network.getTensorMaps(), expectedTensorMaps);
    const std::vector<Module<float>*> expectedModules = {};
    EXPECT_EQ(network.getModules(), expectedModules);
}

TEST_F(NeuralNetworkShould, useNeuralNetworkBuilders) {
    // Given a neural network with one tensor and one tensor map by default
    NeuralNetwork<float> network(3);
    AbstractTensor<float>& firstTensor = network.addTensor(TensorIndex<1>{5});
    AbstractTensorMap<float>& firstTensorMap = network.addTensorMap(new TensorMap<float, TensorSingleton>{&firstTensor});

    // When building the network
    const AbstractNetworkBuilder<float>* addTensorResultBuilder = nullptr;
    std::map<std::string, void*> addTensorBuildingResult = network.build(
        *AddTensorBuilder<float>::getRtti(),
        Initializer<NeuralNetwork<float>*, const AbstractTensorIndex*>(
            &network,
            new TensorIndex<1>{5}
        ),
        &addTensorResultBuilder
    );

    const AbstractNetworkBuilder<float>* addTensorMapResultBuilder = nullptr;
    std::map<std::string, void*> addTensorMapBuildingResult = network.build(
        *AddSingleTensorMapBuilder<float>::getRtti(),
        Initializer<NeuralNetwork<float>*, AbstractTensor<float>*>(
            &network,
            std::move(static_cast<AbstractTensor<float>*>(addTensorBuildingResult["added_tensor"]))
        ),
        &addTensorMapResultBuilder
    );

    ASSERT_EQ(network.getTensorMaps().size(), 2);
    const AbstractNetworkBuilder<float>* addAdditionModuleResultBuilder = nullptr;
    std::map<std::string, void*> addModuleBuildingResult = network.build(
        *AddAdditionModuleBuilder<float>::getRtti(),
        Initializer<NeuralNetwork<float>*, AbstractTensorMap<float>*, AbstractTensorMap<float>*>(
            &network,
            &firstTensorMap,
            std::move(static_cast<AbstractTensorMap<float>*>(addTensorMapBuildingResult["added_tensor_map"]))
        ),
        &addAdditionModuleResultBuilder
    );

    // Then the builders should be written in the result pointers
    // and the newly created elements should appear in the network
    EXPECT_NE(addTensorResultBuilder, nullptr);
    EXPECT_NE(addTensorMapResultBuilder, nullptr);
    EXPECT_NE(addAdditionModuleResultBuilder, nullptr);

    const std::vector<AbstractTensor<float>*> expectedTensors = {&firstTensor, static_cast<AbstractTensor<float>*>(addTensorBuildingResult["added_tensor"])};
    EXPECT_EQ(network.getTensors(), expectedTensors);

    const std::vector<AbstractTensorMap<float>*> expectedTensorMaps = {&firstTensorMap, static_cast<AbstractTensorMap<float>*>(addTensorMapBuildingResult["added_tensor_map"])};
    EXPECT_EQ(network.getTensorMaps(), expectedTensorMaps);

    const std::vector<Module<float>*> expectedModules = {static_cast<Module<float>*>(addModuleBuildingResult["added_module"])};
    EXPECT_EQ(network.getModules(), expectedModules);
}

TEST_F(NeuralNetworkShould, unbuildNeuralNetworkBuilders) {
    // Given a neural network on which neural network builder were used
    NeuralNetwork<float> network(3);
    AbstractTensor<float>& firstTensor = network.addTensor(TensorIndex<1>{5});
    AbstractTensorMap<float>& firstTensorMap = network.addTensorMap(new TensorMap<float, TensorSingleton>{&firstTensor});

    const AbstractNetworkBuilder<float>* addTensorResultBuilder = nullptr;
    std::map<std::string, void*> addTensorBuildingResult = network.build(
        *AddTensorBuilder<float>::getRtti(),
        Initializer<NeuralNetwork<float>*, const AbstractTensorIndex*>(
            &network,
            new TensorIndex<1>{5}
        ),
        &addTensorResultBuilder
    );

    const AbstractNetworkBuilder<float>* addTensorMapResultBuilder = nullptr;
    std::map<std::string, void*> addTensorMapBuildingResult = network.build(
        *AddSingleTensorMapBuilder<float>::getRtti(),
        Initializer<NeuralNetwork<float>*, AbstractTensor<float>*>(
            &network,
            std::move(static_cast<AbstractTensor<float>*>(addTensorBuildingResult["added_tensor"]))
        ),
        &addTensorMapResultBuilder
    );

    ASSERT_EQ(network.getTensorMaps().size(), 2);
    const AbstractNetworkBuilder<float>* addAdditionModuleResultBuilder = nullptr;
    network.build(
        *AddAdditionModuleBuilder<float>::getRtti(),
        Initializer<NeuralNetwork<float>*, AbstractTensorMap<float>*, AbstractTensorMap<float>*>(
            &network,
            &firstTensorMap,
            std::move(static_cast<AbstractTensorMap<float>*>(addTensorMapBuildingResult["added_tensor_map"]))
        ),
        &addAdditionModuleResultBuilder
    );

    // When unbuilding the previously used builders
    ASSERT_NE(addTensorResultBuilder, nullptr);
    ASSERT_NE(addTensorMapResultBuilder, nullptr);
    ASSERT_NE(addAdditionModuleResultBuilder, nullptr);
    network.unbuild(*addAdditionModuleResultBuilder);
    network.unbuild(*addTensorMapResultBuilder);
    network.unbuild(*addTensorResultBuilder);

    // Then the elements created by the previously used builders
    // should be removed from the neural network
    const std::vector<AbstractTensor<float>*> expectedTensors = {&firstTensor};
    EXPECT_EQ(network.getTensors(), expectedTensors);
    const std::vector<AbstractTensorMap<float>*> expectedTensorMaps = {&firstTensorMap};
    EXPECT_EQ(network.getTensorMaps(), expectedTensorMaps);
    const std::vector<Module<float>*> expectedModules = {};
    EXPECT_EQ(network.getModules(), expectedModules);
}

TEST_F(NeuralNetworkShould, notCrashOnDeletion) {
    // Given a neural network on which neural network builder were used
    NeuralNetwork<float>* network = new NeuralNetwork<float>(3);
    AbstractTensor<float>& firstTensor = network->addTensor(TensorIndex<1>{5});
    AbstractTensorMap<float>& firstTensorMap = network->addTensorMap(new TensorMap<float, TensorSingleton>{&firstTensor});

    std::map<std::string, void*> addTensorBuildingResult = network->build(
        *AddTensorBuilder<float>::getRtti(),
        Initializer<NeuralNetwork<float>*, const AbstractTensorIndex*>(
            std::move(static_cast<NeuralNetwork<float>*>(network)),
            new TensorIndex<1>{5}
        ),
        nullptr
    );

    std::map<std::string, void*> addTensorMapBuildingResult = network->build(
        *AddSingleTensorMapBuilder<float>::getRtti(),
        Initializer<NeuralNetwork<float>*, AbstractTensor<float>*>(
            std::move(static_cast<NeuralNetwork<float>*>(network)),
            std::move(static_cast<AbstractTensor<float>*>(addTensorBuildingResult["added_tensor"]))
        ),
        nullptr
    );

    ASSERT_EQ(network->getTensorMaps().size(), 2);
    network->build(
        *AddAdditionModuleBuilder<float>::getRtti(),
        Initializer<NeuralNetwork<float>*, AbstractTensorMap<float>*, AbstractTensorMap<float>*>(
            std::move(static_cast<NeuralNetwork<float>*>(network)),
            &firstTensorMap,
            std::move(static_cast<AbstractTensorMap<float>*>(addTensorMapBuildingResult["added_tensor_map"]))
        ),
        nullptr
    );

    // When deleting the network
    delete network;

    // Then no crash happens
}

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
