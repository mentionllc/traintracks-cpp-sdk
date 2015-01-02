cmake_minimum_required(VERSION 2.6)

project(TTCPPSDK_COMMON_UTIL)

MESSAGE("Entering util")

cmake_minimum_required(VERSION 2.6)

SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY $ENV{TT_ROOT}/bin)
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY $ENV{TT_ROOT}/bin)
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY $ENV{TT_ROOT}/bin)

SET (CURRENT_RUN_DIR $ENV{TT_RUN_DIR})
SET (LINUX_BUILD_BASE_RUN_LOCATION $ENV{TT_BASE_RUN_LOCATION})
SET (TT_CONFIG_DIR=ENV{TT_CONFIG_DIR})

project(TTCPPSDK_COMMON_UTIL)

add_definitions(-msse2 -mfpmath=sse -m64 -fpermissive -Wint-to-pointer-cast -Wwrite-strings -Wno-deprecated -lz -DLINUX_BUILD=${BUILD_TARGET})

INCLUDE_DIRECTORIES(..
$ENV{TT_ROOT}/common/util
$ENV{TT_ROOT}/common/include
/usr/include
/usr/local/include
/usr/local/opt/log4cplus/include
)

add_library(ttutil STATIC
CliwentAssert.cpp 
CommonAssert.cpp
CommonLogging.cpp
CSplitPath.cpp
ExceptionHandler.cpp
stdafx.cpp
StringHelpers.cpp
StringUtils.cpp
)



MESSAGE("Exiting util")
