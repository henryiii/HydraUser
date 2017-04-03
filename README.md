# HydraUser

Get and run a Hydra application. This uses CMake 3.8 (rc)'s new CUDA as a first class language support to provide a great interface for making your own Hydra applications. To get CMake 3.8 rc4:

```bash
mkdir cmake && wget -qO- "https://cmake.org/files/v3.8/cmake-3.8.0-rc4-Linux-x86_64.tar.gz" | tar --strip-components=1 -xz -C cmake
export PATH=`pwd`/cmake/bin:$PATH
```

## Writing your own Hydra applications

To use AddHydra, just include it. If you need to use a development branch, you can set `HYDRA_URL` and `HYDRA_TAG`. If you have a local copy, you can set `hydra_SOURCE_DIR` to skip the download.
```cmake
set(HYDRA_URL "https://github.com/henryiii/Hydra.git" CACHE STRING "")
set(HYDRA_TAG "origin/dev_fixes" CACHE STRING "")
include(AddHydra)
```

To use, you can use the helper macro to conditionally add up to four targets for the four supported platforms:
```
hydra_add_executable(PS src/HydraPhaseSpaceExperimental.cpp)
```

This creates `PS_CPP`, `PS_TBB`, `PS_OMP`, and/or `PS_CUDA`, depending on your platform. You can also manually create any targets you want, and link with `Hydra::CPP`, etc. (For CUDA, you'll also need to set the language on your file to CUDA and use `enable_language(CUDA)`. `hydra_add_executable` makes build-tree copies with `.cu` extensions.)

If you have more libraries to link to, just add to the parent target:

```cmake
target_link_libraries(PS INTERFACE more_libs)
```

## Examples included

* HydraPhaseSpaceExample: A test application for the new phase space generator.


