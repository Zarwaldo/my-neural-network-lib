#include <plugins/natives/tensormaps/TensorMapsHeader.h>

#include <plugincore/ResourcesContainer.h>

#include <plugins/natives/tensormaps/TensorMapKeyEnums.h>

#include <rtti/RttiHolder.impl.h>
#include <rtti/TemplateRtti.h>
#include <rtti/TypenameArgId.h>

#include <tensor/TensorMap.h>

using namespace BuildTimeList;

template <typename ValueType>
struct TemplateProvider
{
    template <TypenameArgId KeyEnumId>
    using TensorMapTemplate = TensorMap<ValueType, TypeFromTypenameArgId<KeyEnumId>>;
};

template <typename ValueType, typename KeyEnum>
using TensorMapRtti = Rtti<
    AbstractTensorMap<ValueType>,
    TensorMap<ValueType, KeyEnum>,
    TypeList<AbstractTensor<ValueType>* const *>,
    TypeList<const std::initializer_list<AbstractTensor<ValueType>*>&>
>;

void
TensorMapsHeader::load(ResourcesContainerToken& token)
{
    RttiHolderToken<AbstractTensorMap<float>>& tensorMapFloatRttiHolderToken = token.getRttiHolderToken<AbstractTensorMap<float>>();
    RttiHolderToken<AbstractTensorMap<double>>& tensorMapDoubleRttiHolderToken = token.getRttiHolderToken<AbstractTensorMap<double>>();

    token.getRttiHolderToken<AbstractTensorMapKeyEnum>().subscribe(
        new Rtti<
            AbstractTensorMapKeyEnum,
            TensorSingleton,
            TypeList<size_t>
        >("TensorSingleton")
    );
    token.getRttiHolderToken<AbstractTensorMapKeyEnum>().subscribe(
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
