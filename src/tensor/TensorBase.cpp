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

template <TypenameArgId ValueTypeId, size_t Dimension>
using TypenameCompliantTensorTemplate = Tensor<TypeFromTypenameArgId<ValueTypeId>, Dimension>;

template <typename ValueType, size_t Dimension>
using TensorEntry = MapEntry<
    Tuple<TypenameArgIdOf<ValueType>, size_t{Dimension}>,
    TypeList<BuildTimeFunctionPointer<createTensor<ValueType, Dimension>>>
>;

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
                    TensorEntry<float, 1>,
                    TensorEntry<float, 2>,
                    TensorEntry<float, 3>,
                    TensorEntry<float, 4>,
                    TensorEntry<float, 5>,
                    TensorEntry<double, 1>,
                    TensorEntry<double, 2>,
                    TensorEntry<double, 3>,
                    TensorEntry<double, 4>,
                    TensorEntry<double, 5>
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
