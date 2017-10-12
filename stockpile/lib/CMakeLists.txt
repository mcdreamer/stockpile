cmake_minimum_required(VERSION 2.6)
project(pile)

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-std=c++14")

include_directories(
  "../"
  "../include"
	"../../thirdparty/json/src"
	"../../thirdparty/PicoSHA2"
	"../../thirdparty/snappy"
	"../../thirdparty/snappy/build"
)

set(SRCS
	../pile.cpp
	../pilecreator.cpp
	../pilecreator.h
	../piledefinition.h
	../piledefinitionloader.cpp
	../piledefinitionloader.h
	../pilehasher.cpp
	../pilehasher.h
	../pileheader.h
	../pileheadercreator.cpp
	../pileheadercreator.h
	../pilereader.cpp
	../pilereader.h
	../pilewriter.cpp
	../pilewriter.h
	../resourcepath.cpp
	../stockpile.cpp
)

add_library(pile STATIC "${SRCS}")
