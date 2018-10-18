#pragma once

#include <vector>

namespace stockpile {

//--------------------------------------------------------
struct PileHeader
{
	enum ComponentType : std::uint8_t
	{
		kChunkName,
		kResourceName,
		kResourceData
	};
	
	struct Component
	{
		std::uint8_t	type;
		std::uint64_t	length;
	};
	
	std::vector<Component> components;
};

}
