#include <plugins/natives/modules/euclidiandistancemodule/EuclidianDistanceModule.h>

#include <plugins/natives/api.h>
#include <plugins/natives/modules/euclidiandistancemodule/EuclidianDistanceModuleImpl.h>

#include <module/api.h>

IMPLEMENT_MODULE_WRAPPER(
    EuclidianDistanceModule,
    ScalarType,
    EuclidianDistanceModuleImpl,
    PACK(RawTuple<const TensorIndex<0>&>),
    PACK(typename, size_t),
    PACK(ScalarType, Dimension)
)

extern template class MY_NEURAL_NETWORK_LIB__MODULE__API Module<float>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceModule<float, 0>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceModule<float, 1>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceModule<float, 2>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceModule<float, 3>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceModule<float, 4>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceModule<float, 5>;

extern template class MY_NEURAL_NETWORK_LIB__MODULE__API Module<double>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceModule<double, 0>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceModule<double, 1>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceModule<double, 2>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceModule<double, 3>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceModule<double, 4>;
template class MY_NEURAL_NETWORK_LIB__NATIVES__API EuclidianDistanceModule<double, 5>;
