#include <gtest/gtest.h>

#include <network/DuplicateTensorBuilder.h>
#include <network/NeuralNetwork.h>

#include <tensor/Tensor.h>

class DuplicateTensorBuilderShould : public ::testing::Test
{};

TEST_F(DuplicateTensorBuilderShould, duplicateATensor) {
    // Given a neural network with a tensor.
    NeuralNetwork<float> network(3);
    AbstractTensor<float>& originalTensor = network.addTensor(TensorIndex<2>{4, 4});

    // When using DuplicateTensorBuilder on this tensor.
    const std::map<std::string, void*> result = network.build(*DuplicateTensorBuilder<float>::getRtti(), Initializer<NeuralNetwork<float>&, const AbstractTensor<float>&>(network, originalTensor));

    // Then the created tensor should have the same size as the original
    AbstractTensor<float>& duplicatedTensor = *static_cast<AbstractTensor<float>*>(result.at(DuplicateTensorBuilder<float>::addedTensorKey));
    EXPECT_EQ(originalTensor.sizes(), duplicatedTensor.sizes());

    // and the content of the network should be the original tensor and its copy.
    std::vector<AbstractTensor<float>*> expectedTensors = {&originalTensor, &duplicatedTensor};
    EXPECT_EQ(network.getTensors(), expectedTensors);
    std::vector<AbstractTensorMap<float>*> expectedTensorMaps = {};
    EXPECT_EQ(network.getTensorMaps(), expectedTensorMaps);
    std::vector<Module<float>*> expectedModules = {};
    EXPECT_EQ(network.getModules(), expectedModules);
}

TEST_F(DuplicateTensorBuilderShould, removeTheDuplicatedTensor) {
    // Given a neural network with a tensor, which was duplicated.
    NeuralNetwork<float> network(3);
    AbstractTensor<float>& originalTensor = network.addTensor(TensorIndex<2>{4, 4});
    const AbstractNetworkBuilder<float>* builder = nullptr;
    network.build(*DuplicateTensorBuilder<float>::getRtti(), Initializer<NeuralNetwork<float>&, const AbstractTensor<float>&>(network, originalTensor), &builder);

    // When unbuilding the duplicated tensor.
    network.unbuild(*builder);

    // Then the content of the network should be the original tensor only.
    std::vector<AbstractTensor<float>*> expectedTensors = {&originalTensor};
    EXPECT_EQ(network.getTensors(), expectedTensors);
    std::vector<AbstractTensorMap<float>*> expectedTensorMaps = {};
    EXPECT_EQ(network.getTensorMaps(), expectedTensorMaps);
    std::vector<Module<float>*> expectedModules = {};
    EXPECT_EQ(network.getModules(), expectedModules);
}
