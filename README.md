# stockpile

stockpile is a resource management library written in C++. It takes a structure of files, compresses them (using [snappy](https://github.com/google/snappy)), and stores them as an indexed binary blob on disk or in memory. This project exists in order to be used in my game engine, [PG](https://github.com/mcdreamer/PG), but could of course be used for anything.

Currently this is really just a sketch even though it has working code with unit tests exercising it. Lots of tidy up and improved client side functionality is still required.

You can use stockpile_cmake.txt to provide the build_pile function in CMakeLists.txt. This function adds a custom command which runs stockpile on an input and produces a pile.

Building now requires boost. Set BOOST_DIR to the location of a boost directory. Boost should be built using `./b2 --with-system --with-filesystem toolset=clang cxxstd=14`.
