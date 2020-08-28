#!/bin/bash
# This file contains documentation snippets for Linux installation tutorial
if [ "$1" = "--check" ] ; then
sudo()
{
    command $@
}
fi

# [body]
# Install minimal prerequisites in Ubuntu  Ubuntu 10.04, 16.04 LST and 18.04 LST
sudo apt update && sudo apt install -y cmake g++ wget unzip

# Install minimal prerequisites in openSUSE Leap 15.2, opensSUSE Tumbleweed and SUSE Linux Enterprise Server
sudo zypper in patterns-devel-C-C++-devel_C_C++

# Install minimal prerequisites in openSUSE Leap 15.2, opensSUSE Tumbleweed and SUSE Linux Enterprise Server
sudo zypper in patterns-devel-C-C++-devel_C_C++ unzip  wget 

# Install minimal prerequisites in CentOS 8 or Red Hat Enterprise Linux 8
sudo dnf -y install gcc gcc-c++ cmake wget unzip

# Download and unpack sources
wget -O opencv.zip https://github.com/opencv/opencv/archive/master.zip
unzip opencv.zip

# Create build directory
mkdir -p build && cd build

# Configure
cmake  ../opencv-master

# Build
cmake --build .
# [body]
