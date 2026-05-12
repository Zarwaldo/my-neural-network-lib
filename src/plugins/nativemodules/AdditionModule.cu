#include <plugins/nativemodules/AdditionModule.h>

#include <plugins/nativemodules/api.h>

#include <module/api.h>
#include <module/ModuleMacros.impl.cuh>

IMPLEMENT_MODULE(
    AdditionModule,
    ValueType,
    TensorSingleton,
    TensorSingleton,
    TensorSingleton,
    PACK(Dimension + 1),
    PACK(Dimension),
    PACK(Dimension + 1),
    PACK(typename, size_t),
    PACK(ValueType, Dimension)
)

template <typename ValueType, size_t Dimension>
HOST
bool
AdditionModule<ValueType, Dimension>::areSizesCorrect(const RawTuple<const TensorIndex<Dimension + 1>&>& inputTensorsSizes, const RawTuple<const TensorIndex<Dimension>&>& parameterTensorsSizes, const RawTuple<const TensorIndex<Dimension + 1>&>& outputTensorsSizes)
{
    const TensorIndex<Dimension + 1>& inputTensorSizesIndex = inputTensorsSizes.template get<0>();
    const TensorIndex<Dimension>& paramTensorSizesIndex = parameterTensorsSizes.template get<0>();
    const TensorIndex<Dimension + 1>& outputTensorSizesIndex = outputTensorsSizes.template get<0>();

    return inputTensorSizesIndex == outputTensorSizesIndex && paramTensorSizesIndex == inputTensorSizesIndex.template range<1, -1>();
}

template <typename ValueType, size_t Dimension>
DEVICE
void
AdditionModule<ValueType, Dimension>::computationKernel__SINGLE_TENSOR(RawTensor<ValueType, Dimension + 1>* output, const RawTensor<ValueType, Dimension>* parameter, const RawTensor<ValueType, Dimension + 1>* input)
{
    TensorThreadDistributor<ValueType, Dimension + 1> distributor(*output);
    distributor.iterate([output, parameter, input](const RawTensorIndex<Dimension + 1>& index) {
        if (!(index < output->sizes()))
            return;

        (*output)[index] = (*parameter)[index.template range<1, -1>()] + (*input)[index];
    });
}

template <typename ValueType, size_t Dimension>
DEVICE
void
AdditionModule<ValueType, Dimension>::inputBackpropagationKernel__SINGLE_TENSOR(RawTensor<ValueType, Dimension + 1>* costPartDerivWRTInput, const RawTensor<ValueType, Dimension + 1>* costPartDerivWRTOutput, const RawTensor<ValueType, Dimension + 1>* output, const RawTensor<ValueType, Dimension>* parameter, const RawTensor<ValueType, Dimension + 1>* input)
{
    TensorThreadDistributor<ValueType, Dimension + 1> distributor(*costPartDerivWRTInput);
    distributor.iterate([costPartDerivWRTInput, costPartDerivWRTOutput](const RawTensorIndex<Dimension + 1>& index) {
        if (!(index < costPartDerivWRTInput->sizes()))
            return;

        (*costPartDerivWRTInput)[index] = (*costPartDerivWRTOutput)[index];
    });
}

template <typename ValueType, size_t Dimension>
DEVICE
void
AdditionModule<ValueType, Dimension>::parameterBackpropagationKernel__SINGLE_TENSOR(RawTensor<ValueType, Dimension>* costPartDerivWRTParameter, const RawTensor<ValueType, Dimension + 1>* costPartDerivWRTOutput, const RawTensor<ValueType, Dimension + 1>* output, const RawTensor<ValueType, Dimension>* parameter, const RawTensor<ValueType, Dimension + 1>* input)
{
    ValueType value;
    RawTensorIndex<Dimension + 1> outputIndex;

    TensorThreadDistributor<ValueType, Dimension> distributor(*costPartDerivWRTParameter);
    distributor.iterate([costPartDerivWRTParameter, costPartDerivWRTOutput, &value, &outputIndex](const RawTensorIndex<Dimension>& index) {
        if (!(index < costPartDerivWRTParameter->sizes()))
            return;

        outputIndex = RawTensorIndex<1>{0} * index;

        value = {};
        size_t nbOutputs = costPartDerivWRTOutput->sizes()[0];
        for (outputIndex[0] = 0; outputIndex[0] < nbOutputs; ++outputIndex[0])
            value += (*costPartDerivWRTOutput)[outputIndex];

        (*costPartDerivWRTParameter)[index] = value;
    });
}

extern template class MY_NEURAL_NETWORK_LIB__MODULE__API Module<float>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API AdditionModule<float, 1>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API AdditionModule<float, 2>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API AdditionModule<float, 3>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API AdditionModule<float, 4>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API AdditionModule<float, 5>;

extern template class MY_NEURAL_NETWORK_LIB__MODULE__API Module<double>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API AdditionModule<double, 1>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API AdditionModule<double, 2>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API AdditionModule<double, 3>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API AdditionModule<double, 4>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API AdditionModule<double, 5>;
