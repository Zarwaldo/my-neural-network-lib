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

template <size_t Dimension>
using AdditionModuleEntry = MapEntry<
    Tuple<size_t{Dimension}>,
    TypeList<
        TypeList<
            const RawTuple<const TensorIndex<Dimension>&>&
        >
    >
>;

template <size_t InputDimension, size_t OutputDimension>
using PerceptronModuleEntry = MapEntry<
    Tuple<size_t{InputDimension}, size_t{OutputDimension}>,
    TypeList<
        TypeList<
            const RawTuple<const TensorIndex<InputDimension + OutputDimension>&, const TensorIndex<OutputDimension>&>&
        >
    >
>;

template <typename ValueType, typename KeyEnum>
using TensorMapEntry = MapEntry<
    Tuple<TypenameArgIdOf<KeyEnum>>,
    TypeList<
        TypeList<AbstractTensor<ValueType>* const [KeyEnum::NbValues]>,
        TypeList<const std::initializer_list<AbstractTensor<ValueType>*>&>
    >
>;

static ResourcesContainerToken* token = nullptr;

void
openNeuralNetworkPlugin(ResourcesContainer* resourcesContainer)
{
    token = new ResourcesContainerToken(resourcesContainer->edit());

    token->modules<float>().subscribe(
        new TemplateRtti<
            Module<float>,
            TemplateProvider<float>::AdditionModuleTemplate,
            Map<
                AdditionModuleEntry<1>,
                AdditionModuleEntry<2>,
                AdditionModuleEntry<3>,
                AdditionModuleEntry<4>,
                AdditionModuleEntry<5>
            >
        >("AdditionModule<float>")
    );
    token->modules<double>().subscribe(
        new TemplateRtti<
            Module<double>,
            TemplateProvider<double>::AdditionModuleTemplate,
            Map<
                AdditionModuleEntry<1>,
                AdditionModuleEntry<2>,
                AdditionModuleEntry<3>,
                AdditionModuleEntry<4>,
                AdditionModuleEntry<5>
            >
        >("AdditionModule<double>")
    );

    token->modules<float>().subscribe(
        new TemplateRtti<
            Module<float>,
            TemplateProvider<float>::PerceptronModuleTemplate,
            Map<
                PerceptronModuleEntry<1, 1>,
                PerceptronModuleEntry<1, 2>,
                PerceptronModuleEntry<1, 3>,
                PerceptronModuleEntry<2, 1>,
                PerceptronModuleEntry<2, 2>,
                PerceptronModuleEntry<2, 3>,
                PerceptronModuleEntry<3, 1>,
                PerceptronModuleEntry<3, 2>,
                PerceptronModuleEntry<3, 3>
            >
        >("PerceptronModule<float>")
    );
    token->modules<double>().subscribe(
        new TemplateRtti<
            Module<double>,
            TemplateProvider<double>::PerceptronModuleTemplate,
            Map<
                PerceptronModuleEntry<1, 1>,
                PerceptronModuleEntry<1, 2>,
                PerceptronModuleEntry<1, 3>,
                PerceptronModuleEntry<2, 1>,
                PerceptronModuleEntry<2, 2>,
                PerceptronModuleEntry<2, 3>,
                PerceptronModuleEntry<3, 1>,
                PerceptronModuleEntry<3, 2>,
                PerceptronModuleEntry<3, 3>
            >
        >("PerceptronModule<double>")
    );

    token->tensorMaps<float>().subscribe(
        new TemplateRtti<
            AbstractTensorMap<float>,
            TemplateProvider<float>::TensorMapTemplate,
            Map<
                TensorMapEntry<float, TensorSingleton>,
                TensorMapEntry<float, PerceptronParamsKeyEnum>
            >
        >("TensorMap<float>")
    );

    token->tensorMaps<double>().subscribe(
        new TemplateRtti<
            AbstractTensorMap<double>,
            TemplateProvider<double>::TensorMapTemplate,
            Map<
                TensorMapEntry<double, TensorSingleton>,
                TensorMapEntry<double, PerceptronParamsKeyEnum>
            >
        >("TensorMap<double>")
    );

    token->tensorMapKeyEnums().subscribe(new Rtti<AbstractTensorMapKeyEnum, TensorSingleton, TypeList<size_t>>("TensorSingleton"));
    token->tensorMapKeyEnums().subscribe(new Rtti<AbstractTensorMapKeyEnum, PerceptronParamsKeyEnum, TypeList<size_t>>("PerceptronParamsKeyEnum"));
}

void
closeNeuralNetworkPlugin()
{
    delete token;
}
