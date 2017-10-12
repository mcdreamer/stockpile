#include "resourcepath.h"

#include <vector>
#include <map>

namespace stockpile {

//--------------------------------------------------------
struct ChunkResourceAddress
{
	ChunkResourceAddress()
	: startPos(0), length(0)
	{}

	ChunkResourceAddress(const ResourcePath& path_, int startPos_, int length_)
	: path(path_), startPos(startPos_), length(length_)
	{}

	ResourcePath path;
	int startPos;
	int length;
};

using ChunkResourceAddressArray = std::vector<ChunkResourceAddress>;

//--------------------------------------------------------
struct ChunkData
{
	ChunkResourceAddressArray resources;
	std::string	data;
};

//--------------------------------------------------------
struct PileData
{
	std::map<ResourcePath, std::unique_ptr<Chunk>> chunks;
};

}
