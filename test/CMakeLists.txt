set(executables
        test_static_vector
)

include(GoogleTest)
foreach(exec ${executables})
    # - Define executables ------
    add_executable(${exec} ${exec}.cpp)

    # - Define include path ------------
    target_include_directories(${exec}        PRIVATE ${CMAKE_SOURCE_DIR}/include/)

    # - Link libraries ---------
    target_link_libraries(${exec} PRIVATE GTest::gtest_main)
    target_link_libraries(${exec} PRIVATE fmt::fmt)

    gtest_discover_tests(${exec})
endforeach()
