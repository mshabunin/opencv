Installation in Linux {#tutorial_linux_install}
=====================

@next_tutorial{tutorial_linux_gcc_cmake}


@tableofcontents

# Quick start {#tutorial_linux_install_quick_start}


## Build core modules {#tutorial_linux_install_quick_build_core}

@include linux_quick_install.sh


## Build with opencv_contrib {#tutorial_linux_install_quick_build_contrib}

@include linux_quick_install_contrib.sh


# Detailed process {#tutorial_linux_install_detailed}

This section covers more details of the build process and describes alternative methods or tools to use. Please refer to the @ref tutorial_general_install tutorial for general installation details and configuration options documentation.


## Install compiler and build tools {#tutorial_linux_install_detailed_basic_compiler}

- To compile OpenCV you will need a C++ compiler. Usually it is G++/GCC or Clang/LLVM:
    - Install GCC...
    @snippet linux_install_a.sh gcc
    - ...or Clang:
    @snippet linux_install_b.sh clang

- OpenCV uses CMake build configuration tool:
@snippet linux_install_a.sh cmake

- CMake can generate scripts for different build systems, e.g. _make_, _ninja_:

    - Install Make...
    @snippet linux_install_a.sh make
    - ... or Ninja:
    @snippet linux_install_b.sh ninja

- Install tool for getting and unpacking sources:

    - _wget_ and _unzip_...
    @snippet linux_install_a.sh wget
    - ... or _git_:
    @snippet linux_install_b.sh git


## Download sources {#tutorial_linux_install_detailed_basic_download}

There are two methods of getting OpenCV sources:

- Download snapshot of repository using web browser or any download tool (~80-90Mb)...
@snippet linux_install_a.sh download
- ... or clone repository to local machine using _git_ to get full change history (>470Mb):
@snippet linux_install_b.sh download


@note
Links to snapshots of other branches, releases or commits can be found on the [GitHub](https://github.com/opencv/opencv) and [official download page](https://opencv.org/releases.html).


## Configure and build {#tutorial_linux_install_detailed_basic_build}

- Create build directory:
@snippet linux_install_a.sh prepare

- Configure - generate build scripts for preferred build system:
    - For _make_...
    @snippet linux_install_a.sh configure
    - ... or for _ninja_:
    @snippet linux_install_b.sh configure

- Build - run actual compilation process:
    - Using _make_...
    @snippet linux_install_a.sh build
    - ...or _ninja_:
    @snippet linux_install_b.sh build


@note
If you experience problems with the build process try to clean or recreate build directory. Changes in configuration like disabling a dependency, modifying build scripts or switching sources to another branch are not handled very well and can result in broken workspace.

@note
Refer to the @ref tutorial_general_install tutorial for full configuration options reference.

@note
Make can run multiple compilation processes in parallel, `-j<NUM>` option means "run `<NUM>` jobs simultaneously". Ninja will automatically detect number of available processor cores and does not need `-j` option.


## Check build results {#tutorial_linux_install_detailed_basic_verify}

After successful build you will find libraries in the `build/lib` directory and executables (test, samples, apps) in the `build/bin` directory:
@snippet linux_install_a.sh check

CMake package files will be located in the build root:
@snippet linux_install_a.sh check cmake


## Install to the system

@note
This step is optional, OpenCV can be used from build directory or installed to local user-owned directory.

    -   Installed as `opencv4`, usage: `pkg-config --cflags --libs opencv4`