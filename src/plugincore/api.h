#pragma once

#ifdef _WIN32
  #ifdef MY_NEURAL_NETWORK_LIB__PLUGINCORE
    #define MY_NEURAL_NETWORK_LIB__PLUGINCORE__API __declspec(dllexport)
  #else
    #define MY_NEURAL_NETWORK_LIB__PLUGINCORE__API __declspec(dllimport)
  #endif
#else
  #define MY_NEURAL_NETWORK_LIB__PLUGINCORE__API
#endif
