OpenCV installation overview {#tutorial_general_install}
============================

@tableofcontents

There are two ways of installing OpenCV on your machine: download prebuilt version for your platform or compile from sources.

# Prebuilt version {#tutorial_general_install_prebuilt}

In many cases one can find prebuilt version of OpenCV that will meet their needs.

## Packages by OpenCV core team  {#tutorial_general_install_prebuilt_core}

Packages for Android, iOS and Windows built with default parameters and recent compilers are published for each release, they do not contain _opencv_contrib_ modules.

- GitHub releases: https://github.com/opencv/opencv/releases
- SourceForge.net: https://sourceforge.net/projects/opencvlibrary/files/


## Third-party packages  {#tutorial_general_install_prebuilt_thirdparty}

Other organizations and people maintain their own distributions of OpenCV. Some examples can be found in following locations:

- System packages in popular Linux distributions (https://pkgs.org/search/?q=opencv)
- PyPI (https://pypi.org/search/?q=opencv)
- Conda (https://anaconda.org/search?q=opencv)
- Conan (https://github.com/conan-community/conan-opencv)
- vcpkg (https://github.com/microsoft/vcpkg/tree/master/ports/opencv)
- NuGet (https://www.nuget.org/packages?q=opencv)
- Brew (https://formulae.brew.sh/formula/opencv)
- Maven (https://search.maven.org/search?q=opencv)


# Build from sources {#tutorial_general_install_sources}

In some cases existing binary packages are not applicable for your use case, then you'll have to build custom version of OpenCV by yourself. This section contains high-level overview of build and installation process. Check tutorial specific for your platform for actual build instructions.

OpenCV uses [CMake](https://cmake.org/) build management system for configuration and build, so this section just describes generalized process of building software with CMake.


## Step 0: Prepare the system {#tutorial_general_install_sources_0}

Install C++ compiler and build tools. On \*NIX platforms it is usually GCC/G++ or Clang compiler and Make or Ninja build tool. On Windows it can be Visual Studio IDE or MinGW-w64 compiler. Native toolchains for Android are provided in Android NDK. XCode is used to build software for OSX and iOS.

Install CMake from official site, your Linux distribution repository or any other sources.

Get other third-party dependencies: it can be libraries and/or tools which are required for extra functionality like decoding videos or showing GUI elements; others can provide optimized implementations of some algorithms; additional tools can be used for documentation generation and other extras.


## Step 1: Get software sources {#tutorial_general_install_sources_1}

Software projects consists of one or multiple code repositories. OpenCV have two code repositories: _opencv_ - main repository with most stable and actively supported algorithms; _opencv_contrib_ - additional experimental and non-free (patented) algorithms.

You can download a snapshot of repository - state of the code at some moment of time; or clone full repository - this method will get all development history and will allow you to switch to any state at any time.

To download snapshot archives:

- Go to https://github.com/opencv/opencv/releases and download "Source code" archive from any release, e.g. 4.1.2.
- (optionally) Go to https://github.com/opencv/opencv_contrib/releases and download "Source code" archive for the same release as _opencv_
- Unpack all archives to some location

To clone repositories run the following commands in console (_git_ must be installed):

```.sh
git clone https://github.com/opencv/opencv
git -C opencv checkout <some-tag>

# optionally
git clone https://github.com/opencv/opencv_contrib
git -C opencv_contrib checkout <same-tag-as-opencv>
```

@note
If you want to build software using more than one repository, make sure all components are compatible with each other. For OpenCV it means that _opencv_ and _opencv_contrib_ repositories must be checked out at the same tag or snapshot archives are downloaded from the same release.


## Step 2: Configure {#tutorial_general_install_sources_2}

Process of configuration will verify that all necessary tools and dependencies are available and compatible with the library and will generate intermediate files required by chosen build system. The result can vary from basic Makefiles to more complex IDE projects and solutions. Usually this step is performed in newly created build directory by command:
```
cmake -G<generator> <configuration-options> <source-directory>
```

@note
`cmake-gui` application allows to see and modify available options using graphical user interface.


## Step 3: Build {#tutorial_general_install_sources_3}

During build process source files are compiled to object files which are linked together or otherwise combined to libraries and applications, other related actions are performed depending on build configuration. This step can be run by universal command:
```
cmake --build <build-directory> <build-options>
```
... or by calling actual build system directly:
```
make
```

## Step 3: Install {#tutorial_general_install_sources_4}

Installation process copies build results and other files from build directory to install location. Default installation root location is `/usr/local` on UNIX and `C:/Program Files` on Windows. This location can be changed on the configuration step by setting `CMAKE_INSTALL_PREFIX` option. To perform installation run the following command:
```
cmake --build <build-directory> --target install <other-options>
```

@note
On UNIX, if the installation root location is a protected system directory, run the installation process as superuser (e.g. `sudo cmake ...`).
