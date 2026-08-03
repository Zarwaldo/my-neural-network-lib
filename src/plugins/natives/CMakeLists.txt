set(NATIVES_SOURCES
    ${SOURCE_ROOT}/plugins/natives/PluginHeader.cpp
    ${SOURCE_ROOT}/plugins/natives/modules/additionmodule/AdditionModule.cpp
    ${SOURCE_ROOT}/plugins/natives/modules/additionmodule/AdditionModuleHeader.cpp
    ${SOURCE_ROOT}/plugins/natives/modules/additionmodule/AdditionModuleImpl.cu
    ${SOURCE_ROOT}/plugins/natives/modules/euclidiandistancemodule/EuclidianDistanceModule.cpp
    ${SOURCE_ROOT}/plugins/natives/modules/euclidiandistancemodule/EuclidianDistanceModuleHeader.cpp
    ${SOURCE_ROOT}/plugins/natives/modules/euclidiandistancemodule/EuclidianDistanceModuleImpl.cu
    ${SOURCE_ROOT}/plugins/natives/modules/perceptronmodule/PerceptronModule.cpp
    ${SOURCE_ROOT}/plugins/natives/modules/perceptronmodule/PerceptronModuleHeader.cpp
    ${SOURCE_ROOT}/plugins/natives/modules/perceptronmodule/PerceptronModuleImpl.cu
    ${SOURCE_ROOT}/plugins/natives/networkbuilders/EuclidianDistanceCostBuilder.cpp
    ${SOURCE_ROOT}/plugins/natives/networkbuilders/NetworkBuildersHeader.cpp
    ${SOURCE_ROOT}/plugins/natives/tensormaps/TensorMapKeyEnums.cpp
    ${SOURCE_ROOT}/plugins/natives/tensormaps/TensorMapsHeader.cpp
)

add_library(natives
    ${NATIVES_SOURCES}
)

target_include_directories(natives
    PUBLIC
        ${SOURCE_ROOT}
)

target_link_libraries(natives
    PUBLIC
        plugincore
        network
)

target_compile_definitions(natives PRIVATE MY_NEURAL_NETWORK_LIB__NATIVE_MODULES)

target_compile_options(natives PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:/Zc:preprocessor>
    $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler /Zc:preprocessor>
)

install(TARGETS natives)
