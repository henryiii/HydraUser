cmake_minimum_required(VERSION 3.6)

project(HydraExample VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/cmake/README.md")
    message(FATAL_ERROR "You must get the submodule(s) also, use `git submodule update --init`")
endif
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(HYDRA_ROOT "../Hydra")

include(AddCLI)
include(AddRang)
include(AddHydra)

add_executable(HydraPhaseSpaceExperimental HydraPhaseSpaceExperimental.cpp)
target_link_libraries(HydraPhaseSpaceExperimental PUBLIC CLI Hydra_OMP rang)
