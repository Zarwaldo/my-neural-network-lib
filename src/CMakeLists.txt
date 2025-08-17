set(SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(MY_NEURAL_NETWORK_LIB_SOURCES
    ${SOURCE_ROOT}/MyNeuralNetworkLib.cpp
)

set(MY_NEURAL_NETWORK_LIB_PUBLIC_HEADERS
    MyNeuralNetworkLib.h
    MyNeuralNetworkLibApi.h
)

add_library(my-neural-network-lib
    ${MY_NEURAL_NETWORK_LIB_SOURCES}
)

target_include_directories(my-neural-network-lib
    PUBLIC
        ${SOURCE_ROOT}
)

target_compile_definitions(my-neural-network-lib PRIVATE MY_NEURAL_NETWORK_LIB)

set_target_properties(my-neural-network-lib PROPERTIES
    PUBLIC_HEADER
        "${MY_NEURAL_NETWORK_LIB_PUBLIC_HEADERS}"
)

install(TARGETS my-neural-network-lib)
