#include <tensor/TensorBase.h>

#include <rtti/AbstractTemplateRtti.h>
#include <rtti/Rtti.h>
#include <rtti/RttiHolder.impl.h>
#include <rtti/TemplateRtti.h>
#include <rtti/TypenameArgId.h>

#include <tensor/TensorIndex.h>
#include <tensor/Tensor.h>

using namespace BuildTimeList;

TensorBase::~TensorBase()
{
}

template <TypenameArgId ValueTypeId, size_t Dimension>
using NTTPTensorTemplate = Tensor<TypeFromTypenameArgId<ValueTypeId>, Dimension>;

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

template <typename ValueType, size_t Dimension>
using TensorRtti = Rtti<
    TensorBase,
    Tensor<ValueType, Dimension>,
    BuildTimeFunctionPointer<createTensor<ValueType, Dimension>>
>;

class TensorRttiHolderInitializer
{
public:
    TensorRttiHolderInitializer()
        : tensorRttiHolder(new RttiHolder<TensorBase>())
        , tensorRttiHolderToken(new RttiHolderToken<TensorBase>(tensorRttiHolder->edit()))
    {
        TemplateRtti<TensorBase, NTTPTensorTemplate>& tensorTemplateRtti = tensorRttiHolderToken->getOrSubscribeTemplateRtti<NTTPTensorTemplate>("Tensor");
        tensorTemplateRtti.subscribe<TypenameArgIdOf<float>, size_t{0}>(new TensorRtti<float, 0>("Tensor<float,0>"));
        tensorTemplateRtti.subscribe<TypenameArgIdOf<float>, size_t{1}>(new TensorRtti<float, 1>("Tensor<float,1>"));
        tensorTemplateRtti.subscribe<TypenameArgIdOf<float>, size_t{2}>(new TensorRtti<float, 2>("Tensor<float,2>"));
        tensorTemplateRtti.subscribe<TypenameArgIdOf<float>, size_t{3}>(new TensorRtti<float, 3>("Tensor<float,3>"));
        tensorTemplateRtti.subscribe<TypenameArgIdOf<float>, size_t{4}>(new TensorRtti<float, 4>("Tensor<float,4>"));
        tensorTemplateRtti.subscribe<TypenameArgIdOf<float>, size_t{5}>(new TensorRtti<float, 5>("Tensor<float,5>"));
        tensorTemplateRtti.subscribe<TypenameArgIdOf<double>, size_t{0}>(new TensorRtti<double, 0>("Tensor<double,0>"));
        tensorTemplateRtti.subscribe<TypenameArgIdOf<double>, size_t{1}>(new TensorRtti<double, 1>("Tensor<double,1>"));
        tensorTemplateRtti.subscribe<TypenameArgIdOf<double>, size_t{2}>(new TensorRtti<double, 2>("Tensor<double,2>"));
        tensorTemplateRtti.subscribe<TypenameArgIdOf<double>, size_t{3}>(new TensorRtti<double, 3>("Tensor<double,3>"));
        tensorTemplateRtti.subscribe<TypenameArgIdOf<double>, size_t{4}>(new TensorRtti<double, 4>("Tensor<double,4>"));
        tensorTemplateRtti.subscribe<TypenameArgIdOf<double>, size_t{5}>(new TensorRtti<double, 5>("Tensor<double,5>"));
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
