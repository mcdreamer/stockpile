#include "PileCreator.h"
#include "PileDefinition.h"
#include "pile.h"

#include <map>
#include <fstream>

namespace stockpile {

//--------------------------------------------------------
Pile PileCreator::createPile(const PileDefinition& pileDef) const
{
	std::map<ResourcePath, Chunk> chunks;
	
	for (const auto& chunkDef : pileDef.getChunkDefinitions())
	{
		std::map<ResourcePath, std::string> resources;
		
		for (const auto& resourceDef : chunkDef.getResourcesDefinitions())
		{
			std::ifstream input(resourceDef.getFilename(), std::ios::binary | std::ios::ate);
			if (input.fail())
			{
				throw new std::exception();
			}
			
			const auto inputLength = input.tellg();
			std::vector<char> inputData(inputLength);
			
			input.seekg(0, std::ios::beg);
			input.read(&inputData[0], inputLength);
			
			resources[resourceDef.getTargetPath()] = std::string(inputData.begin(), inputData.end());
		}
		
		chunks[ResourcePath(chunkDef.getChunkName())] = Chunk(resources);
	}
	
	Pile pile(chunks);
	return pile;
}

}
