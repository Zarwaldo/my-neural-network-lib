#include <plugincore/PluginHeader.h>

#include <plugins/nativemodules/api.h>
#include <plugins/nativemodules/AdditionModule.h>

#include <tensor/TensorIndex.h>

PLUGINCORE__INCLUDE_PLUGIN_HEADER(MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API)

static ResourcesContainerToken* token = nullptr;

void
openNeuralNetworkPlugin(ResourcesContainer* resourcesContainer)
{
    token = new ResourcesContainerToken(resourcesContainer->edit());

    token->modules<float>().subscribe(new Rtti<Module<float>, AdditionModule<float, 1>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<1>&>&>>("AdditionModule<float,1>"));
    token->modules<float>().subscribe(new Rtti<Module<float>, AdditionModule<float, 2>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<2>&>&>>("AdditionModule<float,2>"));
    token->modules<float>().subscribe(new Rtti<Module<float>, AdditionModule<float, 3>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<3>&>&>>("AdditionModule<float,3>"));
    token->modules<float>().subscribe(new Rtti<Module<float>, AdditionModule<float, 4>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<4>&>&>>("AdditionModule<float,4>"));
    token->modules<float>().subscribe(new Rtti<Module<float>, AdditionModule<float, 5>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<5>&>&>>("AdditionModule<float,5>"));
    token->modules<double>().subscribe(new Rtti<Module<double>, AdditionModule<double, 1>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<1>&>&>>("AdditionModule<double,1>"));
    token->modules<double>().subscribe(new Rtti<Module<double>, AdditionModule<double, 2>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<2>&>&>>("AdditionModule<double,2>"));
    token->modules<double>().subscribe(new Rtti<Module<double>, AdditionModule<double, 3>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<3>&>&>>("AdditionModule<double,3>"));
    token->modules<double>().subscribe(new Rtti<Module<double>, AdditionModule<double, 4>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<4>&>&>>("AdditionModule<double,4>"));
    token->modules<double>().subscribe(new Rtti<Module<double>, AdditionModule<double, 5>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<5>&>&>>("AdditionModule<double,5>"));
}

void
closeNeuralNetworkPlugin()
{
    delete token;
}
