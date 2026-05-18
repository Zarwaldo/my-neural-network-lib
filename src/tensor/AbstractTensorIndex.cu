#include <tensor/AbstractTensorIndex.h>

HOST DEVICE
AbstractTensorIndex::~AbstractTensorIndex()
{}

HOST
AbstractTensorIndex*
AbstractTensorIndex::operator*(const AbstractTensorIndex& other) const
{
    const size_t resultsDimension = dim() + other.dim();
    const AbstractRtti<AbstractTensorIndex>* resultsRtti = nullptr; // Waiting for TensorIndex template rtti
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
