#pragma once

#include <rtti/AbstractTemplateRtti.h>

#include <rtti/Rtti.h>

#include <helpers/BuildTimeList.h>

template <typename BaseType>
class TemplateRttiPimpl;

template <typename BaseType, template <auto...> typename Template>
class TemplateRtti : public AbstractTemplateRtti<BaseType>
{
public:
    inline TemplateRtti(const std::string& templateName);

    virtual inline ~TemplateRtti();

    virtual inline const std::string& getTypeName() const override;

    inline const AbstractRtti<BaseType>* instantiate(AbstractInitializer&& initializer) const override;

    template <auto... TemplateParams>
    inline void subscribe(const AbstractRtti<BaseType>* rtti);

private:
    TemplateRttiPimpl<BaseType>* m_pimpl;
};

#include <rtti/TemplateRtti.impl.h>
