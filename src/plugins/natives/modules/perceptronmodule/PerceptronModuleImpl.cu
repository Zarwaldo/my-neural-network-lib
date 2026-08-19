#include <plugins/natives/modules/perceptronmodule/PerceptronModuleImpl.h>

#include <module/api.h>
#include <module/ModuleMacros.impl.cuh>

#include <CommonMacros.h>

IMPLEMENT_MODULE(
    PerceptronModuleImpl,
    ScalarType,
    TensorSingleton,
    PerceptronParamsKeyEnum,
    TensorSingleton,
    PACK(InputDimension + 1),
    PACK(InputDimension + OutputDimension, OutputDimension),
    PACK(OutputDimension + 1),
    PACK(typename, size_t, size_t),
    PACK(ScalarType, InputDimension, OutputDimension)
)

template <typename ScalarType, size_t InputDimension, size_t OutputDimension>
HOST
bool
PerceptronModuleImpl<ScalarType, InputDimension, OutputDimension>::areSizesCorrect(const RawTuple<const TensorIndex<InputDimension + 1>&>& inputTensorsSizes, const RawTuple<const TensorIndex<InputDimension + OutputDimension>&, const TensorIndex<OutputDimension>&>& parameterTensorsSizes, const RawTuple<const TensorIndex<OutputDimension + 1>&>& outputTensorsSizes)
{
    const TensorIndex<InputDimension + 1>& inputTensorSize = inputTensorsSizes.template get<0>();
    const TensorIndex<InputDimension + OutputDimension>& weightsTensorSize = parameterTensorsSizes.template get<0>();
    const TensorIndex<OutputDimension>& biasesTensorSize = parameterTensorsSizes.template get<1>();
    const TensorIndex<OutputDimension + 1>& outputTensorSize = outputTensorsSizes.template get<0>();
    return weightsTensorSize == (inputTensorSize.template range<1, -1>() * outputTensorSize.template range<1, -1>()) && biasesTensorSize == outputTensorSize.template range<1, -1>();
}

template <typename ScalarType, size_t InputDimension, size_t OutputDimension>
DEVICE
void
PerceptronModuleImpl<ScalarType, InputDimension, OutputDimension>::computationKernel__SINGLE_TENSOR(RawTensor<ScalarType, OutputDimension + 1>* output, const RawTensor<ScalarType, InputDimension + OutputDimension>* weights, const RawTensor<ScalarType, OutputDimension>* biases, const RawTensor<ScalarType, InputDimension + 1>* input)
{
    ScalarType result;
    RawTensorIndex<InputDimension + OutputDimension> weightIndex;
    RawTensorIndex<OutputDimension> biasIndex;

    const size_t zeros[InputDimension + 1] = {};
    RawTensorIndex<InputDimension + 1> inputSubTensorMinIndex(zeros);
    RawTensorIndex<InputDimension + 1> inputSubTensorMaxIndex = input->sizes();
    RawTensor<ScalarType, InputDimension + 1> inputSubTensor = input->subtensor(inputSubTensorMinIndex, inputSubTensorMaxIndex);

    TensorThreadDistributor<ScalarType, OutputDimension + 1> distributor(*output);
    distributor.iterate([output, weights, biases, input, &result, &weightIndex, &biasIndex, &inputSubTensorMinIndex, &inputSubTensorMaxIndex, &inputSubTensor](const RawTensorIndex<OutputDimension + 1>& outputIndex) {
        if (!(outputIndex < output->sizes()))
            return;

        biasIndex = outputIndex.template range<1, -1>();
        inputSubTensorMinIndex[0] = outputIndex[0];
        inputSubTensorMaxIndex[0] = outputIndex[0] + 1;
        inputSubTensor = input->subtensor(inputSubTensorMinIndex, inputSubTensorMaxIndex);

        result = (*biases)[biasIndex];

        for (const RawTensorIndex<InputDimension + 1>& inputIndex : inputSubTensor)
        {
            weightIndex = inputIndex.template range<1, -1>() * biasIndex;
            result += inputSubTensor[inputIndex] * (*weights)[weightIndex];
        }

        (*output)[outputIndex] = result;
    });
}

template <typename ScalarType, size_t InputDimension, size_t OutputDimension>
DEVICE
void
PerceptronModuleImpl<ScalarType, InputDimension, OutputDimension>::inputBackpropagationKernel__SINGLE_TENSOR(RawTensor<ScalarType, InputDimension + 1>* costPartDerivWRTInput, const RawTensor<ScalarType, OutputDimension + 1>* costPartDerivWRTOutput, const RawTensor<ScalarType, OutputDimension + 1>* output, const RawTensor<ScalarType, InputDimension + OutputDimension>* weights, const RawTensor<ScalarType, OutputDimension>* biases, const RawTensor<ScalarType, InputDimension + 1>* input)
{
    ScalarType result;
    RawTensorIndex<InputDimension + OutputDimension> weightIndex;

    const size_t zeros[OutputDimension + 1] = {};
    RawTensorIndex<OutputDimension + 1> costPartDerivWRTOutputSubTensorMinIndex(zeros);
    RawTensorIndex<OutputDimension + 1> costPartDerivWRTOutputSubTensorMaxIndex = costPartDerivWRTOutput->sizes();
    RawTensor<ScalarType, OutputDimension + 1> costPartDerivWRTOutputSubTensor = costPartDerivWRTOutput->subtensor(costPartDerivWRTOutputSubTensorMinIndex, costPartDerivWRTOutputSubTensorMaxIndex);

    TensorThreadDistributor<ScalarType, InputDimension + 1> distributor(*costPartDerivWRTInput);
    distributor.iterate([costPartDerivWRTInput, costPartDerivWRTOutput, weights, &result, &weightIndex, &costPartDerivWRTOutputSubTensorMinIndex, &costPartDerivWRTOutputSubTensorMaxIndex, &costPartDerivWRTOutputSubTensor](const RawTensorIndex<InputDimension + 1>& inputIndex) {
        if (!(inputIndex < costPartDerivWRTInput->sizes()))
            return;

        costPartDerivWRTOutputSubTensorMinIndex[0] = inputIndex[0];
        costPartDerivWRTOutputSubTensorMaxIndex[0] = inputIndex[0] + 1;
        costPartDerivWRTOutputSubTensor = costPartDerivWRTOutput->subtensor(costPartDerivWRTOutputSubTensorMinIndex, costPartDerivWRTOutputSubTensorMaxIndex);

        result = ScalarType{};

        for (const RawTensorIndex<OutputDimension + 1>& outputIndex : costPartDerivWRTOutputSubTensor)
        {
            weightIndex = inputIndex.template range<1, -1>() * outputIndex.template range<1, -1>();
            result += costPartDerivWRTOutputSubTensor[outputIndex] * (*weights)[weightIndex];
        }

        (*costPartDerivWRTInput)[inputIndex] = result;
    });
}

template <typename ScalarType, size_t InputDimension, size_t OutputDimension>
DEVICE
void
PerceptronModuleImpl<ScalarType, InputDimension, OutputDimension>::parameterBackpropagationKernel__WEIGHTS(RawTensor<ScalarType, InputDimension + OutputDimension>* costPartDerivWRTWeights, const RawTensor<ScalarType, OutputDimension + 1>* costPartDerivWRTOutput, const RawTensor<ScalarType, OutputDimension + 1>* output, const RawTensor<ScalarType, InputDimension + OutputDimension>* weights, const RawTensor<ScalarType, OutputDimension>* biases, const RawTensor<ScalarType, InputDimension + 1>* input)
{
    ScalarType result;
    RawTensorIndex<InputDimension + 1> inputIndex;
    RawTensorIndex<OutputDimension + 1> outputIndex;

    TensorThreadDistributor<ScalarType, InputDimension + OutputDimension> distributor(*costPartDerivWRTWeights);
    distributor.iterate([costPartDerivWRTWeights, costPartDerivWRTOutput, input, &result, &inputIndex, &outputIndex](const RawTensorIndex<InputDimension + OutputDimension>& weightIndex) {
        if (!(weightIndex < costPartDerivWRTWeights->sizes()))
            return;

        result = ScalarType{};
        inputIndex = RawTensorIndex<1>{0} * weightIndex.template range<0, InputDimension>();
        outputIndex = RawTensorIndex<1>{0} * weightIndex.template range<InputDimension, InputDimension + OutputDimension>();

        for (inputIndex[0] = 0; inputIndex[0] < costPartDerivWRTOutput->sizes()[0]; ++inputIndex[0])
        {
            outputIndex[0] = inputIndex[0];
            result += (*costPartDerivWRTOutput)[outputIndex] * (*input)[inputIndex];
        }

        (*costPartDerivWRTWeights)[weightIndex] = result;
    });
}

template <typename ScalarType, size_t InputDimension, size_t OutputDimension>
DEVICE
void
PerceptronModuleImpl<ScalarType, InputDimension, OutputDimension>::parameterBackpropagationKernel__BIASES(RawTensor<ScalarType, OutputDimension>* costPartDerivWRTBiases, const RawTensor<ScalarType, OutputDimension + 1>* costPartDerivWRTOutput, const RawTensor<ScalarType, OutputDimension + 1>* output, const RawTensor<ScalarType, InputDimension + OutputDimension>* weights, const RawTensor<ScalarType, OutputDimension>* biases, const RawTensor<ScalarType, InputDimension + 1>* input)
{
    ScalarType result;
    RawTensorIndex<OutputDimension + 1> outputIndex;

    TensorThreadDistributor<ScalarType, OutputDimension> distributor(*costPartDerivWRTBiases);
    distributor.iterate([costPartDerivWRTBiases, costPartDerivWRTOutput, &result, &outputIndex](const RawTensorIndex<OutputDimension>& biasIndex) {
        if (!(biasIndex < costPartDerivWRTBiases->sizes()))
            return;

        result = ScalarType{};

        outputIndex = RawTensorIndex<1>{0} * biasIndex;
        for (outputIndex[0] = 0; outputIndex[0] < costPartDerivWRTOutput->sizes()[0]; ++outputIndex[0])
        {
            result += (*costPartDerivWRTOutput)[outputIndex];
        }

        (*costPartDerivWRTBiases)[biasIndex] = result;
    });
}

extern template class MY_NEURAL_NETWORK_LIB__MODULE__API Module<float>;
template class PerceptronModuleImpl<float, 0, 0>;
template class PerceptronModuleImpl<float, 1, 0>;
template class PerceptronModuleImpl<float, 2, 0>;
template class PerceptronModuleImpl<float, 3, 0>;
template class PerceptronModuleImpl<float, 0, 1>;
template class PerceptronModuleImpl<float, 1, 1>;
template class PerceptronModuleImpl<float, 2, 1>;
template class PerceptronModuleImpl<float, 3, 1>;
template class PerceptronModuleImpl<float, 0, 2>;
template class PerceptronModuleImpl<float, 1, 2>;
template class PerceptronModuleImpl<float, 2, 2>;
template class PerceptronModuleImpl<float, 3, 2>;
template class PerceptronModuleImpl<float, 0, 3>;
template class PerceptronModuleImpl<float, 1, 3>;
template class PerceptronModuleImpl<float, 2, 3>;
template class PerceptronModuleImpl<float, 3, 3>;

extern template class MY_NEURAL_NETWORK_LIB__MODULE__API Module<double>;
template class PerceptronModuleImpl<double, 0, 0>;
template class PerceptronModuleImpl<double, 1, 0>;
template class PerceptronModuleImpl<double, 2, 0>;
template class PerceptronModuleImpl<double, 3, 0>;
template class PerceptronModuleImpl<double, 0, 1>;
template class PerceptronModuleImpl<double, 1, 1>;
template class PerceptronModuleImpl<double, 2, 1>;
template class PerceptronModuleImpl<double, 3, 1>;
template class PerceptronModuleImpl<double, 0, 2>;
template class PerceptronModuleImpl<double, 1, 2>;
template class PerceptronModuleImpl<double, 2, 2>;
template class PerceptronModuleImpl<double, 3, 2>;
template class PerceptronModuleImpl<double, 0, 3>;
template class PerceptronModuleImpl<double, 1, 3>;
template class PerceptronModuleImpl<double, 2, 3>;
template class PerceptronModuleImpl<double, 3, 3>;
