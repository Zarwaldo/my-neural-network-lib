#include <gtest/gtest.h>

#include <plugins/nativemodules/AdditionModule.h>

#include <module/MainTensorMapKeyEnums.h>

#include <tensor/TensorMap.h>

class AdditionModuleShould : public ::testing::Test
{
protected:
    const TensorIndex<5> commonSize = {16, 16, 16, 2, 2};
    const TensorIndex<4> paramSize = commonSize.template range<1, -1>();
    Tensor<float, 5>* input;
    TensorMap<float, TensorSingleton>* inputMap;
    Tensor<float, 5>* output;
    TensorMap<float, TensorSingleton>* outputMap;

    void SetUp() override
    {
        input = Tensor<float, 5>::create(commonSize);
        inputMap = new TensorMap<float, TensorSingleton>({input});

        output = Tensor<float, 5>::create(commonSize);
        outputMap = new TensorMap<float, TensorSingleton>({output});

        initializeInputTensor();
    }

    void TearDown() override
    {
        delete inputMap;
        delete input;
        delete outputMap;
        delete output;
    }

    void initializeInputTensor()
    {
        for (const TensorIndex<5>& index : *input)
        {
            (*input)[index] = static_cast<float>(index[0] + index[1] + index[2] + index[3] + index[4]);
        }
    }

    void initializeParameterTensor(AbstractTensor<float>& parameterTensor)
    {
        for (const TensorIndex<4>& index : static_cast<Tensor<float, 4>&>(parameterTensor))
        {
            parameterTensor[index] = static_cast<float>(
                (commonSize[0] + commonSize[1] + commonSize[2] + commonSize[3] + commonSize[4])
                - (index[0] + index[1] + index[2] + index[3])
            );
        }
    }
};

TEST_F(AdditionModuleShould, addAnInputTensorAndItsParameterTensor) {
    // Given an addition module plugged to an input tensor and an output tensor
    // with its parameter tensor initialized.
    const RawTuple<const TensorIndex<4>&>& parameterTensorsSizes = makeRawTuple<const TensorIndex<4>&>(paramSize);
    AdditionModule<float, 4> module(parameterTensorsSizes);
    module.setInputTensorMap(inputMap);
    module.setOutputTensorMap(outputMap);
    TensorMap<float, TensorSingleton>& parameterMap = dynamic_cast<TensorMap<float, TensorSingleton>&>(module.getParameterAbstractTensorMap());
    AbstractTensor<float>& parameterTensor = parameterMap.get(SINGLE_TENSOR);
    initializeParameterTensor(parameterTensor);

    // When the module is computed
    module.compute();

    // Then the output tensor equals the sum of the input tensor and the parameter tensor.
    for (const TensorIndex<5>& index : *output)
    {
        const float result = (*output)[index];
        const float expectedSum = (*input)[index] + parameterTensor[index.template range<1, -1>()];
        EXPECT_EQ(result, expectedSum);
    }
}

TEST_F(AdditionModuleShould, computeBackpropagation) {
    // Given an addition module that has computed a sum, and partial derivatives tensors
    const RawTuple<const TensorIndex<4>&>& parameterTensorsSizes = makeRawTuple<const TensorIndex<4>&>(paramSize);
    AdditionModule<float, 4> module(parameterTensorsSizes);
    module.setInputTensorMap(inputMap);
    module.setOutputTensorMap(outputMap);
    TensorMap<float, TensorSingleton>& parameterMap = dynamic_cast<TensorMap<float, TensorSingleton>&>(module.getParameterAbstractTensorMap());
    initializeParameterTensor(parameterMap.get(SINGLE_TENSOR));
    module.compute();

    Tensor<float, 5>* costPartDerivWRTInput = Tensor<float, 5>::create(commonSize);
    TensorMap<float, TensorSingleton>* costPartDerivWRTInputMap = new TensorMap<float, TensorSingleton>({costPartDerivWRTInput});
    Tensor<float, 4>* costPartDerivWRTParameter = Tensor<float, 4>::create(paramSize);
    TensorMap<float, TensorSingleton>* costPartDerivWRTParameterMap = new TensorMap<float, TensorSingleton>({costPartDerivWRTParameter});
    Tensor<float, 5>* costPartDerivWRTOutput = Tensor<float, 5>::create(commonSize);
    TensorMap<float, TensorSingleton>* costPartDerivWRTOutputMap = new TensorMap<float, TensorSingleton>({costPartDerivWRTOutput});
    // Fill the partial derivatives of the cost with respect to the output tensor with arbitrary values
    for (const TensorIndex<5>& index : *costPartDerivWRTOutput)
    {
        (*costPartDerivWRTOutput)[index] = 2.0f * (*output)[index];
    }

    // When we compute the backpropagation
    module.backpropagate(*costPartDerivWRTInputMap, *costPartDerivWRTParameterMap, *costPartDerivWRTOutputMap);

    // Then the backpropagation is well computed
    for (const TensorIndex<5>& index : *costPartDerivWRTInput)
    {
        // The partial derivative of the cost with respect to the input
        // is the same as the partial derivative of the cost with respect to the output
        EXPECT_EQ((*costPartDerivWRTInput)[index], 2.0f * (*output)[index]);
    }

    for (const TensorIndex<4>& index : *costPartDerivWRTParameter)
    {
        // The partial derivative of the cost with respect to the parameter
        // is the same as the sum of partial derivatives of the cost with
        // respect to the related output neurons.
        TensorIndex<5> outputIndex = TensorIndex<1>{0} * index;

        float expectedValue = 0.0f;
        for (outputIndex[0] = 0; outputIndex[0] < commonSize[0]; ++outputIndex[0])
        {
            expectedValue += 2.0f * (*output)[outputIndex];
        }

        EXPECT_EQ((*costPartDerivWRTParameter)[index], expectedValue);
    }

    // Free the partial derivatives tensors
    delete costPartDerivWRTInput;
    delete costPartDerivWRTParameter;
    delete costPartDerivWRTOutput;
}
