#pragma once

#include <plugincore/ResourcesContainer.h>

#define PLUGINCORE__INCLUDE_PLUGIN_HEADER(DECLSPEC)                                    \
    extern "C"                                                                         \
    {                                                                                  \
        DECLSPEC void openNeuralNetworkPlugin(ResourcesContainer* resourcesContainer); \
                                                                                       \
        DECLSPEC void closeNeuralNetworkPlugin();                                      \
    }
