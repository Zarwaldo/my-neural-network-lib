#include <plugincore/ResourcesContainer.h>

#include <plugincore/ResourcesContainerImpl.impl.h>

#include <helpers/ForEachMacro.h>

#define __RESOURCES_CONTAINER_TOKEN_IMPL__GET_RTTI_HOLDER_TOKEN__EXPLICIT_INSTANCIATION(Type, context)                                                                      \
    template MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolderToken<Type>& ResourcesContainerTokenImpl<__RESOURCES_CONTAINER__RESOURCES_TYPES>::getRttiHolderToken<Type>();
FOR_EACH_SEP(__RESOURCES_CONTAINER_TOKEN_IMPL__GET_RTTI_HOLDER_TOKEN__EXPLICIT_INSTANCIATION, FORWARD(), , __RESOURCES_CONTAINER__RESOURCES_TYPES)

#define __RESOURCES_CONTAINER_IMPL__GET_RTTI_HOLDER__EXPLICIT_INSTANCIATION(Type, context)                                                                               \
    template MY_NEURAL_NETWORK_LIB__PLUGINCORE__API const RttiHolder<Type>& ResourcesContainerImpl<__RESOURCES_CONTAINER__RESOURCES_TYPES>::getRttiHolder<Type>() const;
FOR_EACH_SEP(__RESOURCES_CONTAINER_IMPL__GET_RTTI_HOLDER__EXPLICIT_INSTANCIATION, FORWARD(), , __RESOURCES_CONTAINER__RESOURCES_TYPES)
