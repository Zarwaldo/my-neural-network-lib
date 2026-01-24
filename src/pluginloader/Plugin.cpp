#include <pluginloader/Plugin.h>

#include <plugincore/ResourcesContainer.h>

#include <filesystem>
#include <functional>
#include <stdexcept>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

class PluginPimpl
{
public:
    std::wstring m_name;
    std::filesystem::path m_absoluteLibraryPath;
    ResourcesContainer* m_resourcesContainer;
    void* m_handle;
    std::function<void(ResourcesContainer*)> m_open;
    std::function<void()> m_close;
};

Plugin::Plugin(const std::wstring& name, const std::filesystem::path& absoluteLibraryPath, ResourcesContainer& resourcesContainer)
    : m_pimpl(new PluginPimpl)
{
    m_pimpl->m_name = name;
    m_pimpl->m_absoluteLibraryPath = absoluteLibraryPath;
    m_pimpl->m_resourcesContainer = &resourcesContainer;
    m_pimpl->m_handle = nullptr;
    m_pimpl->m_open = nullptr;
    m_pimpl->m_close = nullptr;

    load();
}

Plugin::Plugin(Plugin&& other)
    : m_pimpl(other.m_pimpl)
{
    other.m_pimpl = nullptr;
}

Plugin::~Plugin()
{
    if (!m_pimpl)
        return;

    if (isLoaded())
        unload();

    delete m_pimpl;
}

Plugin&
Plugin::operator=(Plugin&& other)
{
    if (isLoaded())
        unload();

    delete m_pimpl;

    m_pimpl = other.m_pimpl;
    other.m_pimpl = nullptr;

    return *this;
}

void
Plugin::load()
{
    if (isLoaded())
        throw std::runtime_error("Plugin: Already loaded.");

    m_pimpl->m_handle = loadLibrary(m_pimpl->m_absoluteLibraryPath);
    if (!m_pimpl->m_handle)
        throw std::runtime_error("Plugin: File not found.");

    m_pimpl->m_open = reinterpret_cast<void (*)(ResourcesContainer*)>(
        getLibrarySymbol(m_pimpl->m_handle, "openNeuralNetworkPlugin")
    );
    m_pimpl->m_close = reinterpret_cast<void (*)()>(
        getLibrarySymbol(m_pimpl->m_handle, "closeNeuralNetworkPlugin")
    );

    if (!m_pimpl->m_open || !m_pimpl->m_close) {
        freeLibrary(m_pimpl->m_handle);
        throw std::runtime_error("Plugin: Loaded file was not a plugin library.");
    }

    m_pimpl->m_open(m_pimpl->m_resourcesContainer);
}

void
Plugin::unload()
{
    if (!isLoaded())
        throw std::runtime_error("Plugin: Already unloaded.");

    m_pimpl->m_close();
    freeLibrary(m_pimpl->m_handle);

    m_pimpl->m_handle = nullptr;
    m_pimpl->m_open = nullptr;
    m_pimpl->m_close = nullptr;
}

bool
Plugin::isLoaded() const
{
    return m_pimpl->m_handle != nullptr;
}

const std::wstring&
Plugin::getName() const
{
    return m_pimpl->m_name;
}

const std::filesystem::path&
Plugin::getAbsoluteLibraryPath() const
{
    return m_pimpl->m_absoluteLibraryPath;
}

void*
Plugin::loadLibrary(const std::filesystem::path& libraryPath)
{
#ifdef _WIN32
    return reinterpret_cast<void*>(LoadLibraryW(libraryPath.c_str()));
#else
    return dlopen(libraryPath.c_str(), RTLD_NOW);
#endif
}

void*
Plugin::getLibrarySymbol(void* handle, const std::string& symbol)
{
#ifdef _WIN32
    return reinterpret_cast<void*>(GetProcAddress(reinterpret_cast<HMODULE>(handle), symbol.c_str()));
#else
    return dlsym(handle, symbol.c_str());
#endif
}

void
Plugin::freeLibrary(void* handle)
{
#ifdef _WIN32
    FreeLibrary(reinterpret_cast<HMODULE>(handle));
#else
    dlclose(handle);
#endif
}
