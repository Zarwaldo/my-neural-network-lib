#include <plugins/nativemodules/AdditionModule.h>

#include <plugins/nativemodules/api.h>
#include <plugins/nativemodules/AdditionModuleImpl.h>

#include <module/api.h>

IMPLEMENT_MODULE_WRAPPER(
    AdditionModule,
    ValueType,
    AdditionModuleImpl,
    PACK(RawTuple<const TensorIndex<Dimension>&>),
    PACK(typename, size_t),
    PACK(ValueType, Dimension)
)

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
