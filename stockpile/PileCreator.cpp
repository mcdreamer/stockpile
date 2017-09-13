#include "PileCreator.h"
#include "PileDefinition.h"
#include "Pile.h"

#include "snappy.h"

#include <map>
#include <fstream>

namespace stockpile {

//--------------------------------------------------------
Pile PileCreator::createPile(const PileDefinition& pileDef) const
{
	std::map<std::string, Chunk> chunks;
	
	for (const auto& chunkDef : pileDef.getChunkDefinitions())
	{
		std::map<ResourcePath, ResourceData> resources;
		
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
			
			std::string resourceData;
			snappy::Compress(&inputData[0], inputData.size(), &resourceData);
			
			resources[resourceDef.getTargetPath()] = ResourceData(resourceData);
		}
		
		chunks[chunkDef.getChunkName()] = Chunk(resources);
	}
	
	Pile pile(chunks);
	return pile;
}

}
