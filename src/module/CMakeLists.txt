set(MODULE_SOURCES
    ${SOURCE_ROOT}/module/Module.cpp
)

add_library(module
    ${MODULE_SOURCES}
)

target_include_directories(module
    PUBLIC
        ${SOURCE_ROOT}
)

target_link_libraries(module
    PUBLIC
        helpers
        rtti
        tensor
)

target_compile_definitions(module PRIVATE MY_NEURAL_NETWORK_LIB__MODULE)

target_compile_options(module PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:/Zc:preprocessor>
    $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler /Zc:preprocessor>
)

install(TARGETS module)
