cmake_minimum_required(VERSION 3.20)

project(StaticVector LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# - Add general compile options --------------------------------------------------------------------
add_compile_options(-Wall -Wextra -pedantic -Wconversion -Wshadow)

# - Build type options -----------------------------------------------------------------------------
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address,undefined,leak -O0)
    add_link_options(-fsanitize=address,undefined,leak)
endif()

# - Check if `-march=native` is available ---------------------------------------------------------
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
    add_compile_options(-march=native)
endif()

include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        11.0.2
)
FetchContent_MakeAvailable(fmt)

option(SV_BUILD_TESTS "Builds unit test" ON)
if (SV_BUILD_TESTS)
    enable_testing()

    set(BUILD_GMOCK OFF)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        main
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    message(STATUS "Build tests")
    
    add_subdirectory(${CMAKE_SOURCE_DIR}/test/)
endif()
