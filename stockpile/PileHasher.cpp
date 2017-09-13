#include "PileHasher.h"
#include "Pile.h"
#include "PileHeaderCreator.h"
#include "PileHeader.h"

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
	
	for (const auto& chunk : pile.getChunks())
	{
		hasher.process(chunk.first.begin(), chunk.first.end());
		
		for (const auto& resource : chunk.second.getResources())
		{
			const auto resourcePath = resource.first.toString();
			const auto& data = resource.second.getData();
			
			hasher.process(resourcePath.begin(), resourcePath.end());
			hasher.process(data.begin(), data.end());
		}
	}
	
	return picosha2::get_hash_hex_string(hasher);
};

}
