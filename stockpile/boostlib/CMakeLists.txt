cmake_minimum_required(VERSION 2.6)
project(boostlib)

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-std=c++14")

include_directories(
	"../../thirdparty/boost"
)

set(SRCS
	../../thirdparty/boost/libs/filesystem/src/codecvt_error_category.cpp
	../../thirdparty/boost/libs/filesystem/src/unique_path.cpp
	../../thirdparty/boost/libs/filesystem/src/operations.cpp
	../../thirdparty/boost/libs/filesystem/src/utf8_codecvt_facet.cpp
	../../thirdparty/boost/libs/filesystem/src/path.cpp
	../../thirdparty/boost/libs/filesystem/src/windows_file_codecvt.cpp
	../../thirdparty/boost/libs/filesystem/src/path_traits.cpp
	../../thirdparty/boost/libs/filesystem/src/windows_file_codecvt.hpp
	../../thirdparty/boost/libs/filesystem/src/portability.cpp

	../../thirdparty/boost/libs/system/src/error_code.cpp
)

add_library(boostlib STATIC "${SRCS}")
