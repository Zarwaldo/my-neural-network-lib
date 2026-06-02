#pragma once

#ifdef _WIN32
  #ifdef MY_NEURAL_NETWORK_LIB__NETWORK
    #ifndef __CUDACC__
      #define MY_NEURAL_NETWORK_LIB__NETWORK__API __declspec(dllexport)
      #define MY_NEURAL_NETWORK_LIB__NETWORK__CUDA_API
    #else
      #define MY_NEURAL_NETWORK_LIB__NETWORK__API
      #define MY_NEURAL_NETWORK_LIB__NETWORK__CUDA_API __declspec(dllexport)
    #endif
  #else
    #define MY_NEURAL_NETWORK_LIB__NETWORK__API __declspec(dllimport)
    #define MY_NEURAL_NETWORK_LIB__NETWORK__CUDA_API __declspec(dllimport)
  #endif
#else
  #define MY_NEURAL_NETWORK_LIB__NETWORK__API
  #define MY_NEURAL_NETWORK_LIB__NETWORK__CUDA_API
#endif
