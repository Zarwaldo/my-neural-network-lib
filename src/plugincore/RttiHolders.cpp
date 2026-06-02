#include <plugincore/api.h>

#include <rtti/RttiHolder.impl.h>

#include <module/Module.h>

#include <tensor/TensorMap.h>
#include <tensor/AbstractTensorMapKeyEnum.h>

#include <CommonMacros.h>

template class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolderToken<Module<float>>;
template class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolderToken<Module<double>>;
template class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolderToken<AbstractTensorMap<float>>;
template class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolderToken<AbstractTensorMap<double>>;
template class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolderToken<AbstractTensorMapKeyEnum>;
template class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolder<Module<float>>;
template class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolder<Module<double>>;
template class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolder<AbstractTensorMap<float>>;
template class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolder<AbstractTensorMap<double>>;
template class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolder<AbstractTensorMapKeyEnum>;
