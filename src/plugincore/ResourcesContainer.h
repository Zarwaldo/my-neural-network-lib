#pragma once

#include <module/Module.h>

#include <network/AbstractNetworkBuilder.h>

#include <plugincore/api.h>
#include <plugincore/ResourcesContainerImpl.h>
#include <plugincore/ResourcesTypes.h>

#include <tensor/AbstractTensorMapKeyEnum.h>

using ResourcesContainerToken = ResourcesContainerTokenImpl<__RESOURCES_CONTAINER__RESOURCES_TYPES>;

class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API ResourcesContainer : public ResourcesContainerImpl<__RESOURCES_CONTAINER__RESOURCES_TYPES>
{};
