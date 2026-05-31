#include <gtest/gtest.h>

#include <plugins/nativemodules/PerceptronModule.h>

#include <module/MainTensorMapKeyEnums.h>

#include <tensor/TensorMap.h>

class PerceptronModuleShould : public ::testing::Test
{
protected:
    const TensorIndex<3> inputSize = {4, 16, 16};
    Tensor<float, 3>* input;
    TensorMap<float, TensorSingleton>* inputMap;
    const TensorIndex<2> outputSize = {4, 16};
    Tensor<float, 2>* output;
    TensorMap<float, TensorSingleton>* outputMap;

    void SetUp() override
    {
        input = Tensor<float, 3>::create(inputSize);
        inputMap = new TensorMap<float, TensorSingleton>({input});

        output = Tensor<float, 2>::create(outputSize);
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
        for (const TensorIndex<3>& index : *input)
        {
            (*input)[index] = static_cast<float>(index[0] + index[1] + index[2]);
        }
    }

    void initializeWeightsTensor(AbstractTensor<float>& weightsTensor)
    {
        for (const TensorIndex<3>& index : static_cast<Tensor<float, 3>&>(weightsTensor))
        {
            // This perceptron sums up each line of the input tensor
            // to the corresponding value of the output tensor
            weightsTensor[index] = (index[0] == index[2]) ? 1.0f : 0.0f;
        }
    }

    void initializeBiasesTensor(AbstractTensor<float>& biasesTensor)
    {
        for (const TensorIndex<1>& index : static_cast<Tensor<float, 1>&>(biasesTensor))
        {
            // The bias of each sumed up line is the index of this line
            biasesTensor[index] = static_cast<float>(index[0]);
        }
    }
};

TEST_F(PerceptronModuleShould, sumUpRowsOfTheInputTensorAddingRowIndex) {
    // Given an perceptron module plugged to an input tensor and an output tensor
    // with its parameter tensors initialized.
    const TensorIndex<3> weightsTensorSize = inputSize.template range<1, -1>() * outputSize.template range<1, -1>();
    const TensorIndex<1> biasesTensorSize = outputSize.template range<1, -1>();
    PerceptronModule<float, 2, 1> module(makeRawTuple(weightsTensorSize, biasesTensorSize));

    module.setInputTensorMap(inputMap);
    module.setOutputTensorMap(outputMap);

    TensorMap<float, PerceptronParamsKeyEnum>& parameterMap = static_cast<TensorMap<float, PerceptronParamsKeyEnum>&>(module.getParameterAbstractTensorMap());
    initializeWeightsTensor(parameterMap.get(WEIGHTS));
    initializeBiasesTensor(parameterMap.get(BIASES));

    // When the module is computed
    module.compute();

    // Then the output tensor equals the sum of each row of the input tensor, added to the row index.
    // All computation instances (indexed on the first coordinate of each tensor) are computed in parallel.
    for (const TensorIndex<2>& index : *output)
    {
        const size_t resultId = index[0];
        const size_t sumedUpRowId = index[1];

        // Compute expected value
        float expected = index[1];
        TensorIndex<3> inputIndex = {resultId, sumedUpRowId, 0};
        for (inputIndex[2] = 0; inputIndex[2] < input->sizes()[2]; ++inputIndex[2])
        {
            expected += (*input)[inputIndex];
        }

        EXPECT_EQ((*output)[index], expected);
    }
}

TEST_F(PerceptronModuleShould, computeBackpropagation) {
    // Given an perceptron module plugged to an input tensor and an output tensor
    // after computation and some cost partial derivative tensors
    const TensorIndex<3> weightsTensorSize = inputSize.template range<1, -1>() * outputSize.template range<1, -1>();
    const TensorIndex<1> biasesTensorSize = outputSize.template range<1, -1>();
    PerceptronModule<float, 2, 1> module(makeRawTuple(weightsTensorSize, biasesTensorSize));

    module.setInputTensorMap(inputMap);
    module.setOutputTensorMap(outputMap);

    TensorMap<float, PerceptronParamsKeyEnum>& parameterMap = dynamic_cast<TensorMap<float, PerceptronParamsKeyEnum>&>(module.getParameterAbstractTensorMap());
    initializeWeightsTensor(parameterMap.get(WEIGHTS));
    initializeBiasesTensor(parameterMap.get(BIASES));

    module.compute();

    Tensor<float, 3>* costPartDerivWRTInput = Tensor<float, 3>::create(inputSize);
    TensorMap<float, TensorSingleton>* costPartDerivWRTInputMap = new TensorMap<float, TensorSingleton>({costPartDerivWRTInput});

    Tensor<float, 3>* costPartDerivWRTWeights = Tensor<float, 3>::create(weightsTensorSize);
    Tensor<float, 1>* costPartDerivWRTBiases = Tensor<float, 1>::create(biasesTensorSize);
    TensorMap<float, PerceptronParamsKeyEnum>* costPartDerivWRTParametersMap = new TensorMap<float, PerceptronParamsKeyEnum>({costPartDerivWRTWeights, costPartDerivWRTBiases});

    Tensor<float, 2>* costPartDerivWRTOutput = Tensor<float, 2>::create(outputSize);
    TensorMap<float, TensorSingleton>* costPartDerivWRTOutputMap = new TensorMap<float, TensorSingleton>({costPartDerivWRTOutput});

    for (const TensorIndex<2>& index : *costPartDerivWRTOutput)
    {
        (*costPartDerivWRTOutput)[index] = static_cast<float>(index[0] + index[1]);
    }

    // When the module is backpropagated
    module.backpropagate(*costPartDerivWRTInputMap, *costPartDerivWRTParametersMap, *costPartDerivWRTOutputMap);

    // Then the cost partial derivatives with respect to inputs tensor should be well computed
    for (size_t resultId = 0; resultId < costPartDerivWRTInput->sizes()[0]; ++resultId)
    {
        for (const TensorIndex<3>& inputIndex : *costPartDerivWRTInput)
        {
            if (inputIndex[0] != resultId)
                continue;

            float expected = 0.0f;
            for (const TensorIndex<2>& outputIndex : *costPartDerivWRTOutput)
            {
                if (outputIndex[0] != resultId)
                    continue;

                const TensorIndex<3> weightsIndex = inputIndex.template range<1, -1>() * outputIndex.template range<1, -1>();
                expected += parameterMap.get(WEIGHTS)[weightsIndex] * (*costPartDerivWRTOutput)[outputIndex];
            }

            EXPECT_EQ((*costPartDerivWRTInput)[inputIndex], expected);
        }
    }

    // The cost partial derivatives with respect to weights tensor should be well computed
    for (const TensorIndex<3>& weightIndex : *costPartDerivWRTWeights)
    {
        float expected = 0.0f;
        for (size_t resultId = 0; resultId < input->sizes()[0]; ++resultId)
        {
            const TensorIndex<3> inputIndex = TensorIndex<1>{resultId} * weightIndex.range<0, 1>();
            const TensorIndex<2> outputIndex = TensorIndex<1>{resultId} * weightIndex.range<2, 2>();
            expected += (*input)[inputIndex] * (*costPartDerivWRTOutput)[outputIndex];
        }

        EXPECT_EQ((*costPartDerivWRTWeights)[weightIndex], expected);
    }

    // The cost partial derivatives with respect to biases tensor should be well computed
    for (const TensorIndex<1>& biasIndex : *costPartDerivWRTBiases)
    {
        float expected = 0.0f;
        for (size_t resultId = 0; resultId < input->sizes()[0]; ++resultId)
        {
            const TensorIndex<2> outputIndex = TensorIndex<1>{resultId} * biasIndex;
            expected += (*costPartDerivWRTOutput)[outputIndex];
        }

        EXPECT_EQ((*costPartDerivWRTBiases)[biasIndex], expected);
    }
}
