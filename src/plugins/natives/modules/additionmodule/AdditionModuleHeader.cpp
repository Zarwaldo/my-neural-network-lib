#include <plugins/natives/modules/additionmodule/AdditionModuleHeader.h>

#include <plugincore/ResourcesContainer.h>

#include <plugins/natives/modules/additionmodule/AdditionModule.h>

#include <rtti/RttiHolder.impl.h>
#include <rtti/TemplateRtti.h>

#include <tensor/TensorIndex.h>

using namespace BuildTimeList;

template <typename ValueType>
struct TemplateProvider
{
    template <size_t Dimension>
    using AdditionModuleTemplate = AdditionModule<ValueType, Dimension>;
};

template <typename ValueType, size_t Dimension>
using AdditionModuleRtti = Rtti<
    Module<ValueType>,
    AdditionModule<ValueType, Dimension>,
    TypeList<const RawTuple<const TensorIndex<Dimension>&>&>
>;

void
AdditionModuleHeader::load(ResourcesContainerToken& token)
{
    RttiHolderToken<Module<float>>& moduleFloatRttiHolderToken = token.getRttiHolderToken<Module<float>>();
    RttiHolderToken<Module<double>>& moduleDoubleRttiHolderToken = token.getRttiHolderToken<Module<double>>();

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
}
