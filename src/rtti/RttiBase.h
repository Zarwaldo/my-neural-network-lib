#pragma once

#include <string>

template <typename BaseType>
class RttiBase
{
public:
    virtual inline ~RttiBase();

    virtual inline const std::string& getTypeName() const = 0;
};

#include <rtti/RttiBase.impl.h>
