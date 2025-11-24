#pragma once

#include <tensor/TensorMapKeyEnum.h>

#include <module/api.h>

#define TENSOR_MAP_KEY_ENUM__VALUES_OF_TensorSingleton SINGLE_TENSOR
DEFINE_TENSOR_MAP_KEY_ENUM(TensorSingleton, MY_NEURAL_NETWORK_LIB__MODULE__API)

#define TENSOR_MAP_KEY_ENUM__VALUES_OF_PerceptronParamsKeyEnum WEIGHTS, BIASES
DEFINE_TENSOR_MAP_KEY_ENUM(PerceptronParamsKeyEnum, MY_NEURAL_NETWORK_LIB__MODULE__API)
