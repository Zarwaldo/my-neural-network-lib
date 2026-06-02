#pragma once

#include <tensor/api.h>

#include <rtti/Rtti.h>

#include <CommonMacros.h>

template <typename BaseType>
class AbstractTemplateRtti;

class MY_NEURAL_NETWORK_LIB__TENSOR__CUDA_API AbstractTensorIndex
{
public:
    HOST DEVICE virtual ~AbstractTensorIndex();

    HOST DEVICE virtual size_t dim() const = 0;

    HOST DEVICE virtual bool isValid() const = 0;
    HOST DEVICE virtual void setValid(bool isValid) = 0;

    HOST DEVICE virtual AbstractTensorIndex& operator+=(const AbstractTensorIndex& other) = 0;
    HOST DEVICE virtual AbstractTensorIndex& operator-=(const AbstractTensorIndex& other) = 0;

    HOST DEVICE virtual size_t dot(const AbstractTensorIndex& other) const = 0;
    HOST DEVICE virtual size_t nbInferiorIndices() const = 0;

    HOST DEVICE virtual bool operator==(const AbstractTensorIndex& other) const = 0;
    HOST DEVICE virtual bool operator!=(const AbstractTensorIndex& other) const = 0;
    HOST DEVICE virtual bool operator>=(const AbstractTensorIndex& other) const = 0;
    HOST DEVICE virtual bool operator>(const AbstractTensorIndex& other) const = 0;
    HOST DEVICE virtual bool operator<=(const AbstractTensorIndex& other) const = 0;
    HOST DEVICE virtual bool operator<(const AbstractTensorIndex& other) const = 0;
    HOST virtual AbstractTensorIndex* operator*(const AbstractTensorIndex& other) const;

    HOST DEVICE virtual const size_t& operator[](const size_t index) const = 0;
    HOST DEVICE virtual size_t& operator[](const size_t index) = 0;

    HOST DEVICE virtual const size_t* begin() const = 0;
    HOST DEVICE virtual const size_t* end() const = 0;
    HOST DEVICE virtual size_t* begin() = 0;
    HOST DEVICE virtual size_t* end() = 0;

    HOST static const AbstractTemplateRtti<AbstractTensorIndex>& templateRtti();

    DECLARE_AS_RTTI_BASE_TYPE(AbstractTensorIndex)
};
