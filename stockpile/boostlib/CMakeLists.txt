cmake_minimum_required(VERSION 3.0)
project(boostlib)

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_FLAGS "-std=c++14")

set(BOOST_DIR $ENV{BOOST_DIR})

include_directories(
	${BOOST_DIR}
)

set(SRCS
	${BOOST_DIR}/libs/filesystem/src/codecvt_error_category.cpp
	${BOOST_DIR}/libs/filesystem/src/unique_path.cpp
	${BOOST_DIR}/libs/filesystem/src/operations.cpp
	${BOOST_DIR}/libs/filesystem/src/utf8_codecvt_facet.cpp
	${BOOST_DIR}/libs/filesystem/src/path.cpp
	${BOOST_DIR}/libs/filesystem/src/windows_file_codecvt.cpp
	${BOOST_DIR}/libs/filesystem/src/path_traits.cpp
	${BOOST_DIR}/libs/filesystem/src/windows_file_codecvt.hpp
	${BOOST_DIR}/libs/filesystem/src/portability.cpp
	${BOOST_DIR}/libs/system/src/error_code.cpp
)

add_library(boostlib STATIC "${SRCS}")
