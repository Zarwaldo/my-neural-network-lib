#include <plugins/natives/tensormaps/TensorMapKeyEnums.h>

#include <tensor/TensorMap.impl.h>

#include <module/AbstractModule.h>

IMPLEMENT_TENSOR_MAP_KEY_ENUM(TensorSingleton)

template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API TensorMap<float, TensorSingleton>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API TensorMap<double, TensorSingleton>;

IMPLEMENT_TENSOR_MAP_KEY_ENUM(PerceptronParamsKeyEnum)

template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API TensorMap<float, PerceptronParamsKeyEnum>;
template class MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API TensorMap<double, PerceptronParamsKeyEnum>;
