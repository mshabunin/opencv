OpenCV configuration options reference {#tutorial_config_reference}
======================================

@tableofcontents


# Introduction {#tutorial_config_reference_intro}

@note
We assume you have read @ref tutorial_general_install tutorial or have experience with CMake.

Configuration options can be set in several different ways:
* Command line: `cmake -Doption=value ...`
* Initial cache files: `cmake -C my_options.txt ...`
* GUI

In this reference we will use regular command line.

Most of the options can be found in the root cmake script of OpenCV: `opencv/CMakeLists.txt`. Some options can be defined in specific modules.

It is possible to use CMake tool to print all available options:
```.sh
# initial configuration
cmake ../opencv

# print all options
cmake -L

# print all options with help message
cmake -LH

# print all options including advanced
cmake -LA
```

Most popular and useful are options starting with `WITH_`, `ENABLE_`, `BUILD_`, `OPENCV_`.

Default values vary depending on platform and other options values.


# General options {#tutorial_config_reference_general}

## Build with extra modules {#tutorial_config_reference_general_contrib}

`OPENCV_EXTRA_MODULES_PATH` option contains a semicolon-separated list of directories containing extra modules which will be added to the build. Module directory must have compatible layout and CMakeLists.txt, brief description can be found in the [Coding Style Guide](https://github.com/opencv/opencv/wiki/Coding_Style_Guide).

Examples:
```.sh
# build with all modules in opencv_contrib
cmake -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules ../opencv

# build with one of opencv_contrib modules
cmake -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules/bgsegm ../opencv

# build with two custom modules (semicolon must be escaped in bash)
cmake -DOPENCV_EXTRA_MODULES_PATH=../my_mod1\;../my_mod2 ../opencv
```

@note
Only 0- and 1-level deep module locations are supported, following command will raise an error:
```.sh
cmake -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib ../opencv
```


## Debug build {#tutorial_config_reference_general_debug}

`CMAKE_BUILD_TYPE` option can be used to enable debug build; resulting binaries will contain debug symbols and most of compiler optimizations will be turned off. To enable debug symbols in Release build turn the `BUILD_WITH_DEBUG_INFO` option on.

On some platforms (e.g. Linux) build type must be set at configuration stage:
```.sh
cmake -DCMAKE_BUILD_TYPE=Debug ../opencv
cmake --build .
```
On other platforms different types of build can be produced in the same build directory (e.g. Visual Studio, XCode):
```.sh
cmake <options> ../opencv
cmake --build . --config Debug
```

If you use GNU libstdc++ (default for GCC) you can turn on the `ENABLE_GNU_STL_DEBUG` option, then C++ library will be used in Debug mode, e.g. indexes will be bound-checked during vector element access.

Many kinds of optimizations can be disabled with :star:`CV_DISABLE_OPTIMIZATION` option:
* Some third-party libraries (e.g. IPP, Lapack, Eigen)
* Explicit vectorized implementation (universal intrinsics, raw intirinsics, etc.)
* Dispatched optimizations
* Explicit loop unrolling

@see https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html
@see https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_macros.html
@see https://github.com/opencv/opencv/wiki/CPU-optimizations-build-options


## Static build {#tutorial_config_reference_general_static}

`BUILD_SHARED_LIBS` option control whether to produce dynamic (.dll, .so, .dylib) or static (.a, .lib) libraries.

Example:
```.sh
cmake -DBUILD_SHARED_LIBS=OFF ../opencv
```

@see https://en.wikipedia.org/wiki/Static_library


`ENABLE_PIC` (CMAKE_POSITION_INDEPENDENT_CODE)

## Generate pkg-config info

`OPENCV_GENERATE_PKGCONFIG` option enables `.pc` file generation along with standard CMake package. This file can be useful for projects which do not use CMake for build.

Example:
```.sh
cmake -DOPENCV_GENERATE_PKGCONFIG=ON ../opencv
```

@note
Due to complexity of configuration process resulting `.pc` file can contain incomplete list of third-party dependencies and may not work in some configurations, especially for static builds. This feature is not officially supported since 4.x version and is disabled by default.


## Build tests, samples and applications {#tutorial_config_reference_general_tests}

There are two kinds of tests: accuracy (`opencv_test_*`) and performance (`opencv_perf_*`). Tests and applications are enabled by default. Examples are not being built by default and should be enabled explicitly.

Corresponding _cmake_ options:
```.sh
cmake \
  -DBUILD_TESTS=ON \
  -DBUILD_PERF_TESTS=ON \
  -DBUILD_EXAMPLES=ON \
  -DBUILD_opencv_apps=ON \
  ../opencv
```


## Build limited set of modules {#tutorial_config_reference_general_modules}

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


## Downloaded dependencies {#tutorial_config_reference_general_download}

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


## CPU optimization level {#tutorial_config_reference_general_cpu}

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

`CV_ENABLE_INTRINSICS`
`CV_DISABLE_OPTIMIZATION`

@note
More details on CPU optimization options can be found in wiki: https://github.com/opencv/opencv/wiki/CPU-optimizations-build-options

## Build documentation {#tutorial_config_reference_general_doc}

Documentation can be built using the [Doxygen](http://www.doxygen.org/index.html) tool:
```.sh
sudo apt install doxygen
```
It should be enabled with configuration option:
```.sh
cmake -DBUILD_DOCS=ON ../opencv
```
More information about documentation building and writing can be found in @ref tutorial_documentation tutorial.


# Functional features and dependencies {#tutorial_config_reference_func}

There are many optional dependencies and features that can be turned on or off. _cmake_ has special option allowing to print all available configuration parameters:
```.sh
cmake -LH ../opencv
```

In this section we will cover most popular features available on Linux.

## Heterogeneous computation {#tutorial_config_reference_func_hetero}

|      Option     |   Description  |  Requirements  |
| :-------------- | :------------- | :------------- |
| `WITH_CUDA`     | enable [CUDA](https://en.wikipedia.org/wiki/CUDA) support. Implemented as separate modules located in opencv_contrib repository since version 4. | CUDA toolkit must be installed from the official NVIDIA site.
| `WITH_OPENCL`   | enable [OpenCL](https://en.wikipedia.org/wiki/OpenCL) support. Implemented in same modules as CPU. |  No additional dependencies required. |

## Image reading and writing (imgcodecs module)  {#tutorial_config_reference_func_imgcodecs}
|      Option     |   Description  |  Requirements  |
| :-------------- | :------------- | :------------- |
| `WITH_PNG`      | enable PNG reading and writing | `libpng-dev` |
| `WITH_JPEG`     | enable JPEG reading and writing | `libjpeg-dev` or `libjpeg-turbo8-dev` |
| `WITH_TIFF`     | enable TIFF reading and writing | `libtiff-dev` |
| `WITH_WEBP`     | enable WebP images reading and writing | `libwebp-dev` |
| `WITH_GDAL`     | enable [GDAL](https://en.wikipedia.org/wiki/GDAL) backend (Geospatial Data Abstraction Library) | `libgdal-dev` |
| `WITH_GDCM`     | enable [DICOM](https://en.wikipedia.org/wiki/DICOM) medical images support via [GDCM  library](https://en.wikipedia.org/wiki/GDCM) | `libgdcm2-dev` |

## Video reading and writing (videoio module) {#tutorial_config_reference_func_videoio}

|      Option     |   Description  |  Requirements  |
| :-------------- | :------------- | :------------- |
| `WITH_V4L`      | enable [Video4Linux](https://en.wikipedia.org/wiki/Video4Linux) backend for capturing frames from cameras | Linux kernel headers must be installed: `linux-kernel-headers` |
| `WITH_FFMPEG`   | enable [FFmpeg](https://en.wikipedia.org/wiki/FFmpeg) backend for decoding and encoding video files and network streams. This library can read and write many popular video formats | `libavcodec-dev libavformat-dev libswresample-dev libavutil-dev` |
| `WITH_GSTREAMER`| enable [GStreamer](https://en.wikipedia.org/wiki/GStreamer) backend for decoding and encoding video files, capturing frames from cameras and network streams. Numerous plugins should be installed to extend supported formats list. OpenCV allows running arbitrary pipelines provided as strings | `libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev` |

## Parallel execution (core module) {#tutorial_config_reference_func_core}

By default OpenCV uses _pthreads_ parallel backend, but it can be changed with following options:

|      Option     |   Description  |  Requirements  |
| :-------------- | :------------- | :------------- |
| `WITH_TBB`      | enable [TBB](https://en.wikipedia.org/wiki/Threading_Building_Blocks) parallel backend |  `libtbb-dev` |
| `WITH_OPENMP`   | enable [OpenMP](https://en.wikipedia.org/wiki/OpenMP) parallel backend | must be supported by compiler |

## Window backend (highgui module) {#tutorial_config_reference_highgui}

|      Option     |   Description  |  Requirements  |
| :-------------- | :------------- | :------------- |
| `WITH_GTK`      | enable GTK window backend | |
| `WITH_QT`       | enable Qt window backend | |



`WITH_1394`
`WITH_AVFOUNDATION`
`WITH_CAP_IOS`
`WITH_CAROTENE`
`WITH_CPUFEATURES`
`WITH_VTK`
`WITH_CUDA`
`WITH_CUFFT`
`WITH_CUBLAS`
`WITH_CUDNN`
`WITH_NVCUVID`
`WITH_EIGEN`
`WITH_FFMPEG`
`WITH_GSTREAMER`
`WITH_GTK`
`WITH_GTK_2_X`
`WITH_IPP`
`WITH_HALIDE`
`WITH_VULKAN`
`WITH_INF_ENGINE`
`WITH_JASPER`
`WITH_JPEG`
`WITH_WEBP`
`WITH_OPENEXR`
`WITH_OPENGL`
`WITH_OPENVX`
`WITH_OPENNI`
`WITH_OPENNI2`
`WITH_PNG`
`WITH_GDCM`
`WITH_PVAPI`
`WITH_ARAVIS`
`WITH_QT`
`WITH_WIN32UI`
`WITH_TBB`
`WITH_HPX`
`WITH_OPENMP`
`WITH_PTHREADS_PF`
`WITH_TIFF`
`WITH_V4L`
`WITH_DSHOW`
`WITH_MSMF`
`WITH_MSMF_DXVA`
`WITH_XIMEA`
`WITH_XINE`
`WITH_CLP`
`WITH_OPENCL`
`WITH_OPENCL_SVM`
`WITH_OPENCLAMDFFT`
`WITH_OPENCLAMDBLAS`
`WITH_DIRECTX`
`WITH_OPENCL_D3D11_NV`
`WITH_LIBREALSENSE`
`WITH_VA`
`WITH_VA_INTEL`
`WITH_MFX`
`WITH_GDAL`
`WITH_GPHOTO2`
`WITH_LAPACK`
`WITH_ITT`
`WITH_PROTOBUF`
`WITH_IMGCODEC_HDR`
`WITH_IMGCODEC_SUNRASTER`
`WITH_IMGCODEC_PXM`
`WITH_IMGCODEC_PFM`
`WITH_QUIRC`
`WITH_ANDROID_MEDIANDK`


# Installation layout {#tutorial_config_reference_install}

`INSTALL_CREATE_DISTRIB`
`INSTALL_C_EXAMPLES`
`INSTALL_PYTHON_EXAMPLES`
`INSTALL_ANDROID_EXAMPLES`
`INSTALL_TO_MANGLED_PATHS`
`INSTALL_TESTS`

## Installation root {#tutorial_config_reference_install_root}

To install produced binaries root location should be configured. Default value depends on distribution, in Ubuntu it is usually set to `/usr/local`. It can be changed during configuration:
```.sh
cmake -DCMAKE_INSTALL_PREFIX=/opt/opencv ../opencv
```
This path can be relative to current working directory, in the following example it will be set to `<absolute-path-to-build>/install`:
```.sh
cmake -DCMAKE_INSTALL_PREFIX=install ../opencv
```

After building the library, all files can be copied to the configured install location using the following command:
```.sh
cmake --build . --target install
```

To install binaries to the system location (e.g. `/usr/local`) as a regular user it is necessary to run the previous command with elevated privileges:
```.sh
sudo cmake --build . --target install
```

@note
On some platforms (Linux) it is possible to remove symbol information during install. Binaries will become 10-15% smaller but debugging ability will be limited:
```.sh
cmake --build . --target install/strip
```


## Component locations {#tutorial_config_reference_install_comp}

OPENCV_BIN_INSTALL_PATH
OPENCV_TEST_INSTALL_PATH
OPENCV_SAMPLES_BIN_INSTALL_PATH
OPENCV_LIB_INSTALL_PATH
OPENCV_LIB_ARCHIVE_INSTALL_PATH
OPENCV_3P_LIB_INSTALL_PATH
OPENCV_CONFIG_INSTALL_PATH
OPENCV_INCLUDE_INSTALL_PATH
OPENCV_OTHER_INSTALL_PATH
OPENCV_SAMPLES_SRC_INSTALL_PATH
OPENCV_LICENSES_INSTALL_PATH
OPENCV_TEST_DATA_INSTALL_PATH
OPENCV_DOC_INSTALL_PATH
OPENCV_JAR_INSTALL_PATH
OPENCV_JNI_INSTALL_PATH
OPENCV_JNI_BIN_INSTALL_PATH

INSTALL_TO_MANGLED_PATHS



# Miscellaneous features {#tutorial_config_reference_misc}


## Non-free (patented) algorithms {#tutorial_config_reference_misc_nonfree}

`OPENCV_ENABLE_NONFREE`


## Building own 3rdparty libraries

`OPENCV_FORCE_3RDPARTY_BUILD`
`BUILD_ZLIB`
`BUILD_TIFF`
`BUILD_JASPER`
`BUILD_JPEG`
`BUILD_PNG`
`BUILD_OPENEXR`
`BUILD_WEBP`
`BUILD_TBB`
`BUILD_IPP_IW`
`BUILD_ITT`


## CCache and PCH

`ENABLE_CCACHE`
`ENABLE_PRECOMPILED_HEADERS`


## Python/Java

`ENABLE_PYLINT`
`ENABLE_FLAKE8`
`BUILD_opencv_python2`
`BUILD_opencv_python3`
`BUILD_JAVA`
`BUILD_FAT_JAVA_LIB`

@see @ref tutorial_py_table_of_contents_setup


## Profiling, coverage, sanitize, hardening

`ENABLE_PROFILING`
`ENABLE_COVERAGE`
`OPENCV_ENABLE_MEMORY_SANITIZER`
`ENABLE_BUILD_HARDENING`


## Size optimization

`ENABLE_LTO`
`ENABLE_THIN_LTO`


## OpenCV.js

@see @ref tutorial_js_table_of_contents_setup


## Android

`BUILD_ANDROID_PROJECTS`
`BUILD_ANDROID_EXAMPLES`
`ANDROID_HOME`
`ANDROID_SDK`
`ANDROID_NDK`
`ANDROID_SDK_ROOT`


## Cross-compilation

CMAKE_TOOLCHAIN_FILE


## Automated builds

`ENABLE_NOISY_WARNINGS`
`OPENCV_WARNINGS_ARE_ERRORS`
`ENABLE_CONFIG_VERIFICATION`
`OPENCV_CMAKE_HOOKS_DIR`
`OPENCV_DUMP_HOOKS_FLOW`
