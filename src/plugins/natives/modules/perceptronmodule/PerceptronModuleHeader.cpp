#include <plugins/natives/modules/perceptronmodule/PerceptronModuleHeader.h>

#include <plugincore/ResourcesContainer.h>

#include <plugins/natives/modules/perceptronmodule/PerceptronModule.h>

#include <rtti/RttiHolder.impl.h>
#include <rtti/TemplateRtti.h>

#include <tensor/TensorIndex.h>

using namespace BuildTimeList;

template <typename ScalarType>
struct TemplateProvider
{
    template <size_t InputDimension, size_t OutputDimension>
    using PerceptronModuleTemplate = PerceptronModule<ScalarType, InputDimension, OutputDimension>;
};

template <typename ScalarType, size_t InputDimension, size_t OutputDimension>
using PerceptronModuleRtti = Rtti<
    Module<ScalarType>,
    PerceptronModule<ScalarType, InputDimension, OutputDimension>,
    TypeList<const RawTuple<const TensorIndex<InputDimension + OutputDimension>&, const TensorIndex<OutputDimension>&>&>
>;

void
PerceptronModuleHeader::load(ResourcesContainerToken& token)
{
    RttiHolderToken<Module<float>>& moduleFloatRttiHolderToken = token.getRttiHolderToken<Module<float>>();
    RttiHolderToken<Module<double>>& moduleDoubleRttiHolderToken = token.getRttiHolderToken<Module<double>>();
    
    TemplateRtti<Module<float>, TemplateProvider<float>::PerceptronModuleTemplate>& perceptronModuleFloatTemplateRtti = moduleFloatRttiHolderToken.getOrSubscribeTemplateRtti<TemplateProvider<float>::PerceptronModuleTemplate>("PerceptronModule<float>");
    perceptronModuleFloatTemplateRtti.subscribe<size_t{0}, size_t{0}>(new PerceptronModuleRtti<float, 0, 0>("PerceptronModule<float,0,0>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{1}, size_t{0}>(new PerceptronModuleRtti<float, 1, 0>("PerceptronModule<float,1,0>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{2}, size_t{0}>(new PerceptronModuleRtti<float, 2, 0>("PerceptronModule<float,2,0>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{3}, size_t{0}>(new PerceptronModuleRtti<float, 3, 0>("PerceptronModule<float,3,0>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{0}, size_t{1}>(new PerceptronModuleRtti<float, 0, 1>("PerceptronModule<float,0,1>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{1}, size_t{1}>(new PerceptronModuleRtti<float, 1, 1>("PerceptronModule<float,1,1>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{2}, size_t{1}>(new PerceptronModuleRtti<float, 2, 1>("PerceptronModule<float,2,1>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{3}, size_t{1}>(new PerceptronModuleRtti<float, 3, 1>("PerceptronModule<float,3,1>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{0}, size_t{2}>(new PerceptronModuleRtti<float, 0, 2>("PerceptronModule<float,0,2>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{1}, size_t{2}>(new PerceptronModuleRtti<float, 1, 2>("PerceptronModule<float,1,2>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{2}, size_t{2}>(new PerceptronModuleRtti<float, 2, 2>("PerceptronModule<float,2,2>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{3}, size_t{2}>(new PerceptronModuleRtti<float, 3, 2>("PerceptronModule<float,3,2>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{0}, size_t{3}>(new PerceptronModuleRtti<float, 0, 3>("PerceptronModule<float,0,3>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{1}, size_t{3}>(new PerceptronModuleRtti<float, 1, 3>("PerceptronModule<float,1,3>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{2}, size_t{3}>(new PerceptronModuleRtti<float, 2, 3>("PerceptronModule<float,2,3>"));
    perceptronModuleFloatTemplateRtti.subscribe<size_t{3}, size_t{3}>(new PerceptronModuleRtti<float, 3, 3>("PerceptronModule<float,3,3>"));

    TemplateRtti<Module<double>, TemplateProvider<double>::PerceptronModuleTemplate>& perceptronModuleDoubleTemplateRtti = moduleDoubleRttiHolderToken.getOrSubscribeTemplateRtti<TemplateProvider<double>::PerceptronModuleTemplate>("PerceptronModule<double>");
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{0}, size_t{0}>(new PerceptronModuleRtti<double, 0, 0>("PerceptronModule<double,0,0>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{1}, size_t{0}>(new PerceptronModuleRtti<double, 1, 0>("PerceptronModule<double,1,0>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{2}, size_t{0}>(new PerceptronModuleRtti<double, 2, 0>("PerceptronModule<double,2,0>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{3}, size_t{0}>(new PerceptronModuleRtti<double, 3, 0>("PerceptronModule<double,3,0>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{0}, size_t{1}>(new PerceptronModuleRtti<double, 0, 1>("PerceptronModule<double,0,1>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{1}, size_t{1}>(new PerceptronModuleRtti<double, 1, 1>("PerceptronModule<double,1,1>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{2}, size_t{1}>(new PerceptronModuleRtti<double, 2, 1>("PerceptronModule<double,2,1>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{3}, size_t{1}>(new PerceptronModuleRtti<double, 3, 1>("PerceptronModule<double,3,1>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{0}, size_t{2}>(new PerceptronModuleRtti<double, 0, 2>("PerceptronModule<double,0,2>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{1}, size_t{2}>(new PerceptronModuleRtti<double, 1, 2>("PerceptronModule<double,1,2>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{2}, size_t{2}>(new PerceptronModuleRtti<double, 2, 2>("PerceptronModule<double,2,2>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{3}, size_t{2}>(new PerceptronModuleRtti<double, 3, 2>("PerceptronModule<double,3,2>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{0}, size_t{3}>(new PerceptronModuleRtti<double, 0, 3>("PerceptronModule<double,0,3>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{1}, size_t{3}>(new PerceptronModuleRtti<double, 1, 3>("PerceptronModule<double,1,3>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{2}, size_t{3}>(new PerceptronModuleRtti<double, 2, 3>("PerceptronModule<double,2,3>"));
    perceptronModuleDoubleTemplateRtti.subscribe<size_t{3}, size_t{3}>(new PerceptronModuleRtti<double, 3, 3>("PerceptronModule<double,3,3>"));
}
