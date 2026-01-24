#pragma once

#ifdef _WIN32
  #ifdef MY_NEURAL_NETWORK_LIB__NATIVE_MODULES
    #define MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API __declspec(dllexport)
  #else
    #define MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API __declspec(dllimport)
  #endif
#else
  #define MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API
#endif
