#pragma once

#ifdef __CUDACC__
  #define HOST __host__
  #define DEVICE __device__
  #define GLOBAL __global__
#else
  #define HOST
  #define DEVICE
  #define GLOBAL
#endif
