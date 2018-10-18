#include "include/pile.h"
#include "pilewriter.h"
#include "pilehasher.h"
#include "pileheadercreator.h"
#include "pileheader.h"

#include "snappy.h"

#include <fstream>

namespace stockpile {

//--------------------------------------------------------
void PileWriter::writePile(const Pile& pile, const std::string& path) const
{
	PileHasher pileHasher;
	const auto pileHash = pileHasher.getHash(pile);

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
			std::string resourceData;
			snappy::Compress(data.getData(), data.getSize(), &resourceData);

			output << resourcePath.toString() << resourceData;
		});
	});

	output << pileHash;

	output.close();
}

}
