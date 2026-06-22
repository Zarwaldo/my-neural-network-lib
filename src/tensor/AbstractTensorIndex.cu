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

    result->setValid(isValid() && other.isValid());

    return result;
}

HOST
AbstractTensorIndex*
AbstractTensorIndex::range(long long int from, long long int to) const
{
    if (from < 0 || from >= dim() || to < 0 || to >= dim())
    {
        from = from % static_cast<long long int>(dim());
        if (from < 0)
            from += dim();

        to = to % static_cast<long long int>(dim());
        if (to < 0)
            to += dim();
    }

    if (to < from)
        throw std::runtime_error("AbstractTensorIndex::range: Incorrect range from " + std::to_string(from) + " to " + std::to_string(to) + ".");

    const size_t dim = to - from + 1;
    const AbstractRtti<AbstractTensorIndex>* tensorIndexRtti = templateRtti().instantiate(Initializer<size_t>(std::move(size_t{dim})));

    if (!isValid())
        return tensorIndexRtti->createInstance(Initializer<>());

    size_t* array = new size_t[dim];
    for (size_t index = 0; index < dim; ++index)
        array[index] = (*this)[from + index];

    AbstractTensorIndex* result = tensorIndexRtti->createInstance(Initializer<const size_t*>(static_cast<const size_t*>(array)));

    delete[] array;

    return result;
}

class TensorIndexRttiHolderInitializer
{
public:
    template <size_t Dimension>
    using TensorIndexCtorParamTypes = TypeList<
        TypeList<>,
        TypeList<const size_t*>,
        TypeList<const std::initializer_list<size_t>&>,
        TypeList<const AbstractTensorIndex&>,
        TypeList<const RawTensorIndex<Dimension>&>
    >;

    TensorIndexRttiHolderInitializer()
        : tensorIndexRttiHolder(new RttiHolder<AbstractTensorIndex>())
        , tensorIndexRttiHolderToken(new RttiHolderToken<AbstractTensorIndex>(tensorIndexRttiHolder->edit()))
    {
        tensorIndexRttiHolderToken->subscribe(
            new TemplateRtti<
                AbstractTensorIndex,
                TensorIndex,
                Map<
                    MapEntry<Tuple<size_t{0}>, TensorIndexCtorParamTypes<0>>,
                    MapEntry<Tuple<size_t{1}>, TensorIndexCtorParamTypes<1>>,
                    MapEntry<Tuple<size_t{2}>, TensorIndexCtorParamTypes<2>>,
                    MapEntry<Tuple<size_t{3}>, TensorIndexCtorParamTypes<3>>,
                    MapEntry<Tuple<size_t{4}>, TensorIndexCtorParamTypes<4>>,
                    MapEntry<Tuple<size_t{5}>, TensorIndexCtorParamTypes<5>>
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
