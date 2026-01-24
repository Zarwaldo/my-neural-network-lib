#pragma once

#ifdef _WIN32
  #ifdef MY_NEURAL_NETWORK_LIB__PLUGINLOADER
    #define MY_NEURAL_NETWORK_LIB__PLUGINLOADER__API __declspec(dllexport)
  #else
    #define MY_NEURAL_NETWORK_LIB__PLUGINLOADER__API __declspec(dllimport)
  #endif
#else
  #define MY_NEURAL_NETWORK_LIB__PLUGINLOADER__API
#endif
