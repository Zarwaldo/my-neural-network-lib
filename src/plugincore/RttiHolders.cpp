#include <plugincore/api.h>

#include <rtti/RttiHolder.impl.h>

#include <module/Module.h>

#include <plugincore/ResourcesTypes.h>

#include <tensor/TensorMap.h>
#include <tensor/AbstractTensorMapKeyEnum.h>

#include <CommonMacros.h>

#define __RTTI_HOLDERS__RTTI_HOLDER_TOKEN__EXPLICIT_INSTANCIATION(Type, context) \
    template class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolderToken<Type>;
FOR_EACH_SEP(__RTTI_HOLDERS__RTTI_HOLDER_TOKEN__EXPLICIT_INSTANCIATION, FORWARD(), , __RESOURCES_CONTAINER__RESOURCES_TYPES)

#define __RTTI_HOLDERS__RTTI_HOLDER__EXPLICIT_INSTANCIATION(Type, context) \
    template class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolder<Type>;
FOR_EACH_SEP(__RTTI_HOLDERS__RTTI_HOLDER__EXPLICIT_INSTANCIATION, FORWARD(), , __RESOURCES_CONTAINER__RESOURCES_TYPES)
