#include <plugincore/ResourcesContainer.h>

ResourcesContainerToken::ResourcesContainerToken(ResourcesContainer& owner)
    : m_owner(&owner)
    , m_floatModuleToken(owner.m_floatModules.edit())
    , m_doubleModuleToken(owner.m_doubleModules.edit())
    , m_floatTensorMapToken(owner.m_floatTensorMaps.edit())
    , m_doubleTensorMapToken(owner.m_doubleTensorMaps.edit())
    , m_tensorMapKeyEnumToken(owner.m_tensorMapKeyEnums.edit())
{}

ResourcesContainerToken::ResourcesContainerToken(ResourcesContainerToken&& other)
    : m_owner(other.m_owner)
    , m_floatModuleToken(std::move(other.m_floatModuleToken))
    , m_doubleModuleToken(std::move(other.m_doubleModuleToken))
    , m_floatTensorMapToken(std::move(other.m_floatTensorMapToken))
    , m_doubleTensorMapToken(std::move(other.m_doubleTensorMapToken))
    , m_tensorMapKeyEnumToken(std::move(other.m_tensorMapKeyEnumToken))
{
    other.m_owner = nullptr;
}

ResourcesContainerToken::~ResourcesContainerToken()
{}

ResourcesContainerToken&
ResourcesContainerToken::operator=(ResourcesContainerToken&& other)
{
    std::swap(m_owner, other.m_owner);
    m_floatModuleToken = std::move(other.m_floatModuleToken);
    m_doubleModuleToken = std::move(other.m_doubleModuleToken);
    m_floatTensorMapToken = std::move(other.m_floatTensorMapToken);
    m_doubleTensorMapToken = std::move(other.m_doubleTensorMapToken);
    m_tensorMapKeyEnumToken = std::move(other.m_tensorMapKeyEnumToken);
    return *this;
}

template <typename ValueType>
RttiHolderToken<Module<ValueType>>&
ResourcesContainerToken::modules()
{
    static_assert(std::is_same_v<ValueType, float> || std::is_same_v<ValueType, double>, "ResourcesContainerToken::modules(): Only float and double are supported.");

    if constexpr (std::is_same_v<ValueType, float>)
    {
        return m_floatModuleToken;
    }
    else if constexpr (std::is_same_v<ValueType, double>)
    {
        return m_doubleModuleToken;
    }
}

template MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolderToken<Module<float>>& ResourcesContainerToken::modules<float>();
template MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolderToken<Module<double>>& ResourcesContainerToken::modules<double>();

template <typename ValueType>
RttiHolderToken<AbstractTensorMap<ValueType>>&
ResourcesContainerToken::tensorMaps()
{
    static_assert(std::is_same_v<ValueType, float> || std::is_same_v<ValueType, double>, "ResourcesContainerToken::tensorMaps(): Only float and double are supported.");

    if constexpr (std::is_same_v<ValueType, float>)
    {
        return m_floatTensorMapToken;
    }
    else if constexpr (std::is_same_v<ValueType, double>)
    {
        return m_doubleTensorMapToken;
    }
}

template MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolderToken<AbstractTensorMap<float>>& ResourcesContainerToken::tensorMaps<float>();
template MY_NEURAL_NETWORK_LIB__PLUGINCORE__API RttiHolderToken<AbstractTensorMap<double>>& ResourcesContainerToken::tensorMaps<double>();

RttiHolderToken<AbstractTensorMapKeyEnum>&
ResourcesContainerToken::tensorMapKeyEnums()
{
    return m_tensorMapKeyEnumToken;
}

ResourcesContainer::ResourcesContainer()
    : m_floatModules()
    , m_doubleModules()
    , m_floatTensorMaps()
    , m_doubleTensorMaps()
    , m_tensorMapKeyEnums()
{}

ResourcesContainer::ResourcesContainer(ResourcesContainer&& other)
    : m_floatModules(std::move(other.m_floatModules))
    , m_doubleModules(std::move(other.m_doubleModules))
    , m_floatTensorMaps(std::move(other.m_floatTensorMaps))
    , m_doubleTensorMaps(std::move(other.m_doubleTensorMaps))
    , m_tensorMapKeyEnums(std::move(other.m_tensorMapKeyEnums))
{}

ResourcesContainer::~ResourcesContainer()
{}

ResourcesContainer&
ResourcesContainer::operator=(ResourcesContainer&& other)
{
    m_floatModules = std::move(other.m_floatModules);
    m_doubleModules = std::move(other.m_doubleModules);
    m_floatTensorMaps = std::move(other.m_floatTensorMaps);
    m_doubleTensorMaps = std::move(other.m_doubleTensorMaps);
    m_tensorMapKeyEnums = std::move(other.m_tensorMapKeyEnums);
    return *this;
}

template <typename ValueType>
const RttiHolder<Module<ValueType>>&
ResourcesContainer::modules() const
{
    static_assert(std::is_same_v<ValueType, float> || std::is_same_v<ValueType, double>, "ResourcesContainer::modules(): Only float and double are supported.");

    if constexpr (std::is_same_v<ValueType, float>)
    {
        return m_floatModules;
    }
    else if constexpr (std::is_same_v<ValueType, double>)
    {
        return m_doubleModules;
    }
}

template MY_NEURAL_NETWORK_LIB__PLUGINCORE__API const RttiHolder<Module<float>>& ResourcesContainer::modules<float>() const;
template MY_NEURAL_NETWORK_LIB__PLUGINCORE__API const RttiHolder<Module<double>>& ResourcesContainer::modules<double>() const;

template <typename ValueType>
const RttiHolder<AbstractTensorMap<ValueType>>&
ResourcesContainer::tensorMaps() const
{
    static_assert(std::is_same_v<ValueType, float> || std::is_same_v<ValueType, double>, "ResourcesContainer::tensorMaps(): Only float and double are supported.");

    if constexpr (std::is_same_v<ValueType, float>)
    {
        return m_floatTensorMaps;
    }
    else if constexpr (std::is_same_v<ValueType, double>)
    {
        return m_doubleTensorMaps;
    }
}

const RttiHolder<AbstractTensorMapKeyEnum>&
ResourcesContainer::tensorMapKeyEnums() const
{
    return m_tensorMapKeyEnums;
}

ResourcesContainerToken
ResourcesContainer::edit()
{
    return ResourcesContainerToken(*this);
}
