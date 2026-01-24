#pragma once

#include <pluginloader/api.h>

#include <string>

class Plugin;
class PluginLoaderPimpl;
class ResourcesContainer;

namespace std::filesystem
{
    class path;
}

class MY_NEURAL_NETWORK_LIB__PLUGINLOADER__API PluginLoader
{
public:
    PluginLoader();
    PluginLoader(const PluginLoader& other) = delete;
    PluginLoader(PluginLoader&& other) = delete;
    ~PluginLoader();

    PluginLoader& operator=(const PluginLoader& other) = delete;
    PluginLoader& operator=(PluginLoader&& other) = delete;

    void loadPluginFromPath(const std::filesystem::path& libraryPath);
    void loadPluginsFromDirPath(const std::filesystem::path& dirPath);
    void loadPluginsFromExecutableDir();

    class MY_NEURAL_NETWORK_LIB__PLUGINLOADER__API Iterator
    {
    public:
        Iterator(const PluginLoader& owner, const size_t& index);

        Iterator& operator++();
        Plugin& operator*() const;

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

    private:
        const PluginLoader* m_owner;
        size_t m_index;
    };

    Iterator begin() const;
    Iterator end() const;

    const Plugin* getPluginByName(const std::wstring& name) const;
    Plugin* getPluginByName(const std::wstring& name);

    const ResourcesContainer& resources() const;

    static std::filesystem::path getExecutableDir();

private:
    PluginLoaderPimpl* m_pimpl;

    friend class Iterator;
};
