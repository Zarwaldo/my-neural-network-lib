#include <gtest/gtest.h>

#include <pluginloader/PluginLoader.h>

#include <plugincore/ResourcesContainer.h>

#include <pluginloader/Plugin.h>

#include <filesystem>

class PluginLoaderShould : public ::testing::Test
{};

TEST_F(PluginLoaderShould, beAbleToLoadAPluginFromPath) {
    // Given a plugin loader and the path to a plugin library
    PluginLoader pluginLoader;
    const std::filesystem::path libPath = PluginLoader::getExecutableDir() / "nativemodules.dll";

    // When we load this plugin
    pluginLoader.loadPluginFromPath(libPath);

    // Then the plugin can be found in the plugin loader
    const Plugin* plugin = pluginLoader.getPluginByName(L"nativemodules");
    EXPECT_NE(plugin, nullptr);
    EXPECT_EQ(plugin->getName(), L"nativemodules");

    // And Rttis provided by the plugin can be retrieved
    const AbstractTemplateRtti<Module<float>>* moduleRtti = pluginLoader.resources().modules<float>().getTemplateRttiByName("AdditionModule<float>");
    EXPECT_NE(moduleRtti, nullptr);
}

TEST_F(PluginLoaderShould, beAbleToLoadPluginsFromTheExecutablesDirectory) {
    // Given a plugin loader
    PluginLoader pluginLoader;

    // When we load this plugin
    pluginLoader.loadPluginsFromExecutableDir();

    // Then the plugins can be found in the plugin loader
    const Plugin* nativeModulesPlugin = pluginLoader.getPluginByName(L"nativemodules");
    EXPECT_NE(nativeModulesPlugin, nullptr);
    EXPECT_EQ(nativeModulesPlugin->getName(), L"nativemodules");
}

TEST_F(PluginLoaderShould, beAbleToIterateOnTheLoadedPlugins) {
    // Given a plugin loader that has loaded all plugins from the executable's directory
    PluginLoader pluginLoader;
    pluginLoader.loadPluginsFromExecutableDir();

    // When we iterate on the plugin loader to fetch the loaded plugins' names
    std::vector<std::wstring> pluginsNames;
    for (Plugin& plugin : pluginLoader)
    {
        pluginsNames.push_back(plugin.getName());
    }

    // Then the list should contain all the plugins' names
    EXPECT_EQ(pluginsNames.size(), 1);
    EXPECT_EQ(pluginsNames[0], L"nativemodules");
}
