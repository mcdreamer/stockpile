# stockpile

stockpile is a resource management library written in C++. It takes a structure of files, compresses them (using [snappy](https://github.com/google/snappy)), and stores them as an indexed binary blob on disk or in memory. This project exists in order to be used in my game engine, [PG](https://github.com/mcdreamer/PG), but could of course be used for anything.

Currently this is really just a sketch even though it has working code with unit tests exercising it. Lots of tidy up and improved client side functionality is still required.

Building now requires boost. A link to a boost directory should be created in thirdparty.