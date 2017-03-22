cmake_minimum_required(VERSION 3.8)

project(HydraExample VERSION 0.2 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(HYDRA_ROOT "../Hydra")

include(AddCLI)
include(AddHydra)

add_executable(HydraPhaseSpaceExperimental HydraPhaseSpaceExperimental.cpp)
target_link_libraries(HydraPhaseSpaceExperimental PUBLIC CLI Hydra_CPP)
