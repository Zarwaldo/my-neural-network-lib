#pragma once

#include <rtti/Rtti.h>

#include <map>
#include <string>

template <typename ValueType>
class AbstractTensor;

template <typename ValueType>
class AbstractNetworkBuilder
{
public:
    virtual ~AbstractNetworkBuilder() = default;

    virtual std::map<std::string, void*> build() = 0;

    DECLARE_AS_RTTI_BASE_TYPE(AbstractNetworkBuilder<ValueType>)
};
