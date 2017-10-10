#include "pilecreator.h"
#include "piledefinition.h"
#include "pile.h"

#include <map>
#include <fstream>
#include <sstream>

namespace stockpile {

//--------------------------------------------------------
Pile PileCreator::createPile(const PileDefinition& pileDef) const
{
	std::unique_ptr<PileData> pileData(new PileData);

	for (const auto& chunkDef : pileDef.getChunkDefinitions())
	{
		std::stringstream ss;
		int pos = 0;

		std::unique_ptr<ChunkData> chunkData(new ChunkData);

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

			chunkData->resources.emplace_back(resourceDef.getTargetPath(), pos, (int)s.size());

			pos += s.size();
		}

		chunkData->data = ss.str();

		pileData->chunks[ResourcePath(chunkDef.getChunkName())].reset(new Chunk(chunkData));
	}

	return Pile(pileData);
}

}
