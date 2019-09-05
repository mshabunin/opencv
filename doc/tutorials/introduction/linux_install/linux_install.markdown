Installation in Linux {#tutorial_linux_install}
=====================

@next_tutorial{tutorial_linux_gcc_cmake}


@tableofcontents

Quick start {#tutorial_linux_install_quick_start}
===========

Build core modules {#tutorial_linux_install_quick_build_core}
------------------
```.sh
# Ubuntu 18.04 as reference
sudo apt update && apt install -y cmake g++ wget unzip
# Download sources
wget -O opencv.zip https://github.com/opencv/opencv/archive/master.zip
# Unpack sources
unzip opencv.zip
# Create build directory and switch into it
mkdir -p build && pushd build
# Configure
cmake  ../opencv-master
# Build
cmake --build .
# Return to parent directory
popd
```

Build with opencv_contrib {#tutorial_linux_install_quick_build_contrib}
-------------------------
```.sh
# Ubuntu 18.04 as reference
sudo apt update && apt install -y cmake g++ wget unzip
# Download sources
wget -O opencv.zip https://github.com/opencv/opencv/archive/master.zip
wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/master.zip
# Unpack sources
unzip opencv.zip
unzip opencv_contrib.zip
# Create build directory and switch into it
mkdir -p build && pushd build
# Configure
cmake -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib-master/modules ../opencv-master
# Build
cmake --build .
# Return to parent directory
popd
```

Detailed description {#tutorial_linux_install_detailed}
====================

Basic process {#tutorial_linux_install_detailed_basic}
-------------

In this section we described minimal requirements for building the OpenCV library. Sections after this one demonstrate optional dependencies and features.

### Download sources {#tutorial_linux_install_detailed_basic_download}

One can download snapshot of repository using web browser or any download tool:
```.sh
wget -O opencv.zip https://github.com/opencv/opencv/archive/master.zip
unzip opencv.zip
mv opencv-master opencv
```

Alternatively, it is possible to clone full repository to local machine using _git_ tool:
```.sh
git clone https://github.com/opencv/opencv.git
git -C opencv checkout master
```

@note
Links to snapshots from other branches, tags or commits can be found on the [GitHub](https://github.com/opencv/opencv) and [official download page](https://opencv.org/releases.html).

### Install compiler and build tools {#tutorial_linux_install_detailed_basic_compiler}

Install GCC...
```.sh
sudo apt install g++
```
or Clang:
```.sh
sudo apt install clang
```

Install cmake and make
```.sh
sudo apt install cmake make
```

### Configure and build {#tutorial_linux_install_detailed_basic_build}

Create build directory...
```.sh
mkdir build
cd build
```

configure...
```.sh
cmake ../opencv
```

and build.
```.sh
make
```

@note
In most cases, after installing new dependency build directory should be cleaned up (`rm -rf build`). Otherwise configuration process will not be able to find this dependency or can generate incorrect build scripts.


### Check build results {#tutorial_linux_install_detailed_basic_verify}

By default you can find built libraries in the `build/lib` directory and all executables (test, samples, apps) in the `build/bin` directory:
```.sh
ls bin
ls lib
```

cmake package files can be found in the build root:
```.sh
ls OpenCVConfig*.cmake
ls OpenCVModules.cmake
```

@note
The library can be used right after building even without installation. _cmake_ can find package files in the build directory by using the following environment variable:
```.sh
export OpenCV_DIR=<absolute-path-to-build>
```

    -   Installed as `opencv4`, usage: `pkg-config --cflags --libs opencv4`