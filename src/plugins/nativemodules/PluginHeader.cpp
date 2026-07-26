#include <plugincore/PluginHeader.h>

#include <plugins/nativemodules/api.h>
#include <plugins/nativemodules/AdditionModule.h>
#include <plugins/nativemodules/PerceptronModule.h>
#include <plugins/nativemodules/TensorMapKeyEnums.h>

#include <rtti/RttiHolder.impl.h>
#include <rtti/TemplateRtti.h>
#include <rtti/TypenameArgId.h>

#include <tensor/TensorIndex.h>
#include <tensor/TensorMap.h>

using namespace BuildTimeList;

PLUGINCORE__INCLUDE_PLUGIN_HEADER(MY_NEURAL_NETWORK_LIB__NATIVE_MODULES__API)

template <typename ValueType>
struct TemplateProvider
{
    template <size_t Dimension>
    using AdditionModuleTemplate = AdditionModule<ValueType, Dimension>;

    template <size_t InputDimension, size_t OutputDimension>
    using PerceptronModuleTemplate = PerceptronModule<ValueType, InputDimension, OutputDimension>;

    template <TypenameArgId KeyEnumId>
    using TensorMapTemplate = TensorMap<ValueType, TypeFromTypenameArgId<KeyEnumId>>;
};

template <typename ValueType, size_t Dimension>
using AdditionModuleRtti = Rtti<
    Module<ValueType>,
    AdditionModule<ValueType, Dimension>,
    TypeList<const RawTuple<const TensorIndex<Dimension>&>&>
>;

template <typename ValueType, size_t InputDimension, size_t OutputDimension>
using PerceptronModuleRtti = Rtti<
    Module<ValueType>,
    PerceptronModule<ValueType, InputDimension, OutputDimension>,
    TypeList<const RawTuple<const TensorIndex<InputDimension + OutputDimension>&, const TensorIndex<OutputDimension>&>&>
>;

template <typename ValueType, typename KeyEnum>
using TensorMapRtti = Rtti<
    AbstractTensorMap<ValueType>,
    TensorMap<ValueType, KeyEnum>,
    TypeList<AbstractTensor<ValueType>* const *>,
    TypeList<const std::initializer_list<AbstractTensor<ValueType>*>&>
>;

static ResourcesContainerToken* token = nullptr;

void
openNeuralNetworkPlugin(ResourcesContainer* resourcesContainer)
{
    token = new ResourcesContainerToken(resourcesContainer->edit());

    RttiHolderToken<Module<float>>& moduleFloatRttiHolderToken = token->getRttiHolderToken<Module<float>>();
    RttiHolderToken<Module<double>>& moduleDoubleRttiHolderToken = token->getRttiHolderToken<Module<double>>();

    TemplateRtti<Module<float>, TemplateProvider<float>::AdditionModuleTemplate>& additionModuleFloatTemplateRtti = moduleFloatRttiHolderToken.getOrSubscribeTemplateRtti<TemplateProvider<float>::AdditionModuleTemplate>("AdditionModule<float>");
    additionModuleFloatTemplateRtti.subscribe<size_t{1}>(new AdditionModuleRtti<float, 1>("AdditionModule<float,1>"));
    additionModuleFloatTemplateRtti.subscribe<size_t{2}>(new AdditionModuleRtti<float, 2>("AdditionModule<float,2>"));
    additionModuleFloatTemplateRtti.subscribe<size_t{3}>(new AdditionModuleRtti<float, 3>("AdditionModule<float,3>"));
    additionModuleFloatTemplateRtti.subscribe<size_t{4}>(new AdditionModuleRtti<float, 4>("AdditionModule<float,4>"));
    additionModuleFloatTemplateRtti.subscribe<size_t{5}>(new AdditionModuleRtti<float, 5>("AdditionModule<float,5>"));

    TemplateRtti<Module<double>, TemplateProvider<double>::AdditionModuleTemplate>& additionModuleDoubleTemplateRtti = moduleDoubleRttiHolderToken.getOrSubscribeTemplateRtti<TemplateProvider<double>::AdditionModuleTemplate>("AdditionModule<double>");
    additionModuleDoubleTemplateRtti.subscribe<size_t{1}>(new AdditionModuleRtti<double, 1>("AdditionModule<double,1>"));
    additionModuleDoubleTemplateRtti.subscribe<size_t{2}>(new AdditionModuleRtti<double, 2>("AdditionModule<double,2>"));
    additionModuleDoubleTemplateRtti.subscribe<size_t{3}>(new AdditionModuleRtti<double, 3>("AdditionModule<double,3>"));
    additionModuleDoubleTemplateRtti.subscribe<size_t{4}>(new AdditionModuleRtti<double, 4>("AdditionModule<double,4>"));
    additionModuleDoubleTemplateRtti.subscribe<size_t{5}>(new AdditionModuleRtti<double, 5>("AdditionModule<double,5>"));

    TemplateRtti<Module<float>, TemplateProvider<float>::PerceptronModuleTemplate>& perceptronModuleFloatTemplateRtti = moduleFloatRttiHolderToken.getOrSubscribeTemplateRtti<TemplateProvider<float>::PerceptronModuleTemplate>("PerceptronModule<float>");
    perceptronModuleFloatTemplateRtti.subscribe<size_t{1}, size_t{1}>(new PerceptronModuleRtti<float, 1, 1>("PerceptronModule<float,1,1>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{1}, size_t{2}>(new PerceptronModuleRtti<float, 1, 2>("PerceptronModule<float,1,2>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{1}, size_t{3}>(new PerceptronModuleRtti<float, 1, 3>("PerceptronModule<float,1,3>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{2}, size_t{1}>(new PerceptronModuleRtti<float, 2, 1>("PerceptronModule<float,2,1>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{2}, size_t{2}>(new PerceptronModuleRtti<float, 2, 2>("PerceptronModule<float,2,2>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{2}, size_t{3}>(new PerceptronModuleRtti<float, 2, 3>("PerceptronModule<float,2,3>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{3}, size_t{1}>(new PerceptronModuleRtti<float, 3, 1>("PerceptronModule<float,3,1>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{3}, size_t{2}>(new PerceptronModuleRtti<float, 3, 2>("PerceptronModule<float,3,2>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{3}, size_t{3}>(new PerceptronModuleRtti<float, 3, 3>("PerceptronModule<float,3,3>"));

    TemplateRtti<Module<double>, TemplateProvider<double>::PerceptronModuleTemplate>& perceptronModuleDoubleTemplateRtti = moduleDoubleRttiHolderToken.getOrSubscribeTemplateRtti<TemplateProvider<double>::PerceptronModuleTemplate>("PerceptronModule<double>");
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{1}, size_t{1}>(new PerceptronModuleRtti<double, 1, 1>("PerceptronModule<double,1,1>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{1}, size_t{2}>(new PerceptronModuleRtti<double, 1, 2>("PerceptronModule<double,1,2>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{1}, size_t{3}>(new PerceptronModuleRtti<double, 1, 3>("PerceptronModule<double,1,3>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{2}, size_t{1}>(new PerceptronModuleRtti<double, 2, 1>("PerceptronModule<double,2,1>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{2}, size_t{2}>(new PerceptronModuleRtti<double, 2, 2>("PerceptronModule<double,2,2>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{2}, size_t{3}>(new PerceptronModuleRtti<double, 2, 3>("PerceptronModule<double,2,3>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{3}, size_t{1}>(new PerceptronModuleRtti<double, 3, 1>("PerceptronModule<double,3,1>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{3}, size_t{2}>(new PerceptronModuleRtti<double, 3, 2>("PerceptronModule<double,3,2>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{3}, size_t{3}>(new PerceptronModuleRtti<double, 3, 3>("PerceptronModule<double,3,3>"));

    RttiHolderToken<AbstractTensorMap<float>>& tensorMapFloatRttiHolderToken = token->getRttiHolderToken<AbstractTensorMap<float>>();
    RttiHolderToken<AbstractTensorMap<double>>& tensorMapDoubleRttiHolderToken = token->getRttiHolderToken<AbstractTensorMap<double>>();

    token->getRttiHolderToken<AbstractTensorMapKeyEnum>().subscribe(
        new Rtti<
            AbstractTensorMapKeyEnum,
            TensorSingleton,
            TypeList<size_t>
        >("TensorSingleton")
    );
    token->getRttiHolderToken<AbstractTensorMapKeyEnum>().subscribe(
        new Rtti<
            AbstractTensorMapKeyEnum,
            PerceptronParamsKeyEnum,
            TypeList<size_t>
        >("PerceptronParamsKeyEnum")
    );

    TemplateRtti<AbstractTensorMap<float>, TemplateProvider<float>::TensorMapTemplate>& tensorMapFloatTemplateRtti = tensorMapFloatRttiHolderToken.getOrSubscribeTemplateRtti<TemplateProvider<float>::TensorMapTemplate>("TensorMap<float>");
    tensorMapFloatTemplateRtti.subscribe<TypenameArgIdOf<TensorSingleton>>(new TensorMapRtti<float, TensorSingleton>("TensorMap<float,TensorSingleton>"));
    tensorMapFloatTemplateRtti.subscribe<TypenameArgIdOf<PerceptronParamsKeyEnum>>(new TensorMapRtti<float, PerceptronParamsKeyEnum>("TensorMap<float,PerceptronParamsKeyEnum>"));

    TemplateRtti<AbstractTensorMap<double>, TemplateProvider<double>::TensorMapTemplate>& tensorMapDoubleTemplateRtti = tensorMapDoubleRttiHolderToken.getOrSubscribeTemplateRtti<TemplateProvider<double>::TensorMapTemplate>("TensorMap<double>");
    tensorMapDoubleTemplateRtti.subscribe<TypenameArgIdOf<TensorSingleton>>(new TensorMapRtti<double, TensorSingleton>("TensorMap<double,TensorSingleton>"));
    tensorMapDoubleTemplateRtti.subscribe<TypenameArgIdOf<PerceptronParamsKeyEnum>>(new TensorMapRtti<double, PerceptronParamsKeyEnum>("TensorMap<double,PerceptronParamsKeyEnum>"));
}

void
closeNeuralNetworkPlugin()
{
    delete token;
}
