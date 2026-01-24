set(PLUGINLOADER_SOURCES
    ${SOURCE_ROOT}/pluginloader/PluginLoader.cpp
    ${SOURCE_ROOT}/pluginloader/Plugin.cpp
)

add_library(pluginloader
    ${PLUGINLOADER_SOURCES}
)

target_include_directories(pluginloader
    PUBLIC
        ${SOURCE_ROOT}
)

target_link_libraries(pluginloader
    PUBLIC
        plugincore
)

target_compile_definitions(pluginloader PRIVATE MY_NEURAL_NETWORK_LIB__PLUGINLOADER)

target_compile_options(pluginloader PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:/Zc:preprocessor>
    $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler /Zc:preprocessor>
)

install(TARGETS pluginloader)
