#pragma once

#ifdef _WIN32
  #ifdef MY_NEURAL_NETWORK_LIB
    #define MY_NEURAL_NETWORK_LIB__API __declspec(dllexport)
  #else
    #define MY_NEURAL_NETWORK_LIB__API __declspec(dllimport)
  #endif
#else
  #define MY_NEURAL_NETWORK_LIB__API
#endif
