#pragma once

#include <module/Module.h>
#include <network/AbstractNetworkBuilder.h>
#include <tensor/TensorMap.h>
#include <tensor/AbstractTensorMapKeyEnum.h>

#define __RESOURCES_CONTAINER__RESOURCES_TYPES \
    Module<float>,                             \
    Module<double>,                            \
    AbstractNetworkBuilder<float>,             \
    AbstractNetworkBuilder<double>,            \
    AbstractTensorMap<float>,                  \
    AbstractTensorMap<double>,                 \
    AbstractTensorMapKeyEnum
