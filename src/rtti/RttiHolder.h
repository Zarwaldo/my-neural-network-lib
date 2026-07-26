#pragma once

#include <string>

template <typename BaseType>
class RttiBase;

template <typename BaseType>
class AbstractRtti;

template <typename BaseType>
class AbstractTemplateRtti;

template <typename BaseType>
class RttiHolder;

template <typename BaseType>
class RttiHolderTokenPimpl;

template <typename BaseType, template <auto...> typename Template>
class TemplateRtti;

template <typename BaseType>
class RttiHolderToken
{
public:
    inline RttiHolderToken(RttiHolderToken<BaseType>& other) = delete;
    inline RttiHolderToken(RttiHolderToken<BaseType>&& other);

    inline ~RttiHolderToken();

    inline RttiHolderToken<BaseType>& operator=(const RttiHolderToken<BaseType>& other) = delete;
    inline RttiHolderToken<BaseType>& operator=(RttiHolderToken<BaseType>&& other);

    inline void subscribe(RttiBase<BaseType>* rtti);
    template <template <auto...> typename Template>
    inline TemplateRtti<BaseType, Template>& getOrSubscribeTemplateRtti(const std::string& templateName);

private:
    inline RttiHolderToken(RttiHolder<BaseType>* owner);

    RttiHolderTokenPimpl<BaseType>* m_pimpl;

    friend class RttiHolder<BaseType>;
};

template <typename BaseType>
class RttiHolderPimpl;

template <typename BaseType>
class RttiHolder
{
public:
    inline RttiHolder();
    inline RttiHolder(const RttiHolder& other);
    inline RttiHolder(RttiHolder&& other);

    inline ~RttiHolder();

    inline RttiHolder& operator=(const RttiHolder& other);
    inline RttiHolder& operator=(RttiHolder&& other);

    inline const AbstractRtti<BaseType>* getRttiByName(const std::string& typeName) const;
    inline const AbstractTemplateRtti<BaseType>* getTemplateRttiByName(const std::string& templateName) const;

    inline RttiHolderToken<BaseType> edit();

private:
    inline void addRtti(RttiBase<BaseType>* rtti);
    inline void removeRtti(const RttiBase<BaseType>* rtti);

    template <template <auto...> typename Template>
    inline TemplateRtti<BaseType, Template>* getTemplateRttiByName(const std::string& templateName);

    RttiHolderPimpl<BaseType>* m_pimpl;

    friend class RttiHolderToken<BaseType>;
};
