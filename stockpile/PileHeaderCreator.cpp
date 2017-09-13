#include "PileHeaderCreator.h"
#include "Pile.h"
#include "PileHeader.h"

namespace stockpile {

//--------------------------------------------------------
PileHeader PileHeaderCreator::createHeader(const Pile& pile) const
{
	PileHeader header;
	
	for (const auto& chunk : pile.getChunks())
	{
		header.components.push_back(PileHeader::Component { PileHeader::kChunkName, chunk.first.length() });
		
		for (const auto& resource : chunk.second.getResources())
		{
			header.components.push_back(PileHeader::Component { PileHeader::kResourceName, resource.first.toString().length() });
			
			header.components.push_back(PileHeader::Component { PileHeader::kResourceData, resource.second.getData().length() });
		}
	}
	
	return header;
}

}
