set(PLUGINCORE_SOURCES
    ${SOURCE_ROOT}/plugincore/ResourcesContainer.cpp
    ${SOURCE_ROOT}/plugincore/RttiHolders.cpp
)

add_library(plugincore
    ${PLUGINCORE_SOURCES}
)

target_include_directories(plugincore
    PUBLIC
        ${SOURCE_ROOT}
)

target_link_libraries(plugincore
    PUBLIC
        rtti
        module
)

target_compile_definitions(plugincore PRIVATE MY_NEURAL_NETWORK_LIB__PLUGINCORE)

target_compile_options(plugincore PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:/Zc:preprocessor>
    $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler /Zc:preprocessor>
)

install(TARGETS plugincore)
