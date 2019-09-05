OpenCV installation process overview {#tutorial_general_install}
====================================

Prebuilt version {#tutorial_general_install_prebuilt}
================

Packages provided by OpenCV core team:

- Android, iOS, Windows
- no contrib
- mostly default parameters
- files attached to releases https://github.com/opencv/opencv/releases
- sourceforge.net https://sourceforge.net/projects/opencvlibrary/files/

Third-party packages:

- system packages on Linux (https://pkgs.org/download/opencv)
- pip https://pypi.org/search/?q=opencv
- conda https://anaconda.org/search?q=opencv
- conan https://github.com/conan-community/conan-opencv
- vcpkg https://github.com/microsoft/vcpkg/tree/master/ports/opencv
- nuget https://www.nuget.org/packages?q=opencv
- brew https://formulae.brew.sh/formula/opencv
- maven https://search.maven.org/search?q=opencv
- others


Build from sources {#tutorial_general_install_sources}
==================

Steps
-----

- Prepare the system: install compiler, build tools and cmake
- Get OpenCV sources: download archive or clone repository(ies)
- Configure: cmake ../opencv
- Build: cmake --build .
- Install: cmake --build . --target install

Advanced build features {#tutorial_general_install_detailed_advanced}
-----------------------

OpenCV library has modular structure where each module can depend on other modules and 3rdparty libraries. There are also exist several features allowing to modify build process and produce binaries in different form.

### Debug build {#tutorial_general_install_detailed_advanced_debug}

Binaries will contain debug symbols and all optimizations will be turned off. On some platforms (e.g. Linux) this option should be enabled during configuration stage, because build directory can contain only one configuration:
```.sh
cmake -DCMAKE_BUILD_TYPE=Debug ../opencv
```
Other platforms can switch between configurations during build step (e.g. Visual Studio, XCode):
```.bat
cmake --build . --config Debug
```

### Static build {#tutorial_general_install_detailed_advanced_static}

Binaries will not be linked to dynamic libraries but packed into `.a` or `.lib` archives.
It is controlled by _cmake_ option:
```.sh
cmake -DBUILD_SHARED_LIBS=OFF ../opencv
```
Read more here: https://en.wikipedia.org/wiki/Static_library

### Generate pkg-config info

This file can be useful for projects not using CMake. Please note that resulting `.pc` file can contain incomplete list of third-party dependencies and may not work in some configurations, especially for static builds.

```.sh
cmake -DOPENCV_GENERATE_PKGCONFIG=ON ../opencv
```

### Build on multiple cores {#tutorial_general_install_detailed_advanced_cores}

Some build tools can run several jobs at the same time, thus speeding up the compilation process. For example, _make_ has `-j` option for number of tasks run in parallel:

```.sh
make -j10
```

### Install to specific directory {#tutorial_general_install_detailed_advanced_install}

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

### Build tests, samples and applications {#tutorial_general_install_detailed_advanced_tests}

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

### Build limited set of modules {#tutorial_general_install_detailed_advanced_modules}

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

### Build with extra modules (opencv_contrib) {#tutorial_general_install_detailed_advanced_contrib}

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

### Downloaded dependencies {#tutorial_general_install_detailed_advanced_download}

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


### CPU optimization level {#tutorial_general_install_detailed_advanced_cpu}

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
