#include <tensor/AbstractTensorIndex.h>

#include <rtti/RttiHolder.impl.h>
#include <rtti/TemplateRtti.h>

#include <tensor/TensorIndex.h>

using namespace BuildTimeList;

HOST DEVICE
AbstractTensorIndex::~AbstractTensorIndex()
{}

HOST
AbstractTensorIndex*
AbstractTensorIndex::operator*(const AbstractTensorIndex& other) const
{
    const size_t resultsDimension = dim() + other.dim();
    const AbstractRtti<AbstractTensorIndex>* resultsRtti = templateRtti().instantiate(Initializer<size_t>(std::move(size_t{resultsDimension})));
    if (resultsRtti == nullptr)
        return nullptr;

    AbstractTensorIndex* result = resultsRtti->createInstance(Initializer<>());
    if (result == nullptr)
        return nullptr;

    for (size_t d = 0; d < dim(); ++d)
        (*result)[d] = (*this)[d];

    for (size_t d = dim(); d < resultsDimension; ++d)
        (*result)[d] = other[d - dim()];

    result->setValid(true);

    return result;
}

class TensorIndexRttiHolderInitializer
{
public:
    TensorIndexRttiHolderInitializer()
        : tensorIndexRttiHolder(new RttiHolder<AbstractTensorIndex>())
        , tensorIndexRttiHolderToken(new RttiHolderToken<AbstractTensorIndex>(tensorIndexRttiHolder->edit()))
    {
        tensorIndexRttiHolderToken->subscribe(
            new TemplateRtti<
                AbstractTensorIndex,
                TensorIndex,
                Map<
                    MapEntry<
                        Tuple<size_t{1}>,
                        TypeList<
                            TypeList<>,
                            TypeList<const size_t*>,
                            TypeList<const std::initializer_list<size_t>&>,
                            TypeList<const RawTensorIndex<1>&>
                        >
                    >,
                    MapEntry<
                        Tuple<size_t{2}>,
                        TypeList<
                            TypeList<>,
                            TypeList<const size_t*>,
                            TypeList<const std::initializer_list<size_t>&>,
                            TypeList<const RawTensorIndex<2>&>
                        >
                    >,
                    MapEntry<
                        Tuple<size_t{3}>,
                        TypeList<
                            TypeList<>,
                            TypeList<const size_t*>,
                            TypeList<const std::initializer_list<size_t>&>,
                            TypeList<const RawTensorIndex<3>&>
                        >
                    >,
                    MapEntry<
                        Tuple<size_t{4}>,
                        TypeList<
                            TypeList<>,
                            TypeList<const size_t*>,
                            TypeList<const std::initializer_list<size_t>&>,
                            TypeList<const RawTensorIndex<4>&>
                        >
                    >,
                    MapEntry<
                        Tuple<size_t{5}>,
                        TypeList<
                            TypeList<>,
                            TypeList<const size_t*>,
                            TypeList<const std::initializer_list<size_t>&>,
                            TypeList<const RawTensorIndex<5>&>
                        >
                    >
                >
            >("TensorIndex")
        );
    }

    TensorIndexRttiHolderInitializer(const TensorIndexRttiHolderInitializer& other) = delete;
    TensorIndexRttiHolderInitializer(TensorIndexRttiHolderInitializer&& other) = delete;

    ~TensorIndexRttiHolderInitializer()
    {
        delete tensorIndexRttiHolderToken;
        delete tensorIndexRttiHolder;
    }

    TensorIndexRttiHolderInitializer operator=(const TensorIndexRttiHolderInitializer& other) = delete;
    TensorIndexRttiHolderInitializer operator=(TensorIndexRttiHolderInitializer&& other) = delete;

    RttiHolder<AbstractTensorIndex>* tensorIndexRttiHolder;
    RttiHolderToken<AbstractTensorIndex>* tensorIndexRttiHolderToken;
};

static const TensorIndexRttiHolderInitializer rttiInitializer;

HOST
const AbstractTemplateRtti<AbstractTensorIndex>&
AbstractTensorIndex::templateRtti()
{
    return *rttiInitializer.tensorIndexRttiHolder->getTemplateRttiByName("TensorIndex");
}
