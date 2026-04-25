find_package(GTest REQUIRED)

macro(add_neural_network_test NAME MAIN_FILE)
    add_executable(${NAME} ${MAIN_FILE})

    target_link_libraries(${NAME}
        PRIVATE
            GTest::GTest
            GTest::gmock
            GTest::Main
            ${ARGN}
    )

    target_compile_options(${NAME} PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:/Zc:preprocessor>
        $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler /Zc:preprocessor>
    )

    add_test(NAME ${NAME} COMMAND ${NAME})
endmacro()

add_subdirectory(helpers)
add_subdirectory(rtti)
add_subdirectory(tensor)
