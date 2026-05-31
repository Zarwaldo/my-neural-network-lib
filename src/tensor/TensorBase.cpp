#include <tensor/TensorBase.h>

#include <rtti/AbstractTemplateRtti.h>
#include <rtti/RttiHolder.impl.h>
#include <rtti/TemplateRtti.h>
#include <rtti/TypenameArgId.h>

#include <tensor/TensorIndex.h>
#include <tensor/Tensor.h>

using namespace BuildTimeList;

TensorBase::~TensorBase()
{
}

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

template <TypenameArgId ValueType, size_t Dimension>
using TypenameCompliantTensorTemplate = Tensor<TypeFromTypenameArgId<ValueType>, Dimension>;

class TensorRttiHolderInitializer
{
public:
    TensorRttiHolderInitializer()
        : tensorRttiHolder(new RttiHolder<TensorBase>())
        , tensorRttiHolderToken(new RttiHolderToken<TensorBase>(tensorRttiHolder->edit()))
    {
        tensorRttiHolderToken->subscribe(
            new TemplateRtti<
                TensorBase,
                TypenameCompliantTensorTemplate,
                Map<
                    MapEntry<
                        Tuple<TypenameArgIdOf<float>, size_t{1}>,
                        TypeList<BuildTimeFunctionPointer<createTensor<float, 1>>>
                    >,
                    MapEntry<
                        Tuple<TypenameArgIdOf<float>, size_t{2}>,
                        TypeList<BuildTimeFunctionPointer<createTensor<float, 2>>>
                    >,
                    MapEntry<
                        Tuple<TypenameArgIdOf<float>, size_t{3}>,
                        TypeList<BuildTimeFunctionPointer<createTensor<float, 3>>>
                    >,
                    MapEntry<
                        Tuple<TypenameArgIdOf<float>, size_t{4}>,
                        TypeList<BuildTimeFunctionPointer<createTensor<float, 4>>>
                    >,
                    MapEntry<
                        Tuple<TypenameArgIdOf<float>, size_t{5}>,
                        TypeList<BuildTimeFunctionPointer<createTensor<float, 5>>>
                    >,
                    MapEntry<
                        Tuple<TypenameArgIdOf<double>, size_t{1}>,
                        TypeList<BuildTimeFunctionPointer<createTensor<double, 1>>>
                    >,
                    MapEntry<
                        Tuple<TypenameArgIdOf<double>, size_t{2}>,
                        TypeList<BuildTimeFunctionPointer<createTensor<double, 2>>>
                    >,
                    MapEntry<
                        Tuple<TypenameArgIdOf<double>, size_t{3}>,
                        TypeList<BuildTimeFunctionPointer<createTensor<double, 3>>>
                    >,
                    MapEntry<
                        Tuple<TypenameArgIdOf<double>, size_t{4}>,
                        TypeList<BuildTimeFunctionPointer<createTensor<double, 4>>>
                    >,
                    MapEntry<
                        Tuple<TypenameArgIdOf<double>, size_t{5}>,
                        TypeList<BuildTimeFunctionPointer<createTensor<double, 5>>>
                    >
                >
            >("Tensor")
        );
    }

    ~TensorRttiHolderInitializer()
    {
        delete tensorRttiHolderToken;
        delete tensorRttiHolder;
    }

    RttiHolder<TensorBase>* tensorRttiHolder;
    RttiHolderToken<TensorBase>* tensorRttiHolderToken;
};

static const TensorRttiHolderInitializer initializer;

HOST
const AbstractTemplateRtti<TensorBase>&
TensorBase::templateRtti()
{
    return *initializer.tensorRttiHolder->getTemplateRttiByName("Tensor");
}
