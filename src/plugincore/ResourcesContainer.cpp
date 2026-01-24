#include <plugincore/ResourcesContainer.h>

ResourcesContainerToken::ResourcesContainerToken(ResourcesContainer& owner)
    : m_owner(&owner)
    , m_floatModuleToken(owner.m_floatModules.edit())
    , m_doubleModuleToken(owner.m_doubleModules.edit())
{}

ResourcesContainerToken::ResourcesContainerToken(ResourcesContainerToken&& other)
    : m_owner(other.m_owner)
    , m_floatModuleToken(std::move(other.m_floatModuleToken))
    , m_doubleModuleToken(std::move(other.m_doubleModuleToken))
{
    other.m_owner = nullptr;
}

ResourcesContainerToken::~ResourcesContainerToken()
{}

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

ResourcesContainer::ResourcesContainer()
    : m_floatModules()
    , m_doubleModules()
{}

ResourcesContainer::ResourcesContainer(ResourcesContainer&& other)
    : m_floatModules(std::move(other.m_floatModules))
    , m_doubleModules(std::move(other.m_doubleModules))
{}

ResourcesContainer::~ResourcesContainer()
{}

ResourcesContainer&
ResourcesContainer::operator=(ResourcesContainer&& other)
{
    m_floatModules = std::move(other.m_floatModules);
    m_doubleModules = std::move(other.m_doubleModules);
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

ResourcesContainerToken
ResourcesContainer::edit()
{
    return ResourcesContainerToken(*this);
}
