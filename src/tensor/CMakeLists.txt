set(TENSOR_SOURCES
    ${SOURCE_ROOT}/tensor/AbstractTensorIndex.cu
    ${SOURCE_ROOT}/tensor/ManagedMemorySharedPtr.cu
    ${SOURCE_ROOT}/tensor/TensorBase.cpp
)

add_library(tensor
    ${TENSOR_SOURCES}
)

target_include_directories(tensor
    PUBLIC
        ${SOURCE_ROOT}
)

target_link_libraries(tensor
    PUBLIC
        helpers
)

target_compile_definitions(tensor PRIVATE MY_NEURAL_NETWORK_LIB__TENSOR)

target_compile_options(tensor PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:/Zc:preprocessor>
    $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler /Zc:preprocessor>
)

install(TARGETS tensor)
