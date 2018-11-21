cmake_minimum_required(VERSION 3.0)
project(stockpiletests)

set(SNAPPY_BUILD_TESTS OFF CACHE BOOL "")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("../lib" "lib")
add_subdirectory("../../thirdparty/googletest/googletest" "thirdparty/googletest")

set(BOOST_ROOT $ENV{BOOST_DIR})
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS filesystem REQUIRED)

include_directories(
	"../"
	"../../thirdparty/googletest/googletest/include"
	${Boost_INCLUDE_DIRS}
)

set(SRCS
	tests.cpp
)

add_executable(stockpiletests "${SRCS}")

target_link_libraries(stockpiletests pile ${Boost_LIBRARIES} snappy gtest gtest_main)

file(COPY "pile.json" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "test.txt" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "test1.txt" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "test2.txt" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "moredata/test3.txt" DESTINATION "${CMAKE_BINARY_DIR}/moredata")
