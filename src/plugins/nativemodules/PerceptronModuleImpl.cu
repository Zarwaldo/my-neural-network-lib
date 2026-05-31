#include <plugins/nativemodules/PerceptronModuleImpl.h>

#include <module/api.h>
#include <module/ModuleMacros.impl.cuh>

#include <CommonMacros.h>

IMPLEMENT_MODULE(
    PerceptronModuleImpl,
    ValueType,
    TensorSingleton,
    PerceptronParamsKeyEnum,
    TensorSingleton,
    PACK(InputDimension + 1),
    PACK(InputDimension + OutputDimension, OutputDimension),
    PACK(OutputDimension + 1),
    PACK(typename, size_t, size_t),
    PACK(ValueType, InputDimension, OutputDimension)
)

template <typename ValueType, size_t InputDimension, size_t OutputDimension>
HOST
bool
PerceptronModuleImpl<ValueType, InputDimension, OutputDimension>::areSizesCorrect(const RawTuple<const TensorIndex<InputDimension + 1>&>& inputTensorsSizes, const RawTuple<const TensorIndex<InputDimension + OutputDimension>&, const TensorIndex<OutputDimension>&>& parameterTensorsSizes, const RawTuple<const TensorIndex<OutputDimension + 1>&>& outputTensorsSizes)
{
    const TensorIndex<InputDimension + 1>& inputTensorSize = inputTensorsSizes.template get<0>();
    const TensorIndex<InputDimension + OutputDimension>& weightsTensorSize = parameterTensorsSizes.template get<0>();
    const TensorIndex<OutputDimension>& biasesTensorSize = parameterTensorsSizes.template get<1>();
    const TensorIndex<OutputDimension + 1>& outputTensorSize = outputTensorsSizes.template get<0>();
    return weightsTensorSize == (inputTensorSize.template range<1, -1>() * outputTensorSize.template range<1, -1>()) && biasesTensorSize == outputTensorSize.template range<1, -1>();
}

template <typename ValueType, size_t InputDimension, size_t OutputDimension>
DEVICE
void
PerceptronModuleImpl<ValueType, InputDimension, OutputDimension>::computationKernel__SINGLE_TENSOR(RawTensor<ValueType, OutputDimension + 1>* output, const RawTensor<ValueType, InputDimension + OutputDimension>* weights, const RawTensor<ValueType, OutputDimension>* biases, const RawTensor<ValueType, InputDimension + 1>* input)
{
    ValueType result;
    RawTensorIndex<InputDimension + OutputDimension> weightIndex;
    RawTensorIndex<OutputDimension> biasIndex;

    const size_t zeros[InputDimension + 1] = {};
    RawTensorIndex<InputDimension + 1> inputSubTensorMinIndex(zeros);
    RawTensorIndex<InputDimension + 1> inputSubTensorMaxIndex = input->sizes();
    RawTensor<ValueType, InputDimension + 1> inputSubTensor = input->subtensor(inputSubTensorMinIndex, inputSubTensorMaxIndex);

    TensorThreadDistributor<ValueType, OutputDimension + 1> distributor(*output);
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

template <typename ValueType, size_t InputDimension, size_t OutputDimension>
DEVICE
void
PerceptronModuleImpl<ValueType, InputDimension, OutputDimension>::inputBackpropagationKernel__SINGLE_TENSOR(RawTensor<ValueType, InputDimension + 1>* costPartDerivWRTInput, const RawTensor<ValueType, OutputDimension + 1>* costPartDerivWRTOutput, const RawTensor<ValueType, OutputDimension + 1>* output, const RawTensor<ValueType, InputDimension + OutputDimension>* weights, const RawTensor<ValueType, OutputDimension>* biases, const RawTensor<ValueType, InputDimension + 1>* input)
{
    ValueType result;
    RawTensorIndex<InputDimension + OutputDimension> weightIndex;

    const size_t zeros[OutputDimension + 1] = {};
    RawTensorIndex<OutputDimension + 1> costPartDerivWRTOutputSubTensorMinIndex(zeros);
    RawTensorIndex<OutputDimension + 1> costPartDerivWRTOutputSubTensorMaxIndex = costPartDerivWRTOutput->sizes();
    RawTensor<ValueType, OutputDimension + 1> costPartDerivWRTOutputSubTensor = costPartDerivWRTOutput->subtensor(costPartDerivWRTOutputSubTensorMinIndex, costPartDerivWRTOutputSubTensorMaxIndex);

    TensorThreadDistributor<ValueType, InputDimension + 1> distributor(*costPartDerivWRTInput);
    distributor.iterate([costPartDerivWRTInput, costPartDerivWRTOutput, weights, &result, &weightIndex, &costPartDerivWRTOutputSubTensorMinIndex, &costPartDerivWRTOutputSubTensorMaxIndex, &costPartDerivWRTOutputSubTensor](const RawTensorIndex<InputDimension + 1>& inputIndex) {
        if (!(inputIndex < costPartDerivWRTInput->sizes()))
            return;

        costPartDerivWRTOutputSubTensorMinIndex[0] = inputIndex[0];
        costPartDerivWRTOutputSubTensorMaxIndex[0] = inputIndex[0] + 1;
        costPartDerivWRTOutputSubTensor = costPartDerivWRTOutput->subtensor(costPartDerivWRTOutputSubTensorMinIndex, costPartDerivWRTOutputSubTensorMaxIndex);

        result = ValueType{};

        for (const RawTensorIndex<OutputDimension + 1>& outputIndex : costPartDerivWRTOutputSubTensor)
        {
            weightIndex = inputIndex.template range<1, -1>() * outputIndex.template range<1, -1>();
            result += costPartDerivWRTOutputSubTensor[outputIndex] * (*weights)[weightIndex];
        }

        (*costPartDerivWRTInput)[inputIndex] = result;
    });
}

template <typename ValueType, size_t InputDimension, size_t OutputDimension>
DEVICE
void
PerceptronModuleImpl<ValueType, InputDimension, OutputDimension>::parameterBackpropagationKernel__WEIGHTS(RawTensor<ValueType, InputDimension + OutputDimension>* costPartDerivWRTWeights, const RawTensor<ValueType, OutputDimension + 1>* costPartDerivWRTOutput, const RawTensor<ValueType, OutputDimension + 1>* output, const RawTensor<ValueType, InputDimension + OutputDimension>* weights, const RawTensor<ValueType, OutputDimension>* biases, const RawTensor<ValueType, InputDimension + 1>* input)
{
    ValueType result;
    RawTensorIndex<InputDimension + 1> inputIndex;
    RawTensorIndex<OutputDimension + 1> outputIndex;

    TensorThreadDistributor<ValueType, InputDimension + OutputDimension> distributor(*costPartDerivWRTWeights);
    distributor.iterate([costPartDerivWRTWeights, costPartDerivWRTOutput, input, &result, &inputIndex, &outputIndex](const RawTensorIndex<InputDimension + OutputDimension>& weightIndex) {
        if (!(weightIndex < costPartDerivWRTWeights->sizes()))
            return;

        result = ValueType{};
        inputIndex = RawTensorIndex<1>{0} * weightIndex.template range<0, InputDimension - 1>();
        outputIndex = RawTensorIndex<1>{0} * weightIndex.template range<InputDimension, InputDimension + OutputDimension - 1>();

        for (inputIndex[0] = 0; inputIndex[0] < costPartDerivWRTOutput->sizes()[0]; ++inputIndex[0])
        {
            outputIndex[0] = inputIndex[0];
            result += (*costPartDerivWRTOutput)[outputIndex] * (*input)[inputIndex];
        }

        (*costPartDerivWRTWeights)[weightIndex] = result;
    });
}

template <typename ValueType, size_t InputDimension, size_t OutputDimension>
DEVICE
void
PerceptronModuleImpl<ValueType, InputDimension, OutputDimension>::parameterBackpropagationKernel__BIASES(RawTensor<ValueType, OutputDimension>* costPartDerivWRTBiases, const RawTensor<ValueType, OutputDimension + 1>* costPartDerivWRTOutput, const RawTensor<ValueType, OutputDimension + 1>* output, const RawTensor<ValueType, InputDimension + OutputDimension>* weights, const RawTensor<ValueType, OutputDimension>* biases, const RawTensor<ValueType, InputDimension + 1>* input)
{
    ValueType result;
    RawTensorIndex<OutputDimension + 1> outputIndex;

    TensorThreadDistributor<ValueType, OutputDimension> distributor(*costPartDerivWRTBiases);
    distributor.iterate([costPartDerivWRTBiases, costPartDerivWRTOutput, &result, &outputIndex](const RawTensorIndex<OutputDimension>& biasIndex) {
        if (!(biasIndex < costPartDerivWRTBiases->sizes()))
            return;

        result = ValueType{};

        outputIndex = RawTensorIndex<1>{0} * biasIndex;
        for (outputIndex[0] = 0; outputIndex[0] < costPartDerivWRTOutput->sizes()[0]; ++outputIndex[0])
        {
            result += (*costPartDerivWRTOutput)[outputIndex];
        }

        (*costPartDerivWRTBiases)[biasIndex] = result;
    });
}

extern template class MY_NEURAL_NETWORK_LIB__MODULE__API Module<float>;
template class PerceptronModuleImpl<float, 1, 1>;
template class PerceptronModuleImpl<float, 2, 1>;
template class PerceptronModuleImpl<float, 3, 1>;
template class PerceptronModuleImpl<float, 1, 2>;
template class PerceptronModuleImpl<float, 2, 2>;
template class PerceptronModuleImpl<float, 3, 2>;
template class PerceptronModuleImpl<float, 1, 3>;
template class PerceptronModuleImpl<float, 2, 3>;
template class PerceptronModuleImpl<float, 3, 3>;

extern template class MY_NEURAL_NETWORK_LIB__MODULE__API Module<double>;
template class PerceptronModuleImpl<double, 1, 1>;
template class PerceptronModuleImpl<double, 2, 1>;
template class PerceptronModuleImpl<double, 3, 1>;
template class PerceptronModuleImpl<double, 1, 2>;
template class PerceptronModuleImpl<double, 2, 2>;
template class PerceptronModuleImpl<double, 3, 2>;
template class PerceptronModuleImpl<double, 1, 3>;
template class PerceptronModuleImpl<double, 2, 3>;
template class PerceptronModuleImpl<double, 3, 3>;
