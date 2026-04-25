#pragma once

#ifdef _WIN32
  #ifdef MY_NEURAL_NETWORK_LIB__TENSOR
    #define MY_NEURAL_NETWORK_LIB__TENSOR__API __declspec(dllexport)
  #else
    #define MY_NEURAL_NETWORK_LIB__TENSOR__API __declspec(dllimport)
  #endif
#else
  #define MY_NEURAL_NETWORK_LIB__TENSOR__API
#endif
