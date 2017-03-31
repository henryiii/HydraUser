cmake_minimum_required(VERSION 3.8)

project(HydraExample VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Choose the type of build, options are: Debug Release"
        FORCE)
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/cmake/README.md")
    message(FATAL_ERROR "You must get the submodule(s) also, use `git submodule update --init` or use `--recursive` when you clone this repository.")
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include(AddCLI)
include(AddRang)
include(AddHydra)


hydra_add_executable(PS src/HydraPhaseSpaceExperimental.cpp)
target_link_libraries(PS INTERFACE CLI11 rang)
