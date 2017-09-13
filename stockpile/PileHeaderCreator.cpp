#include "PileHeaderCreator.h"
#include "Pile.h"
#include "PileHeader.h"

namespace stockpile {

//--------------------------------------------------------
PileHeader PileHeaderCreator::createHeader(const Pile& pile) const
{
	PileHeader header;
	
	pile.forEachChunk([&](const ResourcePath& chunkPath, const Chunk& chunk)
	{
		header.components.push_back(PileHeader::Component { PileHeader::kChunkName, chunkPath.toString().length() });
		
		chunk.forEachResource([&](const ResourcePath& resourcePath, const ResourceData& data)
		{
			header.components.push_back(PileHeader::Component { PileHeader::kResourceName, resourcePath.toString().length() });
			
			header.components.push_back(PileHeader::Component { PileHeader::kResourceData, data.getData().length() });
		});
	});
	
	return header;
}

}
