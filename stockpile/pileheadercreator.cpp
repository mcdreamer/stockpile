#include "pileheadercreator.h"
#include "pile.h"
#include "pileheader.h"

#include "snappy.h"

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

			// Improve this
			std::string compressedData;
			snappy::Compress(&data.getData()[0], data.getData().size(), &compressedData);

			header.components.push_back(PileHeader::Component { PileHeader::kResourceData, compressedData.length() });
		});
	});

	return header;
}

}
