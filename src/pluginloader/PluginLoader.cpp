#include <pluginloader/PluginLoader.h>

#include <pluginloader/Plugin.h>

#include <plugincore/ResourcesContainer.h>

#include <filesystem>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

class PluginLoaderPimpl
{
public:
    PluginLoaderPimpl()
        : m_plugins()
        , m_resourcesContainer()
    {}

    PluginLoaderPimpl(const PluginLoaderPimpl& other) = delete;

    PluginLoaderPimpl(PluginLoaderPimpl&& other) = delete;

    ~PluginLoaderPimpl()
    {
        // Close all plugins to empty the ResourcesContainer before deleting it
        m_plugins.clear();
    }

    PluginLoaderPimpl& operator=(const PluginLoaderPimpl& other) = delete;

    PluginLoaderPimpl& operator=(PluginLoaderPimpl&& other) = delete;

    std::vector<Plugin> m_plugins;
    ResourcesContainer m_resourcesContainer;
};

PluginLoader::PluginLoader(bool loadNatives)
    : m_pimpl(new PluginLoaderPimpl)
{
    if (loadNatives)
    {
#ifdef _WIN32
        loadPluginFromPath(getExecutableDir() / "natives.dll");
#else
        loadPluginFromPath(getExecutableDir() / "natives.so");
#endif
    }
}

PluginLoader::~PluginLoader()
{
    delete m_pimpl;
}

void
PluginLoader::loadPluginFromPath(const std::filesystem::path& libraryPath)
{
    m_pimpl->m_plugins.push_back(Plugin(libraryPath.stem().c_str(), libraryPath, m_pimpl->m_resourcesContainer));
}

void
PluginLoader::loadPluginsFromDirPath(const std::filesystem::path& dirPath)
{
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(dirPath))
        if (entry.is_regular_file())
            try
            {
                loadPluginFromPath(entry.path());
            }
            catch (std::exception& e)
            {
                continue;
            }
}

void
PluginLoader::loadPluginsFromExecutableDir()
{
    loadPluginsFromDirPath(getExecutableDir());
}

PluginLoader::Iterator::Iterator(const PluginLoader& owner, size_t index)
    : m_owner(&owner)
    , m_index(index)
{}

PluginLoader::Iterator&
PluginLoader::Iterator::operator++()
{
    m_index++;
    return *this;
}

Plugin&
PluginLoader::Iterator::operator*() const
{
    return m_owner->m_pimpl->m_plugins[m_index];
}

bool
PluginLoader::Iterator::operator==(const PluginLoader::Iterator& other) const
{
    return (m_owner == other.m_owner) && (m_index == other.m_index);
}

bool
PluginLoader::Iterator::operator!=(const PluginLoader::Iterator& other) const
{
    return !(*this == other);
}

PluginLoader::Iterator
PluginLoader::begin() const
{
    return PluginLoader::Iterator(*this, 0);
}

PluginLoader::Iterator
PluginLoader::end() const
{
    return PluginLoader::Iterator(*this, m_pimpl->m_plugins.size());
}

const Plugin*
PluginLoader::getPluginByName(const std::wstring& name) const
{
    for (const Plugin& plugin : *this)
    {
        if (plugin.getName() == name)
        {
            return &plugin;
        }
    }

    return nullptr;
}

Plugin*
PluginLoader::getPluginByName(const std::wstring& name)
{
    return const_cast<Plugin*>(static_cast<const PluginLoader*>(this)->getPluginByName(name));
}

const ResourcesContainer&
PluginLoader::resources() const
{
    return m_pimpl->m_resourcesContainer;
}

std::filesystem::path
PluginLoader::getExecutableDir()
{
#ifdef _WIN32
    #define MAX_PATH_LENGTH 256

    wchar_t buffer[MAX_PATH_LENGTH];
    DWORD length = GetModuleFileNameW(nullptr, buffer, MAX_PATH_LENGTH);

    if (length == 0) {
        throw std::runtime_error("GetModuleFileNameW failed");
    }

    return std::filesystem::path(buffer).parent_path();
#else
    return std::filesystem::read_symlink("/proc/self/exe").parent_path();
#endif
}
