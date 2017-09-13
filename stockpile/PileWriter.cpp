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
	
	for (const auto& chunk : pile.getChunks())
	{
		output << chunk.first;
		
		for (const auto& resource : chunk.second.getResources())
		{
			output << resource.first.toString() << resource.second.getData();
		}
	}
	
	PileHasher pileHasher;
	output << pileHasher.getHash(pile);
	
	output.close();
}

}
