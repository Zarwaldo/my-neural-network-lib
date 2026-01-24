#pragma once

#include <pluginloader/api.h>

#include <CommonMacros.h>

#include <string>

class PluginPimpl;

class ResourcesContainer;

namespace std::filesystem
{
    class path;
}

class MY_NEURAL_NETWORK_LIB__PLUGINLOADER__API Plugin
{
public:
    Plugin(const std::wstring& name, const std::filesystem::path& absoluteLibraryPath, ResourcesContainer& resourcesContainer);
    Plugin(const Plugin& other) = delete;
    Plugin(Plugin&& other);

    ~Plugin();

    Plugin& operator=(const Plugin& other) = delete;
    Plugin& operator=(Plugin&& other);

    void load();
    void unload();

    bool isLoaded() const;
    const std::wstring& getName() const;
    const std::filesystem::path& getAbsoluteLibraryPath() const;

private:
    static void* loadLibrary(const std::filesystem::path& libraryPath);
    static void* getLibrarySymbol(void* handle, const std::string& symbol);

    static void freeLibrary(void* handle);

    PluginPimpl* m_pimpl;
};
