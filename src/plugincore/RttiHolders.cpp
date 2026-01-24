#include <plugincore/api.h>

#include <rtti/RttiHolder.impl.h>

#include <module/Module.h>

#include <CommonMacros.h>

template class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolderToken<Module<float>>;
template class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolderToken<Module<double>>;
template class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolder<Module<float>>;
template class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolder<Module<double>>;
