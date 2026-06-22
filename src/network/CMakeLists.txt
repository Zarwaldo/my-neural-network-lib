set(NETWORK_SOURCES
    ${SOURCE_ROOT}/network/Flow.cpp
    ${SOURCE_ROOT}/network/NeuralNetwork.cpp
    ${SOURCE_ROOT}/network/NullInputProvider.cpp
    ${SOURCE_ROOT}/network/NullOutputReceiver.cpp
    ${SOURCE_ROOT}/network/NullParamTensorFiller.cpp
    ${SOURCE_ROOT}/network/TrainingNeuralNetwork.cpp
)

add_library(network
    ${NETWORK_SOURCES}
)

target_include_directories(network
    PUBLIC
        ${SOURCE_ROOT}
)

target_link_libraries(network
    PUBLIC
        rtti
        module
)

target_compile_definitions(network PRIVATE MY_NEURAL_NETWORK_LIB__NETWORK)

target_compile_options(network PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:/Zc:preprocessor>
    $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler /Zc:preprocessor>
)

install(TARGETS network)
