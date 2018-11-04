cmake_minimum_required(VERSION 3.0)
project(pile)

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-std=c++14")

include_directories(
	"../"
	"../../thirdparty/json/include"
	"../../thirdparty/json/include/nlohmann"
	"../../thirdparty/PicoSHA2"
	"../../thirdparty/snappy"
	"../../thirdparty/snappy/build"
)

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
