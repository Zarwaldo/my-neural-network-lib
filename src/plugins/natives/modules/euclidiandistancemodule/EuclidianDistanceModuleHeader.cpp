#include <plugins/natives/modules/euclidiandistancemodule/EuclidianDistanceModuleHeader.h>

#include <plugincore/ResourcesContainer.h>

#include <plugins/natives/modules/euclidiandistancemodule/EuclidianDistanceModule.h>

#include <rtti/RttiHolder.impl.h>
#include <rtti/TemplateRtti.h>

#include <tensor/TensorIndex.h>

using namespace BuildTimeList;

template <typename ScalarType>
struct TemplateProvider
{
    template <size_t Dimension>
    using EuclidianDistanceModuleTemplate = EuclidianDistanceModule<ScalarType, Dimension>;
};

template <typename ScalarType, size_t Dimension>
using EuclidianDistanceModuleRtti = Rtti<
    Module<ScalarType>,
    EuclidianDistanceModule<ScalarType, Dimension>,
    TypeList<const RawTuple<const TensorIndex<0>&>&>
>;

void
EuclidianDistanceModuleHeader::load(ResourcesContainerToken& token)
{
    RttiHolderToken<Module<float>>& moduleFloatRttiHolderToken = token.getRttiHolderToken<Module<float>>();
    RttiHolderToken<Module<double>>& moduleDoubleRttiHolderToken = token.getRttiHolderToken<Module<double>>();

    TemplateRtti<Module<float>, TemplateProvider<float>::EuclidianDistanceModuleTemplate>& euclidianDistanceModuleFloatTemplateRtti = moduleFloatRttiHolderToken.getOrSubscribeTemplateRtti<TemplateProvider<float>::EuclidianDistanceModuleTemplate>("EuclidianDistanceModule<float>");
    euclidianDistanceModuleFloatTemplateRtti.subscribe<size_t{0}>(new EuclidianDistanceModuleRtti<float, 0>("EuclidianDistanceModule<float,0>"));
    euclidianDistanceModuleFloatTemplateRtti.subscribe<size_t{1}>(new EuclidianDistanceModuleRtti<float, 1>("EuclidianDistanceModule<float,1>"));
    euclidianDistanceModuleFloatTemplateRtti.subscribe<size_t{2}>(new EuclidianDistanceModuleRtti<float, 2>("EuclidianDistanceModule<float,2>"));
    euclidianDistanceModuleFloatTemplateRtti.subscribe<size_t{3}>(new EuclidianDistanceModuleRtti<float, 3>("EuclidianDistanceModule<float,3>"));
    euclidianDistanceModuleFloatTemplateRtti.subscribe<size_t{4}>(new EuclidianDistanceModuleRtti<float, 4>("EuclidianDistanceModule<float,4>"));
    euclidianDistanceModuleFloatTemplateRtti.subscribe<size_t{5}>(new EuclidianDistanceModuleRtti<float, 5>("EuclidianDistanceModule<float,5>"));

    TemplateRtti<Module<double>, TemplateProvider<double>::EuclidianDistanceModuleTemplate>& euclidianDistanceModuleDoubleTemplateRtti = moduleDoubleRttiHolderToken.getOrSubscribeTemplateRtti<TemplateProvider<double>::EuclidianDistanceModuleTemplate>("EuclidianDistanceModule<double>");
    euclidianDistanceModuleDoubleTemplateRtti.subscribe<size_t{0}>(new EuclidianDistanceModuleRtti<double, 0>("EuclidianDistanceModule<double,0>"));
    euclidianDistanceModuleDoubleTemplateRtti.subscribe<size_t{1}>(new EuclidianDistanceModuleRtti<double, 1>("EuclidianDistanceModule<double,1>"));
    euclidianDistanceModuleDoubleTemplateRtti.subscribe<size_t{2}>(new EuclidianDistanceModuleRtti<double, 2>("EuclidianDistanceModule<double,2>"));
    euclidianDistanceModuleDoubleTemplateRtti.subscribe<size_t{3}>(new EuclidianDistanceModuleRtti<double, 3>("EuclidianDistanceModule<double,3>"));
    euclidianDistanceModuleDoubleTemplateRtti.subscribe<size_t{4}>(new EuclidianDistanceModuleRtti<double, 4>("EuclidianDistanceModule<double,4>"));
    euclidianDistanceModuleDoubleTemplateRtti.subscribe<size_t{5}>(new EuclidianDistanceModuleRtti<double, 5>("EuclidianDistanceModule<double,5>"));
}
