# stockpile

stockpile is a resource management library written in C++. It takes a structure of files, compresses them using Google's snappy, and stores them as an indexed binary blob on disk or in memory. This project exists in order to be used in my game engine, PG, but could of course be used for anything.

Currently this is really just a sketch even though it has working code with unit tests exercising it. Lots of tidy up and improved client side functionality is still required. Additionally the library and tool need to be separated so that clients applications can just pull in the loading functionality.
