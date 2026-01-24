#include <plugincore/PluginHeader.h>

#include <plugins/nativemodules/api.h>

PLUGINCORE__INCLUDE_PLUGIN_HEADER(MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API)

static ResourcesContainerToken* token = nullptr;

void
openNeuralNetworkPlugin(ResourcesContainer* resourcesContainer)
{
    token = new ResourcesContainerToken(resourcesContainer->edit());
}

void
closeNeuralNetworkPlugin()
{
    delete token;
}
