#include <plugins/natives/modules/perceptronmodule/PerceptronModule.h>

#include <plugins/natives/modules/perceptronmodule/PerceptronModuleImpl.h>
#include <plugins/natives/api.h>

#include <module/api.h>

IMPLEMENT_MODULE_WRAPPER(
    PerceptronModule,
    ValueType,
    PerceptronModuleImpl,
    PACK(RawTuple<const TensorIndex<InputDimension + OutputDimension>&, const TensorIndex<OutputDimension>&>),
    PACK(typename, size_t, size_t),
    PACK(ValueType, InputDimension, OutputDimension)
)

extern template class MY_NEURAL_NETWORK_LIB__MODULE__API Module<float>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<float, 0, 0>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<float, 1, 0>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<float, 2, 0>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<float, 3, 0>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<float, 0, 1>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<float, 1, 1>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<float, 2, 1>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<float, 3, 1>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<float, 0, 2>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<float, 1, 2>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<float, 2, 2>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<float, 3, 2>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<float, 0, 3>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<float, 1, 3>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<float, 2, 3>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<float, 3, 3>;

extern template class MY_NEURAL_NETWORK_LIB__MODULE__API Module<double>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<double, 0, 0>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<double, 1, 0>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<double, 2, 0>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<double, 3, 0>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<double, 0, 1>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<double, 1, 1>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<double, 2, 1>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<double, 3, 1>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<double, 0, 2>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<double, 1, 2>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<double, 2, 2>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<double, 3, 2>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<double, 0, 3>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<double, 1, 3>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<double, 2, 3>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API PerceptronModule<double, 3, 3>;
