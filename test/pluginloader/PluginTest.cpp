#include <gtest/gtest.h>

#include <pluginloader/Plugin.h>

#include <plugincore/ResourcesContainer.h>

#include <pluginloader/PluginLoader.h>

#include <filesystem>

class PluginShould : public ::testing::Test
{};

TEST_F(PluginShould, throwAnExceptionIfTheLibraryDoesNotExist) {
    // Given a resources container and a library path to a non-existing library
    ResourcesContainer resourcesContainer;
    const std::filesystem::path libPath = PluginLoader::getExecutableDir() / "nonexistinglibrary.dll";

    // When instanciating the plugin
    // Then the call throws an exception
    EXPECT_THROW({ Plugin(L"nonexistingplugin", libPath, resourcesContainer); }, std::runtime_error);
}

TEST_F(PluginShould, throwAnExceptionIfTheInputPathIsNotANeuralNetworkPluginLibrary) {
    // Given a resources container and a library path to a library that does not contain a neural network plugin
    ResourcesContainer resourcesContainer;
    const std::filesystem::path libPath = PluginLoader::getExecutableDir() / "tensor.dll";

    // When instanciating the plugin
    // Then the call throws an exception
    EXPECT_THROW({ Plugin(L"tensor", libPath, resourcesContainer); }, std::runtime_error);
}

TEST_F(PluginShould, notThrowAnExceptionIfTheInputPathIsANeuralNetworkPluginLibrary) {
    // Given a resources container and a library path to a non-existing library
    ResourcesContainer resourcesContainer;
    const std::filesystem::path libPath = PluginLoader::getExecutableDir() / "natives.dll";

    // When instanciating the plugin
    Plugin(L"natives", libPath, resourcesContainer);

    // Then the call does not throw an exception
}

TEST_F(PluginShould, haveTheCorrectName) {
    // Given a plugin
    ResourcesContainer resourcesContainer;
    const std::filesystem::path libPath = PluginLoader::getExecutableDir() / "natives.dll";
    const Plugin plugin = Plugin(L"natives", libPath, resourcesContainer);

    // When getting its name
    const std::wstring result = plugin.getName();

    // Then the returned name is the one passed to constructor
    EXPECT_EQ(result, L"natives");
}

TEST_F(PluginShould, haveTheCorrectLibraryPath) {
    // Given a plugin
    ResourcesContainer resourcesContainer;
    const std::filesystem::path libPath = PluginLoader::getExecutableDir() / "natives.dll";
    const Plugin plugin = Plugin(L"natives", libPath, resourcesContainer);

    // When getting its library path
    const std::filesystem::path result = plugin.getAbsoluteLibraryPath();

    // Then the returned name is the one passed to constructor
    EXPECT_EQ(result, libPath);
}

TEST_F(PluginShould, beLoadedByDefault) {
    // Given a plugin
    ResourcesContainer resourcesContainer;
    const std::filesystem::path libPath = PluginLoader::getExecutableDir() / "natives.dll";
    const Plugin plugin = Plugin(L"natives", libPath, resourcesContainer);

    // When checking if it is loaded
    const bool result = plugin.isLoaded();

    // Then the plugin should be loaded
    EXPECT_TRUE(result);
}

TEST_F(PluginShould, notBeLoadedIfUnloadWasCalled) {
    // Given a plugin
    ResourcesContainer resourcesContainer;
    const std::filesystem::path libPath = PluginLoader::getExecutableDir() / "natives.dll";
    Plugin plugin = Plugin(L"natives", libPath, resourcesContainer);

    // When unloading it
    plugin.unload();

    // Then the plugin should not be loaded
    EXPECT_FALSE(plugin.isLoaded());
}

TEST_F(PluginShould, notBeLoadedIfLoadWasCalled) {
    // Given an unloaded plugin
    ResourcesContainer resourcesContainer;
    const std::filesystem::path libPath = PluginLoader::getExecutableDir() / "natives.dll";
    Plugin plugin = Plugin(L"natives", libPath, resourcesContainer);
    plugin.unload();

    // When loading it
    plugin.load();

    // Then the plugin should be loaded
    EXPECT_TRUE(plugin.isLoaded());
}

TEST_F(PluginShould, provideRttisInTheDefaultCase) {
    // Given a plugin
    ResourcesContainer resourcesContainer;
    const std::filesystem::path libPath = PluginLoader::getExecutableDir() / "natives.dll";
    Plugin plugin = Plugin(L"natives", libPath, resourcesContainer);

    // When fetching an rtti from its resources
    const AbstractTemplateRtti<Module<float>>* result = resourcesContainer.getRttiHolder<Module<float>>().getTemplateRttiByName("AdditionModule<float>");

    // Then the returned rtti should be non-null
    EXPECT_NE(result, nullptr);
}

TEST_F(PluginShould, provideRttisWhenUnloaded) {
    // Given an unloaded plugin
    ResourcesContainer resourcesContainer;
    const std::filesystem::path libPath = PluginLoader::getExecutableDir() / "natives.dll";
    Plugin plugin = Plugin(L"natives", libPath, resourcesContainer);
    plugin.unload();

    // When fetching an rtti from its resources
    const AbstractTemplateRtti<Module<float>>* result = resourcesContainer.getRttiHolder<Module<float>>().getTemplateRttiByName("AdditionModule<float>");

    // Then the returned rtti should be null
    EXPECT_EQ(result, nullptr);
}

TEST_F(PluginShould, provideRttisWhenReloaded) {
    // Given a reloaded plugin
    ResourcesContainer resourcesContainer;
    const std::filesystem::path libPath = PluginLoader::getExecutableDir() / "natives.dll";
    Plugin plugin = Plugin(L"natives", libPath, resourcesContainer);
    plugin.unload();
    plugin.load();

    // When fetching an rtti from its resources
    const AbstractTemplateRtti<Module<float>>* result = resourcesContainer.getRttiHolder<Module<float>>().getTemplateRttiByName("AdditionModule<float>");

    // Then the returned rtti should be non-null
    EXPECT_NE(result, nullptr);
}
