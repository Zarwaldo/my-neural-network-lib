#pragma once

#include <rtti/Rtti.h>

#include <map>
#include <string>

template <typename ScalarType>
class AbstractTensor;

template <typename ScalarType>
class AbstractNetworkBuilder
{
public:
    virtual ~AbstractNetworkBuilder() = default;

    virtual std::map<std::string, void*> build() = 0;

    DECLARE_AS_RTTI_BASE_TYPE(AbstractNetworkBuilder<ScalarType>)
};
