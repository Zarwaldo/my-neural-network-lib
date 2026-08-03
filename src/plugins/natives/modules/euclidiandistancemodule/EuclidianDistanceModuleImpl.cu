#include <plugins/natives/modules/euclidiandistancemodule/EuclidianDistanceModuleImpl.h>

#include <module/api.h>

#include <module/ModuleMacros.impl.cuh>

IMPLEMENT_MODULE(
    EuclidianDistanceModuleImpl,
    ValueType,
    TensorPair,
    TensorSingleton,
    TensorSingleton,
    PACK(Dimension + 1, Dimension + 1),
    PACK(0),
    PACK(0),
    PACK(typename, size_t),
    PACK(ValueType, Dimension)
)

template <typename ValueType, size_t Dimension>
HOST
bool
EuclidianDistanceModuleImpl<ValueType, Dimension>::areSizesCorrect(const RawTuple<const TensorIndex<Dimension + 1>&, const TensorIndex<Dimension + 1>&>& inputTensorsSizes, const RawTuple<const TensorIndex<0>&>& parameterTensorsSizes, const RawTuple<const TensorIndex<0>&>& outputTensorsSizes)
{
    return inputTensorsSizes.template get<0>() == inputTensorsSizes.template get<1>();
}

template <typename ValueType, size_t Dimension>
DEVICE
void
EuclidianDistanceModuleImpl<ValueType, Dimension>::computationKernel__SINGLE_TENSOR(RawTensor<ValueType, 0>* output__SINGLE_TENSOR, const RawTensor<ValueType, 0>* param__SINGLE_TENSOR, const RawTensor<ValueType, Dimension + 1>* input__FIRST_TENSOR, const RawTensor<ValueType, Dimension + 1>* input__SECOND_TENSOR)
{
    ValueType result = {};

    TensorThreadDistributor<ValueType, 0> distributor(*output__SINGLE_TENSOR);
    distributor.iterate([output__SINGLE_TENSOR, input__FIRST_TENSOR, input__SECOND_TENSOR, &result](const RawTensorIndex<0>& outputIndex) {
        if (!(outputIndex < output__SINGLE_TENSOR->sizes()))
            return;

        result = {};
        for (const RawTensorIndex<Dimension + 1>& inputIndex : *input__FIRST_TENSOR)
        {
            result += pow((*input__FIRST_TENSOR)[inputIndex] - (*input__SECOND_TENSOR)[inputIndex], 2);
        }

        (*output__SINGLE_TENSOR)[outputIndex] = result;
    });
}

template <typename ValueType, size_t Dimension>
DEVICE
void
EuclidianDistanceModuleImpl<ValueType, Dimension>::inputBackpropagationKernel__FIRST_TENSOR(RawTensor<ValueType, Dimension + 1>* costPartDerivWRTInput__FIRST_TENSOR, const RawTensor<ValueType, 0>* costPartDerivWRTOutput__SINGLE_TENSOR, const RawTensor<ValueType, 0>* output__SINGLE_TENSOR, const RawTensor<ValueType, 0>* param__SINGLE_TENSOR, const RawTensor<ValueType, Dimension + 1>* input__FIRST_TENSOR, const RawTensor<ValueType, Dimension + 1>* input__SECOND_TENSOR)
{
    TensorThreadDistributor<ValueType, Dimension + 1> distributor(*costPartDerivWRTInput__FIRST_TENSOR);
    distributor.iterate([costPartDerivWRTInput__FIRST_TENSOR, costPartDerivWRTOutput__SINGLE_TENSOR, input__FIRST_TENSOR, input__SECOND_TENSOR](const RawTensorIndex<Dimension + 1>& index) {
        if (!(index < costPartDerivWRTInput__FIRST_TENSOR->sizes()))
            return;

        (*costPartDerivWRTInput__FIRST_TENSOR)[index] = 2 * ((*input__FIRST_TENSOR)[index] - (*input__SECOND_TENSOR)[index]) * (*costPartDerivWRTOutput__SINGLE_TENSOR)[RawTensorIndex<0>(true)];
    });
}

template <typename ValueType, size_t Dimension>
DEVICE
void
EuclidianDistanceModuleImpl<ValueType, Dimension>::inputBackpropagationKernel__SECOND_TENSOR(RawTensor<ValueType, Dimension + 1>* costPartDerivWRTInput__SECOND_TENSOR, const RawTensor<ValueType, 0>* costPartDerivWRTOutput__SINGLE_TENSOR, const RawTensor<ValueType, 0>* output__SINGLE_TENSOR, const RawTensor<ValueType, 0>* param__SINGLE_TENSOR, const RawTensor<ValueType, Dimension + 1>* input__FIRST_TENSOR, const RawTensor<ValueType, Dimension + 1>* input__SECOND_TENSOR)
{
    TensorThreadDistributor<ValueType, Dimension + 1> distributor(*costPartDerivWRTInput__SECOND_TENSOR);
    distributor.iterate([costPartDerivWRTInput__SECOND_TENSOR, costPartDerivWRTOutput__SINGLE_TENSOR, input__FIRST_TENSOR, input__SECOND_TENSOR](const RawTensorIndex<Dimension + 1>& index) {
        if (!(index < costPartDerivWRTInput__SECOND_TENSOR->sizes()))
            return;

        (*costPartDerivWRTInput__SECOND_TENSOR)[index] = 2 * ((*input__SECOND_TENSOR)[index] - (*input__FIRST_TENSOR)[index]) * (*costPartDerivWRTOutput__SINGLE_TENSOR)[RawTensorIndex<0>(true)];
    });
}

template <typename ValueType, size_t Dimension>
DEVICE
void
EuclidianDistanceModuleImpl<ValueType, Dimension>::parameterBackpropagationKernel__SINGLE_TENSOR(RawTensor<ValueType, 0>* costPartDerivWRTParam__SINGLE_TENSOR, const RawTensor<ValueType, 0>* costPartDerivWRTOutput__SINGLE_TENSOR, const RawTensor<ValueType, 0>* output__SINGLE_TENSOR, const RawTensor<ValueType, 0>* param__SINGLE_TENSOR, const RawTensor<ValueType, Dimension + 1>* input__FIRST_TENSOR, const RawTensor<ValueType, Dimension + 1>* input__SECOND_TENSOR)
{
    TensorThreadDistributor<ValueType, 0> distributor(*costPartDerivWRTParam__SINGLE_TENSOR);
    distributor.iterate([costPartDerivWRTParam__SINGLE_TENSOR](const RawTensorIndex<0>& index) {
        if (!(index < costPartDerivWRTParam__SINGLE_TENSOR->sizes()))
            return;

        (*costPartDerivWRTParam__SINGLE_TENSOR)[index] = ValueType{};
    });
}

extern template class MY_NEURAL_NETWORK_LIB__MODULE__API Module<float>;
template class EuclidianDistanceModuleImpl<float, 0>;
template class EuclidianDistanceModuleImpl<float, 1>;
template class EuclidianDistanceModuleImpl<float, 2>;
template class EuclidianDistanceModuleImpl<float, 3>;
template class EuclidianDistanceModuleImpl<float, 4>;
template class EuclidianDistanceModuleImpl<float, 5>;

extern template class MY_NEURAL_NETWORK_LIB__MODULE__API Module<double>;
template class EuclidianDistanceModuleImpl<double, 0>;
template class EuclidianDistanceModuleImpl<double, 1>;
template class EuclidianDistanceModuleImpl<double, 2>;
template class EuclidianDistanceModuleImpl<double, 3>;
template class EuclidianDistanceModuleImpl<double, 4>;
template class EuclidianDistanceModuleImpl<double, 5>;
