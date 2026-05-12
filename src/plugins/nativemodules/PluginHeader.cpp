#include <plugincore/PluginHeader.h>

#include <plugins/nativemodules/api.h>
#include <plugins/nativemodules/AdditionModule.h>
#include <plugins/nativemodules/PerceptronModule.h>

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

    token->modules<float>().subscribe(new Rtti<Module<float>, PerceptronModule<float, 1, 1>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<2>&, const TensorIndex<1>&>&>>("PerceptronModule<float,1,1>"));
    token->modules<float>().subscribe(new Rtti<Module<float>, PerceptronModule<float, 2, 1>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<3>&, const TensorIndex<1>&>&>>("PerceptronModule<float,2,1>"));
    token->modules<float>().subscribe(new Rtti<Module<float>, PerceptronModule<float, 3, 1>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<4>&, const TensorIndex<1>&>&>>("PerceptronModule<float,3,1>"));
    token->modules<float>().subscribe(new Rtti<Module<float>, PerceptronModule<float, 1, 2>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<3>&, const TensorIndex<2>&>&>>("PerceptronModule<float,1,2>"));
    token->modules<float>().subscribe(new Rtti<Module<float>, PerceptronModule<float, 2, 2>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<4>&, const TensorIndex<2>&>&>>("PerceptronModule<float,2,2>"));
    token->modules<float>().subscribe(new Rtti<Module<float>, PerceptronModule<float, 3, 2>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<5>&, const TensorIndex<2>&>&>>("PerceptronModule<float,3,2>"));
    token->modules<float>().subscribe(new Rtti<Module<float>, PerceptronModule<float, 1, 3>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<4>&, const TensorIndex<3>&>&>>("PerceptronModule<float,1,3>"));
    token->modules<float>().subscribe(new Rtti<Module<float>, PerceptronModule<float, 2, 3>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<5>&, const TensorIndex<3>&>&>>("PerceptronModule<float,2,3>"));
    token->modules<float>().subscribe(new Rtti<Module<float>, PerceptronModule<float, 3, 3>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<6>&, const TensorIndex<3>&>&>>("PerceptronModule<float,3,3>"));
    token->modules<double>().subscribe(new Rtti<Module<double>, PerceptronModule<double, 1, 1>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<2>&, const TensorIndex<1>&>&>>("PerceptronModule<double,1,1>"));
    token->modules<double>().subscribe(new Rtti<Module<double>, PerceptronModule<double, 2, 1>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<3>&, const TensorIndex<1>&>&>>("PerceptronModule<double,2,1>"));
    token->modules<double>().subscribe(new Rtti<Module<double>, PerceptronModule<double, 3, 1>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<4>&, const TensorIndex<1>&>&>>("PerceptronModule<double,3,1>"));
    token->modules<double>().subscribe(new Rtti<Module<double>, PerceptronModule<double, 1, 2>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<3>&, const TensorIndex<2>&>&>>("PerceptronModule<double,1,2>"));
    token->modules<double>().subscribe(new Rtti<Module<double>, PerceptronModule<double, 2, 2>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<4>&, const TensorIndex<2>&>&>>("PerceptronModule<double,2,2>"));
    token->modules<double>().subscribe(new Rtti<Module<double>, PerceptronModule<double, 3, 2>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<5>&, const TensorIndex<2>&>&>>("PerceptronModule<double,3,2>"));
    token->modules<double>().subscribe(new Rtti<Module<double>, PerceptronModule<double, 1, 3>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<4>&, const TensorIndex<3>&>&>>("PerceptronModule<double,1,3>"));
    token->modules<double>().subscribe(new Rtti<Module<double>, PerceptronModule<double, 2, 3>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<5>&, const TensorIndex<3>&>&>>("PerceptronModule<double,2,3>"));
    token->modules<double>().subscribe(new Rtti<Module<double>, PerceptronModule<double, 3, 3>, BuildTimeList::TypeList<const RawTuple<const TensorIndex<6>&, const TensorIndex<3>&>&>>("PerceptronModule<double,3,3>"));
}

void
closeNeuralNetworkPlugin()
{
    delete token;
}
