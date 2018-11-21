cmake_minimum_required(VERSION 3.0)
project(pile)

set(SNAPPY_BUILD_TESTS OFF CACHE BOOL "")

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("../../thirdparty/snappy" "snappy")

include_directories(
	"../"
	"../../thirdparty/json/include"
	"../../thirdparty/json/include/nlohmann"
	"../../thirdparty/PicoSHA2"
	"../../thirdparty/snappy"
	"${PROJECT_BINARY_DIR}/snappy"
)

message("${PROJECT_BINARY_DIR}/snappy")

set(SRCS
	pile.cpp
	pilecreator.cpp
	pilecreator.h
	piledata.h
	piledefinition.h
	piledefinitionloader.cpp
	piledefinitionloader.h
	pilehasher.cpp
	pilehasher.h
	pileheader.h
	pileheadercreator.cpp
	pileheadercreator.h
	pilereader.cpp
	pilereader.h
	pilewriter.cpp
	pilewriter.h
	resourcepath.cpp
	stockpile.cpp
)

add_library(pile STATIC "${SRCS}")
