cmake_minimum_required(VERSION 3.5)
project(opencv_videoio_ffmpeg)

include(FindPkgConfig)

# if(NOT FFMPEG_FOUND)
#   find_package(FFMPEG QUIET) # Required components: AVCODEC AVFORMAT AVUTIL SWSCALE
#   if(FFMPEG_FOUND)
#     message("Found FFmpeg via find_package:")
#     message("FFMPEG_VERSION = ${FFMPEG_VERSION}")
#   endif()
# endif()

if(NOT FFMPEG_FOUND)
  if(PKG_CONFIG_FOUND)
    pkg_check_modules(FFMPEG libavcodec libavformat libavutil libswscale libavresample)
    message("FFMPEG_FOUND = ${FFMPEG_FOUND}")
    message("FFMPEG_LIBRARIES = ${FFMPEG_LIBRARIES}")
    message("FFMPEG_LIBRARY_DIRS = ${FFMPEG_LIBRARY_DIRS}")
    message("FFMPEG_LDFLAGS = ${FFMPEG_LDFLAGS}")
    message("FFMPEG_LDFLAGS_OTHER = ${FFMPEG_LDFLAGS_OTHER}")
    message("FFMPEG_LIBDIR = ${FFMPEG_LIBDIR}")
    message("FFMPEG_INCLUDE_DIRS = ${FFMPEG_INCLUDE_DIRS}")
    message("FFMPEG_CFLAGS = ${FFMPEG_CFLAGS}")
    message("FFMPEG_CFLAGS_OTHER = ${FFMPEG_CFLAGS_OTHER}")
    if(FFMPEG_FOUND)
      add_library(found_ffmpeg INTERFACE IMPORTED)
      set_target_properties(found_ffmpeg PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${FFMPEG_LIBRARIES}"
      )
      set(FFMPEG_FOUND ON)
    endif()
  endif()
endif()


if(NOT FFMPEG_FOUND OR NOT TARGET found_ffmpeg)
  message(FATAL_ERROR "Not found!")
endif()

add_library(opencv_videoio_ffmpeg MODULE ../../src/cap_ffmpeg.cpp)
target_include_directories(opencv_videoio_ffmpeg PRIVATE ../../src ../../include ../../../core/include ../../../imgproc/include)
target_compile_definitions(opencv_videoio_ffmpeg PRIVATE HAVE_FFMPEG PLUGIN_BUILD)
target_link_libraries(opencv_videoio_ffmpeg found_ffmpeg)
set_target_properties(opencv_videoio_ffmpeg PROPERTIES
  CXX_STANDARD 11
  CXX_VISIBILITY_PRESET hidden
)
