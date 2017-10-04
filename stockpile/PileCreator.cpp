#include "PileCreator.h"
#include "PileDefinition.h"
#include "pile.h"

#include <map>
#include <fstream>
#include <sstream>

namespace stockpile {

//--------------------------------------------------------
Pile PileCreator::createPile(const PileDefinition& pileDef) const
{
	std::map<ResourcePath, Chunk> chunks;
	
	for (const auto& chunkDef : pileDef.getChunkDefinitions())
	{
		std::stringstream ss;
		std::vector<std::pair<ResourcePath, std::pair<int, int>>> resources;
		int pos = 0;
		
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
			
			const std::string s(inputData.begin(), inputData.end());
			ss << s;
			
			resources.emplace_back(resourceDef.getTargetPath(), std::make_pair(pos, (int)s.size()));
			
			pos += s.size();
		}
		
		chunks[ResourcePath(chunkDef.getChunkName())] = Chunk(resources, ss.str());
	}
	
	Pile pile(chunks);
	return pile;
}

}
