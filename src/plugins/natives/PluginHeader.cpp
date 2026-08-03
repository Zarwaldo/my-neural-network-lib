#include <plugincore/PluginHeader.h>

#include <plugins/natives/api.h>
#include <plugins/natives/modules/additionmodule/AdditionModuleHeader.h>
#include <plugins/natives/modules/euclidiandistancemodule/EuclidianDistanceModuleHeader.h>
#include <plugins/natives/modules/perceptronmodule/PerceptronModuleHeader.h>
#include <plugins/natives/tensormaps/TensorMapsHeader.h>

PLUGINCORE__INCLUDE_PLUGIN_HEADER(MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API)

static ResourcesContainerToken* token = nullptr;

void
openNeuralNetworkPlugin(ResourcesContainer* resourcesContainer)
{
    token = new ResourcesContainerToken(resourcesContainer->edit());

    AdditionModuleHeader::load(*token);
    EuclidianDistanceModuleHeader::load(*token);
    PerceptronModuleHeader::load(*token);

    TensorMapsHeader::load(*token);
}

void
closeNeuralNetworkPlugin()
{
    delete token;
}
