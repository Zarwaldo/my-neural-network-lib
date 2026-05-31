set(NATIVEMODULES_SOURCES
    ${SOURCE_ROOT}/plugins/nativemodules/PluginHeader.cpp
    ${SOURCE_ROOT}/plugins/nativemodules/AdditionModule.cpp
    ${SOURCE_ROOT}/plugins/nativemodules/AdditionModuleImpl.cu
    ${SOURCE_ROOT}/plugins/nativemodules/PerceptronModule.cu
)

add_library(nativemodules
    ${NATIVEMODULES_SOURCES}
)

target_include_directories(nativemodules
    PUBLIC
        ${SOURCE_ROOT}
)

target_link_libraries(nativemodules
    PUBLIC
        plugincore
)

target_compile_definitions(nativemodules PRIVATE MY_NEURAL_NETWORK_LIB__NATIVE_MODULES)

target_compile_options(nativemodules PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:/Zc:preprocessor>
    $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler /Zc:preprocessor>
)

install(TARGETS nativemodules)
