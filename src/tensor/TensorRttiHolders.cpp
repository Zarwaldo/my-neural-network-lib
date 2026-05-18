#include <tensor/TensorRttiHolders.h>

#include <tensor/api.h>
#include <tensor/TensorIndex.h>
#include <tensor/Tensor.h>

#include <rtti/AbstractTemplateRtti.h>
#include <rtti/RttiHolder.impl.h>
#include <rtti/TemplateRtti.h>

using namespace BuildTimeList;

namespace
{
    template <typename ValueType, size_t Dimension>
    Tensor<ValueType, Dimension>* createTensor(const AbstractTensorIndex& sizes)
    {
        const TensorIndex<Dimension>* concreteSizes = dynamic_cast<const TensorIndex<Dimension>*>(&sizes);
        if (concreteSizes == nullptr)
        {
            return nullptr;
        }

        return Tensor<ValueType, Dimension>::create(*concreteSizes);
    }
}

template <size_t Dimension>
using FloatTensor = Tensor<float, Dimension>;

template <size_t Dimension>
using DoubleTensor = Tensor<double, Dimension>;

class TensorRttiHoldersInitializer
{
public:
    TensorRttiHoldersInitializer()
        : floatTensorRttiHolder(new RttiHolder<AbstractTensor<float>>())
        , floatTensorRttiHolderToken(new RttiHolderToken<AbstractTensor<float>>(floatTensorRttiHolder->edit()))
        , doubleTensorRttiHolder(new RttiHolder<AbstractTensor<double>>())
        , doubleTensorRttiHolderToken(new RttiHolderToken<AbstractTensor<double>>(doubleTensorRttiHolder->edit()))
    {
        floatTensorRttiHolderToken->subscribe(
            new TemplateRtti<
                AbstractTensor<float>,
                FloatTensor,
                Map<
                    MapEntry<
                        Tuple<size_t{1}>,
                        TypeList<
                            BuildTimeFunctionPointer<createTensor<float, 1>>
                        >
                    >,
                    MapEntry<
                        Tuple<size_t{2}>,
                        TypeList<
                            BuildTimeFunctionPointer<createTensor<float, 2>>
                        >
                    >,
                    MapEntry<
                        Tuple<size_t{3}>,
                        TypeList<
                            BuildTimeFunctionPointer<createTensor<float, 3>>
                        >
                    >,
                    MapEntry<
                        Tuple<size_t{4}>,
                        TypeList<
                            BuildTimeFunctionPointer<createTensor<float, 4>>
                        >
                    >,
                    MapEntry<
                        Tuple<size_t{5}>,
                        TypeList<
                            BuildTimeFunctionPointer<createTensor<float, 5>>
                        >
                    >
                >
            >("Tensor<float>")
        );

        doubleTensorRttiHolderToken->subscribe(
            new TemplateRtti<
                AbstractTensor<double>,
                DoubleTensor,
                Map<
                    MapEntry<
                        Tuple<size_t{1}>,
                        TypeList<
                            BuildTimeFunctionPointer<createTensor<double, 1>>
                        >
                    >,
                    MapEntry<
                        Tuple<size_t{2}>,
                        TypeList<
                            BuildTimeFunctionPointer<createTensor<double, 2>>
                        >
                    >,
                    MapEntry<
                        Tuple<size_t{3}>,
                        TypeList<
                            BuildTimeFunctionPointer<createTensor<double, 3>>
                        >
                    >,
                    MapEntry<
                        Tuple<size_t{4}>,
                        TypeList<
                            BuildTimeFunctionPointer<createTensor<double, 4>>
                        >
                    >,
                    MapEntry<
                        Tuple<size_t{5}>,
                        TypeList<
                            BuildTimeFunctionPointer<createTensor<double, 5>>
                        >
                    >
                >
            >("Tensor<double>")
        );
    }

    ~TensorRttiHoldersInitializer()
    {
        delete floatTensorRttiHolderToken;
        delete floatTensorRttiHolder;

        delete doubleTensorRttiHolderToken;
        delete doubleTensorRttiHolder;
    }

    RttiHolder<AbstractTensor<float>>* floatTensorRttiHolder;
    RttiHolderToken<AbstractTensor<float>>* floatTensorRttiHolderToken;

    RttiHolder<AbstractTensor<double>>* doubleTensorRttiHolder;
    RttiHolderToken<AbstractTensor<double>>* doubleTensorRttiHolderToken;
};

static const TensorRttiHoldersInitializer initializer;

namespace TensorRttis
{
    template <typename ValueType>
    const AbstractTemplateRtti<AbstractTensor<ValueType>>& tensorTemplateRtti()
    {
        static_assert(std::is_same_v<ValueType, float> || std::is_same_v<ValueType, double>, "tensorTemplateRtti: Invalid ValueType, only float and double supported.");

        if constexpr (std::is_same_v<ValueType, float>)
        {
            return *initializer.floatTensorRttiHolder->getTemplateRttiByName("Tensor<float>");
        }

        if constexpr (std::is_same_v<ValueType, double>)
        {
            return *initializer.doubleTensorRttiHolder->getTemplateRttiByName("Tensor<double>");
        }
    }

    template MY_NEURAL_NETWORK_LIB__TENSOR__API const AbstractTemplateRtti<AbstractTensor<float>>& tensorTemplateRtti<float>();
    template MY_NEURAL_NETWORK_LIB__TENSOR__API const AbstractTemplateRtti<AbstractTensor<double>>& tensorTemplateRtti<double>();
}
