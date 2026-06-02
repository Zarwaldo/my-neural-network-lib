#pragma once

#include <plugincore/api.h>

#include <module/Module.h>

#include <rtti/RttiHolder.h>

#include <tensor/AbstractTensorMapKeyEnum.h>

#include <CommonMacros.h>

class ResourcesContainer;

class MY_NEURAL_NETWORK_LIB__PLUGINCORE__API ResourcesContainerToken
{
public:
    ResourcesContainerToken(const ResourcesContainerToken& other) = delete;
    ResourcesContainerToken(ResourcesContainerToken&& other);

    ~ResourcesContainerToken();

    ResourcesContainerToken& operator=(const ResourcesContainerToken& other) = delete;
    ResourcesContainerToken& operator=(ResourcesContainerToken&& other);

    template <typename ValueType>
    RttiHolderToken<Module<ValueType>>& modules();

    template <typename ValueType>
    RttiHolderToken<AbstractTensorMap<ValueType>>& tensorMaps();

    RttiHolderToken<AbstractTensorMapKeyEnum>& tensorMapKeyEnums();

private:
    ResourcesContainerToken(ResourcesContainer& owner);

    ResourcesContainer* m_owner;
    RttiHolderToken<Module<float>> m_floatModuleToken;
    RttiHolderToken<Module<double>> m_doubleModuleToken;
    RttiHolderToken<AbstractTensorMap<float>> m_floatTensorMapToken;
    RttiHolderToken<AbstractTensorMap<double>> m_doubleTensorMapToken;
    RttiHolderToken<AbstractTensorMapKeyEnum> m_tensorMapKeyEnumToken;

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

    template <typename ValueType>
    const RttiHolder<AbstractTensorMap<ValueType>>& tensorMaps() const;

    const RttiHolder<AbstractTensorMapKeyEnum>& tensorMapKeyEnums() const;

    ResourcesContainerToken edit();

private:
    RttiHolder<Module<float>> m_floatModules;
    RttiHolder<Module<double>> m_doubleModules;
    RttiHolder<AbstractTensorMap<float>> m_floatTensorMaps;
    RttiHolder<AbstractTensorMap<double>> m_doubleTensorMaps;
    RttiHolder<AbstractTensorMapKeyEnum> m_tensorMapKeyEnums;

    friend class ResourcesContainerToken;
};
