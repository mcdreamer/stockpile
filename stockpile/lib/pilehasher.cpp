#include "include/pile.h"
#include "pilehasher.h"
#include "pileheadercreator.h"
#include "pileheader.h"

#include "picosha2.h"

#include <array>

namespace stockpile {

//--------------------------------------------------------
std::string PileHasher::getHash(const Pile& pile) const
{
	picosha2::hash256_one_by_one hasher;

	PileHeaderCreator headerCreator;
	const auto header = headerCreator.createHeader(pile);
	constexpr auto headerCompHashSize = sizeof(std::uint8_t) + sizeof(std::uint64_t) + 1;
	static_assert(headerCompHashSize == 10, "Unexpected size of header component hash");
	std::array<std::uint8_t, headerCompHashSize> headerCompHash;
	for (const auto& pileComp : header.components)
	{
		headerCompHash.fill(0);
		headerCompHash[0] = pileComp.type;
		*(&headerCompHash[1]) = pileComp.length;
		hasher.process(headerCompHash.begin(), headerCompHash.end());
	}

	pile.forEachChunk([&](const ResourcePath& chunkPath, const Chunk& chunk)
	{
		const auto pathStr = chunkPath.toString();
		hasher.process(pathStr.begin(), pathStr.end());

		chunk.forEachResource([&](const ResourcePath& resourcePath, const ResourceData& data)
		{
			const auto resourcePathStr = resourcePath.toString();

			hasher.process(resourcePathStr.begin(), resourcePathStr.end());
			hasher.process(data.getData(), data.getData() + data.getSize());
		});
	});

	return picosha2::get_hash_hex_string(hasher);
};

}
