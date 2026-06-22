#include <gtest/gtest.h>

#include <network/NeuralNetwork.h>
#include <network/NeuralNetworkPartHolder.h>
#include <network/NullParamTensorFiller.h>

#include <pluginloader/PluginLoader.h>

#include <plugins/nativemodules/AdditionModule.h>
#include <plugins/nativemodules/TensorMapKeyEnums.h>

#include <tensor/TensorIndex.h>
#include <tensor/TensorMap.h>

#include <filesystem>

class NeuralNetworkPartHolderShould : public ::testing::Test
{
protected:
    void SetUp() override
    {
        pluginLoader = new PluginLoader();
        const std::filesystem::path libPath = PluginLoader::getExecutableDir() / "nativemodules.dll";
        pluginLoader->loadPluginFromPath(libPath);
    }

    void TearDown() override
    {
        delete pluginLoader;
    }

protected:
    PluginLoader* pluginLoader;
};

TEST_F(NeuralNetworkPartHolderShould, addTensors) {
    // Given an empty neural network and a neural network part holder.
    NeuralNetwork<float> network(3);

    NeuralNetworkPartHolder<float>* partHolder = new NeuralNetworkPartHolder(network);

    // When adding two tensors though the network part holder.
    AbstractTensor<float>& firstTensor = partHolder->addTensor(TensorIndex<2>{4, 4});
    AbstractTensor<float>& secondTensor = partHolder->addTensor(TensorIndex<2>{5, 5});

    // Then the network should have the expected two tensors.
    const std::vector<AbstractTensor<float>*> expectedTensors = {&firstTensor, &secondTensor};
    EXPECT_EQ(network.getTensors(), expectedTensors);
    const std::vector<AbstractTensorMap<float>*> expectedTensorMaps = {};
    EXPECT_EQ(network.getTensorMaps(), expectedTensorMaps);
    const std::vector<Module<float>*> expectedModules = {};
    EXPECT_EQ(network.getModules(), expectedModules);

    // Free the neural network part holder.
    delete partHolder;
}

TEST_F(NeuralNetworkPartHolderShould, addTensorMaps) {
    // Given a neural network and a neural network part holder with two tensors.
    NeuralNetwork<float> network(3);

    NeuralNetworkPartHolder<float>* partHolder = new NeuralNetworkPartHolder(network);

    AbstractTensor<float>& firstTensor = partHolder->addTensor(TensorIndex<2>{4, 4});
    AbstractTensor<float>& secondTensor = partHolder->addTensor(TensorIndex<2>{5, 5});

    // When adding two tensor maps though the network part holder.
    AbstractTensorMap<float>& firstTensorMap = partHolder->addTensorMap(new TensorMap<float, TensorSingleton>{&firstTensor});
    AbstractTensorMap<float>& secondTensorMap = partHolder->addTensorMap(new TensorMap<float, TensorSingleton>{&secondTensor});

    // Then the network should have the expected two tensors and two tensor maps.
    const std::vector<AbstractTensor<float>*> expectedTensors = {&firstTensor, &secondTensor};
    EXPECT_EQ(network.getTensors(), expectedTensors);
    const std::vector<AbstractTensorMap<float>*> expectedTensorMaps = {&firstTensorMap, &secondTensorMap};
    EXPECT_EQ(network.getTensorMaps(), expectedTensorMaps);
    const std::vector<Module<float>*> expectedModules = {};
    EXPECT_EQ(network.getModules(), expectedModules);

    // Free the neural network part holder.
    delete partHolder;
}

TEST_F(NeuralNetworkPartHolderShould, addModules) {
    // Given a neural network and a neural network part holder with two tensors and two tensor maps.
    NeuralNetwork<float> network(3);

    NeuralNetworkPartHolder<float>* partHolder = new NeuralNetworkPartHolder(network);

    AbstractTensor<float>& firstTensor = partHolder->addTensor(TensorIndex<1>{5});
    AbstractTensor<float>& secondTensor = partHolder->addTensor(TensorIndex<1>{5});
    AbstractTensorMap<float>& firstTensorMap = partHolder->addTensorMap(new TensorMap<float, TensorSingleton>{&firstTensor});
    AbstractTensorMap<float>& secondTensorMap = partHolder->addTensorMap(new TensorMap<float, TensorSingleton>{&secondTensor});

    // When adding a module though the network part holder.
    const AbstractRtti<Module<float>>* additionModuleRtti = AdditionModule<float, 1>::getRtti();
    NullParamTensorFiller<float> paramTensorFiller;
    Module<float>& module = partHolder->addModule(*additionModuleRtti, Initializer<const RawTuple<const TensorIndex<1>&>&>(makeRawTuple(TensorIndex<1>{5})), firstTensorMap, secondTensorMap, paramTensorFiller);

    // Then the network should have two tensors, two tensor maps and a module.
    const std::vector<AbstractTensor<float>*> expectedTensors = {&firstTensor, &secondTensor};
    EXPECT_EQ(network.getTensors(), expectedTensors);
    const std::vector<AbstractTensorMap<float>*> expectedTensorMaps = {&firstTensorMap, &secondTensorMap};
    EXPECT_EQ(network.getTensorMaps(), expectedTensorMaps);
    const std::vector<Module<float>*> expectedModules = {&module};
    EXPECT_EQ(network.getModules(), expectedModules);

    // Free the neural network part holder.
    delete partHolder;
}

TEST_F(NeuralNetworkPartHolderShould, removeTheRelatedNetworkElements) {
    // Given a neural network and a neural network part holder with two tensors, two tensor maps and a module.
    // Only one module, one tensor map and one tensor are added though the network part holder.
    NeuralNetwork<float> network(3);

    NeuralNetworkPartHolder<float>* partHolder = new NeuralNetworkPartHolder(network);

    AbstractTensor<float>& firstTensor = network.addTensor(TensorIndex<1>{5});
    AbstractTensor<float>& secondTensor = partHolder->addTensor(TensorIndex<1>{5});
    AbstractTensorMap<float>& firstTensorMap = network.addTensorMap(new TensorMap<float, TensorSingleton>{&firstTensor});
    AbstractTensorMap<float>& secondTensorMap = partHolder->addTensorMap(new TensorMap<float, TensorSingleton>{&secondTensor});
    const AbstractRtti<Module<float>>* additionModuleRtti = AdditionModule<float, 1>::getRtti();
    NullParamTensorFiller<float> paramTensorFiller;
    Module<float>& module = partHolder->addModule(*additionModuleRtti, Initializer<const RawTuple<const TensorIndex<1>&>&>(makeRawTuple(TensorIndex<1>{5})), firstTensorMap, secondTensorMap, paramTensorFiller);

    // When deleting the network part holder.
    delete partHolder;

    // Then the network should be empty again.
    const std::vector<AbstractTensor<float>*> expectedTensors = {&firstTensor};
    EXPECT_EQ(network.getTensors(), expectedTensors);
    const std::vector<AbstractTensorMap<float>*> expectedTensorMaps = {&firstTensorMap};
    EXPECT_EQ(network.getTensorMaps(), expectedTensorMaps);
    const std::vector<Module<float>*> expectedModules = {};
    EXPECT_EQ(network.getModules(), expectedModules);
}
