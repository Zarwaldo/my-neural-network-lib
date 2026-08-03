#include <gtest/gtest.h>

#include <network/DuplicateTensorMapBuilder.h>
#include <network/NeuralNetwork.h>

#include <rtti/RttiHolder.h>
#include <rtti/RttiHolder.impl.h>

#include <tensor/Tensor.h>
#include <tensor/TensorMap.impl.h>

// Define a sample KeyEnum
#define TENSOR_MAP_KEY_ENUM__VALUES_OF_KeyEnum FIRST_TENSOR, SECOND_TENSOR
DECLARE_TENSOR_MAP_KEY_ENUM(KeyEnum, )
IMPLEMENT_TENSOR_MAP_KEY_ENUM(KeyEnum)

class DuplicateTensorMapBuilderShould : public ::testing::Test
{
public:
    void SetUp() override
    {
        keyEnumsRttiHolder = new RttiHolder<AbstractTensorMapKeyEnum>;

        keyEnumsRttiToken = new RttiHolderToken(std::move(keyEnumsRttiHolder->edit()));
        keyEnumsRttiToken->subscribe(new Rtti<AbstractTensorMapKeyEnum, KeyEnum, BuildTimeList::TypeList<size_t>>("KeyEnum"));

        tensorMapRttiHolder = new RttiHolder<AbstractTensorMap<float>>;

        tensorMapRttiToken = new RttiHolderToken(std::move(tensorMapRttiHolder->edit()));
        tensorMapRttiToken->subscribe(
            new Rtti<
                AbstractTensorMap<float>,
                TensorMap<float, KeyEnum>,
                BuildTimeList::TypeList<AbstractTensor<float>* const *>,
                BuildTimeList::TypeList<const std::initializer_list<AbstractTensor<float>*>&>
            >("TensorMap<float, KeyEnum>")
        );
    }

    void TearDown() override
    {
        delete tensorMapRttiToken;
        delete tensorMapRttiHolder;
        delete keyEnumsRttiToken;
        delete keyEnumsRttiHolder;
    }

protected:
    RttiHolder<AbstractTensorMapKeyEnum>* keyEnumsRttiHolder;
    RttiHolderToken<AbstractTensorMapKeyEnum>* keyEnumsRttiToken;
    RttiHolder<AbstractTensorMap<float>>* tensorMapRttiHolder;
    RttiHolderToken<AbstractTensorMap<float>>* tensorMapRttiToken;
};

TEST_F(DuplicateTensorMapBuilderShould, duplicateATensorMap) {
    // Given a neural network with a tensor map and a correspondance map of tensors.
    NeuralNetwork<float> network(3);

    AbstractTensor<float>& firstTensor = network.addTensor(TensorIndex<2>{4, 4});
    AbstractTensor<float>& secondTensor = network.addTensor(TensorIndex<2>{5, 5});
    AbstractTensorMap<float>& originalTensorMap = network.addTensorMap(new TensorMap<float, KeyEnum>{&firstTensor, &secondTensor});

    AbstractTensor<float>& tensorCorrespondingToTheSecond = network.addTensor(TensorIndex<2>{5, 5});
    std::map<const AbstractTensor<float>*, AbstractTensor<float>*> correspondingTensors;
    correspondingTensors[&secondTensor] = &tensorCorrespondingToTheSecond;

    // When using DuplicateTensorMapBuilder on this tensor.
    const std::map<std::string, void*> result = network.build(
        *DuplicateTensorMapBuilder<float>::getRtti(),
        Initializer<
            NeuralNetwork<float>&,
            const AbstractTensorMap<float>&,
            const std::map<const AbstractTensor<float>*, AbstractTensor<float>*>&
        >(
            network,
            originalTensorMap,
            correspondingTensors
        )
    );

    // Then the created tensor map should have tensors of the same size as the originals,
    // and should have the corresponding tensors if specified.
    AbstractTensorMap<float>& duplicatedTensorMap = *static_cast<AbstractTensorMap<float>*>(result.at(DuplicateTensorMapBuilder<float>::addedTensorMapKey));
    EXPECT_EQ(duplicatedTensorMap.get(0).sizes(), firstTensor.sizes());
    EXPECT_EQ(duplicatedTensorMap.get(1).sizes(), secondTensor.sizes());
    EXPECT_EQ(&duplicatedTensorMap.get(1), &tensorCorrespondingToTheSecond);

    // and the content of the network should be the original tensor map and its copy.
    std::vector<AbstractTensor<float>*> expectedTensors = {&firstTensor, &secondTensor, &tensorCorrespondingToTheSecond, &duplicatedTensorMap.get(0)};
    EXPECT_EQ(network.getTensors(), expectedTensors);
    std::vector<AbstractTensorMap<float>*> expectedTensorMaps = {&originalTensorMap, &duplicatedTensorMap};
    EXPECT_EQ(network.getTensorMaps(), expectedTensorMaps);
    std::vector<Module<float>*> expectedModules = {};
    EXPECT_EQ(network.getModules(), expectedModules);
}

TEST_F(DuplicateTensorMapBuilderShould, removeTheDuplicatedTensorMap) {
    // Given a neural network with a tensor map, which was duplicated.
    NeuralNetwork<float> network(3);

    AbstractTensor<float>& firstTensor = network.addTensor(TensorIndex<2>{4, 4});
    AbstractTensor<float>& secondTensor = network.addTensor(TensorIndex<2>{5, 5});
    AbstractTensorMap<float>& originalTensorMap = network.addTensorMap(new TensorMap<float, KeyEnum>{&firstTensor, &secondTensor});

    AbstractTensor<float>& tensorCorrespondingToTheSecond = network.addTensor(TensorIndex<2>{5, 5});
    std::map<const AbstractTensor<float>*, AbstractTensor<float>*> correspondingTensors;
    correspondingTensors[&secondTensor] = &tensorCorrespondingToTheSecond;

    const AbstractNetworkBuilder<float>* builder = nullptr;
    network.build(
        *DuplicateTensorMapBuilder<float>::getRtti(),
        Initializer<
            NeuralNetwork<float>&,
            const AbstractTensorMap<float>&,
            const std::map<const AbstractTensor<float>*, AbstractTensor<float>*>&
        >(
            network,
            originalTensorMap,
            correspondingTensors
        ),
        &builder
    );

    // When unbuilding the duplicated tensor map.
    network.unbuild(*builder);

    // Then the content of the network should be the original tensor map
    // and the few tensors the duplication did not create.
    std::vector<AbstractTensor<float>*> expectedTensors = {&firstTensor, &secondTensor, &tensorCorrespondingToTheSecond};
    EXPECT_EQ(network.getTensors(), expectedTensors);
    std::vector<AbstractTensorMap<float>*> expectedTensorMaps = {&originalTensorMap};
    EXPECT_EQ(network.getTensorMaps(), expectedTensorMaps);
    std::vector<Module<float>*> expectedModules = {};
    EXPECT_EQ(network.getModules(), expectedModules);
}
