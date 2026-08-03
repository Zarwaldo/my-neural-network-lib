#include <plugins/natives/networkbuilders/NetworkBuildersHeader.h>

#include <helpers/BuildTimeList.h>

#include <network/AbstractNetworkBuilder.h>

#include <plugins/natives/networkbuilders/EuclidianDistanceCostBuilder.h>

#include <rtti/RttiHolder.impl.h>
#include <rtti/TemplateRtti.h>

using namespace BuildTimeList;

template <typename ValueType>
struct TemplateProvider
{
    template <size_t Dimension>
    using EuclidianDistanceCostBuilderTemplate = EuclidianDistanceCostBuilder<ValueType, Dimension>;
};

template <typename ValueType, size_t Dimension>
using EuclidianDistanceCostBuilderRtti = Rtti<
    AbstractNetworkBuilder<ValueType>,
    EuclidianDistanceCostBuilder<ValueType, Dimension>,
    TypeList<
        NeuralNetwork<ValueType>&,
        AbstractTensorMap<ValueType>&,
        AbstractTensorMap<ValueType>&,
        AbstractTensor<ValueType>&
    >
>;


using namespace BuildTimeList;

void
NetworkBuildersHeader::load(ResourcesContainerToken& token)
{
    RttiHolderToken<AbstractNetworkBuilder<float>>& networkBuilderFloatRttiHolderToken = token.getRttiHolderToken<AbstractNetworkBuilder<float>>();
    RttiHolderToken<AbstractNetworkBuilder<double>>& networkBuilderDoubleRttiHolderToken = token.getRttiHolderToken<AbstractNetworkBuilder<double>>();

    TemplateRtti<AbstractNetworkBuilder<float>, TemplateProvider<float>::EuclidianDistanceCostBuilderTemplate>& euclidianDistanceCostBuilderFloatTemplateRtti = networkBuilderFloatRttiHolderToken.getOrSubscribeTemplateRtti<TemplateProvider<float>::EuclidianDistanceCostBuilderTemplate>("EuclidianDistanceCostBuilder<float>");
    euclidianDistanceCostBuilderFloatTemplateRtti.subscribe<size_t{0}>(new EuclidianDistanceCostBuilderRtti<float, 0>("EuclidianDistanceCostBuilder<float,0>"));
    euclidianDistanceCostBuilderFloatTemplateRtti.subscribe<size_t{1}>(new EuclidianDistanceCostBuilderRtti<float, 1>("EuclidianDistanceCostBuilder<float,1>"));
    euclidianDistanceCostBuilderFloatTemplateRtti.subscribe<size_t{2}>(new EuclidianDistanceCostBuilderRtti<float, 2>("EuclidianDistanceCostBuilder<float,2>"));
    euclidianDistanceCostBuilderFloatTemplateRtti.subscribe<size_t{3}>(new EuclidianDistanceCostBuilderRtti<float, 3>("EuclidianDistanceCostBuilder<float,3>"));
    euclidianDistanceCostBuilderFloatTemplateRtti.subscribe<size_t{4}>(new EuclidianDistanceCostBuilderRtti<float, 4>("EuclidianDistanceCostBuilder<float,4>"));
    euclidianDistanceCostBuilderFloatTemplateRtti.subscribe<size_t{5}>(new EuclidianDistanceCostBuilderRtti<float, 5>("EuclidianDistanceCostBuilder<float,5>"));

    TemplateRtti<AbstractNetworkBuilder<double>, TemplateProvider<double>::EuclidianDistanceCostBuilderTemplate>& euclidianDistanceCostBuilderDoubleTemplateRtti = networkBuilderDoubleRttiHolderToken.getOrSubscribeTemplateRtti<TemplateProvider<double>::EuclidianDistanceCostBuilderTemplate>("EuclidianDistanceCostBuilder<double>");
    euclidianDistanceCostBuilderDoubleTemplateRtti.subscribe<size_t{0}>(new EuclidianDistanceCostBuilderRtti<double, 0>("EuclidianDistanceCostBuilder<double,0>"));
    euclidianDistanceCostBuilderDoubleTemplateRtti.subscribe<size_t{1}>(new EuclidianDistanceCostBuilderRtti<double, 1>("EuclidianDistanceCostBuilder<double,1>"));
    euclidianDistanceCostBuilderDoubleTemplateRtti.subscribe<size_t{2}>(new EuclidianDistanceCostBuilderRtti<double, 2>("EuclidianDistanceCostBuilder<double,2>"));
    euclidianDistanceCostBuilderDoubleTemplateRtti.subscribe<size_t{3}>(new EuclidianDistanceCostBuilderRtti<double, 3>("EuclidianDistanceCostBuilder<double,3>"));
    euclidianDistanceCostBuilderDoubleTemplateRtti.subscribe<size_t{4}>(new EuclidianDistanceCostBuilderRtti<double, 4>("EuclidianDistanceCostBuilder<double,4>"));
    euclidianDistanceCostBuilderDoubleTemplateRtti.subscribe<size_t{5}>(new EuclidianDistanceCostBuilderRtti<double, 5>("EuclidianDistanceCostBuilder<double,5>"));
}
