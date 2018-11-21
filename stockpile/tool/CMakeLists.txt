cmake_minimum_required(VERSION 3.0)
project(stockpile)

set(SNAPPY_BUILD_TESTS OFF CACHE BOOL "")

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("../lib" "lib")

set(BOOST_ROOT $ENV{BOOST_DIR})
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS filesystem REQUIRED)

include_directories(
  "../"
  "../../thirdparty/args"
  ${Boost_INCLUDE_DIRS}
)

set(SRCS
	main.cpp
)

add_executable(stockpile "${SRCS}")

target_link_libraries(stockpile pile snappy ${Boost_LIBRARIES})
