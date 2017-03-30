cmake_minimum_required(VERSION 3.8)

project(HydraExample VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Choose the type of build, options are: Debug Release"
        FORCE)
endif()


if(NOT EXISTS "${PROJECT_SOURCE_DIR}/cmake/README.md")
    message(FATAL_ERROR "You must get the submodule(s) also, use `git submodule update --init`")
endif()
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(HYDRA_ROOT "../Hydra")

include(AddCLI)
include(AddRang)
include(AddHydra)

add_executable(PS_CPP HydraPhaseSpaceExperimental.cpp)
target_link_libraries(PS_CPP PUBLIC CLI11 Hydra::CPP rang)

if(OPENMP_FOUND)
    add_executable(PS_OMP HydraPhaseSpaceExperimental.cpp)
    target_link_libraries(PS_OMP PUBLIC CLI11 Hydra::OMP rang)
endif()

if(TBB_FOUND)
    add_executable(PS_TBB HydraPhaseSpaceExperimental.cpp)
    target_link_libraries(PS_TBB PUBLIC CLI11 Hydra::TBB rang)
endif()

if(CUDA_FOUND)
    enable_language(CUDA)
    configure_file(HydraPhaseSpaceExperimental.cpp HydraPhaseSpaceExperimental.cu)
    add_executable(PS_CUDA HydraPhaseSpaceExperimental.cu)
    target_link_libraries(PS_CUDA PUBLIC CLI11 Hydra::CUDA rang)
endif()

