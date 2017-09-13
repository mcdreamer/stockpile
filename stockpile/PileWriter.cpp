#include "PileWriter.h"
#include "Pile.h"
#include "PileHasher.h"
#include "PileHeaderCreator.h"
#include "PileHeader.h"

#include <fstream>

namespace stockpile {

//--------------------------------------------------------
void PileWriter::writePile(const Pile& pile, const std::string& path) const
{
	PileHeaderCreator headerCreator;
	const auto header = headerCreator.createHeader(pile);
	
	std::ofstream output;
	output.open(path);
	
	output << header.components.size();
	
	for (const auto& pileComp : header.components)
	{
		output << pileComp.type << pileComp.length;
	}
	
	pile.forEachChunk([&](const ResourcePath& chunkPath, const Chunk& chunk)
	{
		output << chunkPath.toString();
		
		chunk.forEachResource([&](const ResourcePath& resourcePath, const ResourceData& data)
		{
			output << resourcePath.toString() << data.getData();
		});
	});
	
	PileHasher pileHasher;
	output << pileHasher.getHash(pile);
	
	output.close();
}

}
