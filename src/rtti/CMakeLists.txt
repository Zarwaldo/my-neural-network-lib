add_library(rtti INTERFACE)

target_include_directories(rtti INTERFACE
    INTERFACE_INCLUDE_DIRECTORIES
        ${SOURCE_ROOT}
    $<BUILD_INTERFACE:${SOURCE_ROOT}>
    $<INSTALL_INTERFACE:.>
)

target_link_libraries(rtti
    INTERFACE
        helpers
)

install(TARGETS rtti)
