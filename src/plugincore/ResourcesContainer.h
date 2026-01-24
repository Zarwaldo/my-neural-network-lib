#pragma once

#include <plugincore/api.h>

#include <module/Module.h>

#include <rtti/RttiHolder.h>

#include <CommonMacros.h>

class ResourcesContainer;

class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API ResourcesContainerToken
{
public:
    ResourcesContainerToken(const ResourcesContainerToken& other) = delete;
    ResourcesContainerToken(ResourcesContainerToken&& other);

    ~ResourcesContainerToken();

    template <typename ValueType>
    RttiHolderToken<Module<ValueType>>& modules();

private:
    ResourcesContainerToken(ResourcesContainer& owner);

    ResourcesContainer* m_owner;
    RttiHolderToken<Module<float>> m_floatModuleToken;
    RttiHolderToken<Module<double>> m_doubleModuleToken;

    friend class ResourcesContainer;
};

class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API ResourcesContainer
{
public:
    ResourcesContainer();
    ResourcesContainer(const ResourcesContainer& other) = delete;
    ResourcesContainer(ResourcesContainer&& other);

    ~ResourcesContainer();

    ResourcesContainer& operator=(const ResourcesContainer& other) = delete;
    ResourcesContainer& operator=(ResourcesContainer&& other);

    template <typename ValueType>
    const RttiHolder<Module<ValueType>>& modules() const;

    ResourcesContainerToken edit();

private:
    RttiHolder<Module<float>> m_floatModules;
    RttiHolder<Module<double>> m_doubleModules;

    friend class ResourcesContainerToken;
};
