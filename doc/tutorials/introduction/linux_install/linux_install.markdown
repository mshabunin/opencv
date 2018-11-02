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


Advanced build features {#tutorial_linux_install_detailed_advanced}
-----------------------

OpenCV library has modular structure where each module can depend on other modules and 3rdparty libraries. There are also exist several features allowing to modify build process and produce binaries in different form.

### Debug build {#tutorial_linux_install_detailed_advanced_debug}

Binaries will contain debug symbols (`-g`) and all optimizations will be turned off (`-o0`). Add the following option to _cmake_:
```.sh
cmake -DCMAKE_BUILD_TYPE=Debug ../opencv
```

### Static build {#tutorial_linux_install_detailed_advanced_static}

Binaries will not be linked to dynamic libraries but packed into `.a` archives.
It is controlled by _cmake_ option:
```.sh
cmake -DBUILD_SHARED_LIBS=OFF ../opencv
```
Read more here: https://en.wikipedia.org/wiki/Static_library


### Generate pkg-config info
-   Add this flag when running CMake: `-DOPENCV_GENERATE_PKGCONFIG=ON`
-   Will generate the .pc file for pkg-config and install it.
-   Useful if not using CMake in projects that use OpenCV
    -   Installed as `opencv4`, usage: `pkg-config --cflags --libs opencv4`


### Build on multiple cores {#tutorial_linux_install_detailed_advanced_cores}

The _make_ tool allows to build several files simultaneously:
```.sh
make -j4
```
Where `4` is the number of compilation processes you want to run in parallel.

### Install to specific directory {#tutorial_linux_install_detailed_advanced_install}

To install produced binaries root location should be configured. Default value depends on distribution, in Ubuntu it is usually set to `/usr/local`. It can be changed during configuration:
```.sh
cmake -DCMAKE_INSTALL_PREFIX=/opt/opencv ../opencv
```
This path can be relative to current working directory, in the following example it will be set to `<absolute-path-to-build>/install`:
```.sh
cmake -DCMAKE_INSTALL_PREFIX=install ../opencv
```

After building the library with _make_ all files can be copied to the configured install location using the following command:
```.sh
make install
```

To install binaries to the system location (e.g. `/usr/local`) as a regular user it is necessary to run the previous command with elevated privileges:
```.sh
sudo make install
```

@note
It is possible to remove symbol information during install. Binaries will become 10-15% smaller but debugging ability will be limited:
```.sh
make install/strip
```

### Build tests, samples and applications {#tutorial_linux_install_detailed_advanced_tests}

There are two kinds of tests: accuracy (`opencv_test_*`) and performance (`opencv_perf_*`).Tests and applications are enabled by default. Examples are not being built by default and should be enabled explicitly.

Corresponding _cmake_ options:
```.sh
cmake \
  -DBUILD_TESTS=ON \
  -DBUILD_PERF_TESTS=ON \
  -DBUILD_EXAMPLES=ON \
  -DBUILD_opencv_apps=ON \
  ../opencv
```

### Build limited set of modules {#tutorial_linux_install_detailed_advanced_modules}

Each module is a subdirectory of the `modules` directory. It is possible to disable one module:
```.sh
cmake -DBUILD_opencv_calib3d=OFF ../opencv
```

The opposite option is to build only specified modules and all modules they depend on:
```.sh
cmake -DBUILD_LIST=calib3d,videoio,ts ../opencv
```
In this example we requested 3 modules and configuration script has determined all dependencies automatically:
```
--   OpenCV modules:
--     To be built:                 calib3d core features2d flann highgui imgcodecs imgproc ts videoio
```

### Build with extra modules (opencv_contrib) {#tutorial_linux_install_detailed_advanced_contrib}

It is possible to add more modules from external directories:
```.sh
cmake -DOPENCV_EXTRA_MODULES_PATH=../my_module ../opencv
```

[opencv_contrib](https://github.com/opencv/opencv_contrib) repository contains experimental modules, it can be downloaded from the GitHub:
```.sh
wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/master.zip
unzip opencv_contrib.zip
mv opencv_contrib-master opencv_contrib
```

All modules from this repository can be added to the compilation:
```.sh
cmake -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules ../opencv
```

Several locations can be added as a `;`-separated list (semicolon must be escaped in bash):
```.sh
cmake \
  -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules/bgsegm\;../opencv_contrib/modules/bioinspired \
  ../opencv
```

### Downloaded dependencies {#tutorial_linux_install_detailed_advanced_download}

Configuration script can try to download additional libraries and files from the internet, if it fails to do it corresponding features will be turned off. In some cases configuration error can occur. By default all files are first downloaded to the `<source>/.cache` directory and then unpacked or copied to the build directory. It is possible to change download cache location by setting environment variable or configuration option:
```.sh
export OPENCV_DOWNLOAD_PATH=/tmp/opencv-cache
cmake ../opencv
# or
cmake -DOPENCV_DOWNLOAD_PATH=/tmp/opencv-cache ../opencv
```

In case of access via proxy, corresponding environment variables should be set before running cmake:
```.sh
export http_proxy=<proxy-host>:<port>
export https_proxy=<proxy-host>:<port>
```

Full log of download process can be found in build directory - `CMakeDownloadLog.txt`. In addition, for each failed download a command will be added to helper scripts in the build directory, e.g. `download_with_wget.sh`. Users can run these scripts as is or modify according to their needs.


### CPU optimization level {#tutorial_linux_install_detailed_advanced_cpu}

On x86_64 machines the library will be compiled for SSE3 instruction set level by default. This level can be changed by configuration option:
```.sh
cmake -DCPU_BASELINE=SSE2 ../opencv
```

@note
Other platforms have their own instruction set levels: `VFPV3` and `NEON` on ARM, `VSX` on PowerPC.

Some functions support dispatch mechanism allowing to compile them for several instruction sets and to choose one during runtime. List of enabled instruction sets can be changed during configuration:
```.sh
cmake -DCPU_DISPATCH=AVX,AVX2 ../opencv
```
To disable dispatch mechanism this option should be set to an empty value:
```.sh
cmake -DCPU_DISPATCH= ../opencv
```

@note
More details on CPU optimization options can be found in wiki: https://github.com/opencv/opencv/wiki/CPU-optimizations-build-options

### Build documentation

Documentation can be built using the [Doxygen](http://www.doxygen.org/index.html) tool:
```.sh
sudo apt install doxygen
```
It should be enabled with configuration option:
```.sh
cmake -DBUILD_DOCS=ON ../opencv
```
More information about documentation building and writing can be found in @ref tutorial_documentation tutorial.

Functional features and dependencies
------------------------------------

There are many optional dependencies and features that can be turned on or off. _cmake_ has special option allowing to print all available configuration parameters:
```.sh
cmake -LH ../opencv
```

In this section we will cover most popular features available on Linux.

### Heterogeneous computation

|      Option     |   Description  |  Requirements  |
| :-------------- | :------------- | :------------- |
| `WITH_CUDA`     | enable [CUDA](https://en.wikipedia.org/wiki/CUDA) support. Implemented as separate modules located in opencv_contrib repository since version 4. | CUDA toolkit must be installed from the official NVIDIA site.
| `WITH_OPENCL`   | enable [OpenCL](https://en.wikipedia.org/wiki/OpenCL) support. Implemented in same modules as CPU. |  No additional dependencies required. |

### Image reading and writing (_imgcodecs_ module)
|      Option     |   Description  |  Requirements  |
| :-------------- | :------------- | :------------- |
| `WITH_PNG`      | enable PNG reading and writing | `libpng-dev` |
| `WITH_JPEG`     | enable JPEG reading and writing | `libjpeg-dev` or `libjpeg-turbo8-dev` |
| `WITH_TIFF`     | enable TIFF reading and writing | `libtiff-dev` |
| `WITH_WEBP`     | enable WebP images reading and writing | `libwebp-dev` |
| `WITH_GDAL`     | enable [GDAL](https://en.wikipedia.org/wiki/GDAL) backend (Geospatial Data Abstraction Library) | `libgdal-dev` |
| `WITH_GDCM`     | enable [DICOM](https://en.wikipedia.org/wiki/DICOM) medical images support via [GDCM  library](https://en.wikipedia.org/wiki/GDCM) | `libgdcm2-dev` |

### Video reading and writing (_videoio_ module)

|      Option     |   Description  |  Requirements  |
| :-------------- | :------------- | :------------- |
| `WITH_V4L`      | enable [Video4Linux](https://en.wikipedia.org/wiki/Video4Linux) backend for capturing frames from cameras | Linux kernel headers must be installed: `linux-kernel-headers` |
| `WITH_FFMPEG`   | enable [FFmpeg](https://en.wikipedia.org/wiki/FFmpeg) backend for decoding and encoding video files and network streams. This library can read and write many popular video formats | `libavcodec-dev libavformat-dev libswresample-dev libavutil-dev` |
| `WITH_GSTREAMER`| enable [GStreamer](https://en.wikipedia.org/wiki/GStreamer) backend for decoding and encoding video files, capturing frames from cameras and network streams. Numerous plugins should be installed to extend supported formats list. OpenCV allows running arbitrary pipelines provided as strings | `libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev` |

### Parallel execution (_core_ module)

By default OpenCV uses _pthreads_ parallel backend, but it can be changed with following options:

|      Option     |   Description  |  Requirements  |
| :-------------- | :------------- | :------------- |
| `WITH_TBB`      | enable [TBB](https://en.wikipedia.org/wiki/Threading_Building_Blocks) parallel backend |  `libtbb-dev` |
| `WITH_OPENMP`   | enable [OpenMP](https://en.wikipedia.org/wiki/OpenMP) parallel backend | must be supported by compiler |

### Window backend (_highgui_ module)

|      Option     |   Description  |  Requirements  |
| :-------------- | :------------- | :------------- |
| `WITH_GTK`      | enable GTK window backend | |
| `WITH_QT`       | enable Qt window backend | |
