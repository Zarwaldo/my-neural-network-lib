add_library(helpers INTERFACE)

target_include_directories(helpers INTERFACE
    INTERFACE_INCLUDE_DIRECTORIES
        ${SOURCE_ROOT}
    $<BUILD_INTERFACE:${SOURCE_ROOT}>
    $<INSTALL_INTERFACE:.>
)

install(TARGETS helpers)
