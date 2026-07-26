#pragma once

#include <module/Module.h>

#include <network/AbstractNetworkBuilder.h>

#include <plugincore/api.h>
#include <plugincore/ResourcesContainerImpl.h>

#include <tensor/AbstractTensorMapKeyEnum.h>

#define __RESOURCES_CONTAINER__SUPPORTED_TYPES \
    Module<float>,                             \
    Module<double>,                            \
    AbstractNetworkBuilder<float>,             \
    AbstractNetworkBuilder<double>,            \
    AbstractTensorMap<float>,                  \
    AbstractTensorMap<double>,                 \
    AbstractTensorMapKeyEnum

using ResourcesContainerToken = ResourcesContainerTokenImpl<__RESOURCES_CONTAINER__SUPPORTED_TYPES>;

class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API ResourcesContainer : public ResourcesContainerImpl<__RESOURCES_CONTAINER__SUPPORTED_TYPES>
{};
