#include "PileReader.h"
#include "pile.h"
#include "PileHeader.h"
#include "PileHasher.h"

#include "snappy.h"

#include <fstream>

namespace stockpile {

namespace
{
	//-----------------------------------------------------------------
	struct State
	{
		std::map<ResourcePath, Chunk> chunks;
		std::map<ResourcePath, std::string> resources;
		std::string chunkName;
		std::string resourceName;
	};
	
	//--------------------------------------------------------
	PileHeader readHeader(std::ifstream& input)
	{
		PileHeader header;
		
		unsigned long numComponents = 0;
		input >> numComponents;
		
		for (unsigned long nthComp = 0; nthComp < numComponents; ++nthComp)
		{
			PileHeader::Component comp;
			
			input >> comp.type;
			input >> comp.length;
			
			header.components.push_back(comp);
		}
		
		return header;
	}
	
	//--------------------------------------------------------
	std::string readData(const PileHeader::Component& pileComp, std::ifstream& input)
	{
		std::string data(pileComp.length, '\0');
		
		input.read(&data[0], pileComp.length);
		
		return data;
	}
	
	//--------------------------------------------------------
	void completeChunk(State& s)
	{
		if (!s.chunkName.empty() && !s.resources.empty())
		{
			s.chunks[ResourcePath(s.chunkName)] = Chunk(s.resources);
		}
		
		s.resources.clear();
	}
}

//--------------------------------------------------------
Pile PileReader::readPile(const std::string& path) const
{
	std::ifstream input;
	input.open(path);
	
	const auto header = readHeader(input);
	
	State s;
	
	for (const auto& pileComp : header.components)
	{
		if (pileComp.type == PileHeader::kChunkName)
		{
			completeChunk(s);
			
			s.chunkName = readData(pileComp, input);
		}
		else if (pileComp.type == PileHeader::kResourceName)
		{
			s.resourceName = readData(pileComp, input);
		}
		else if (pileComp.type == PileHeader::kResourceData)
		{
			const auto data = readData(pileComp, input);
			std::string uncompressed;
			snappy::Uncompress(data.data(), data.size(), &uncompressed);
	
			s.resources[ResourcePath(s.resourceName)] = uncompressed;
		}
	}
	
	completeChunk(s);

	std::string fileHash;
	input >> fileHash;
	
	input.close();
	
	const Pile pile(s.chunks);
	
	PileHasher pileHasher;
	const auto hash = pileHasher.getHash(pile);
	if (hash != fileHash)
	{
		return {};
	}
	
	return pile;
}

}
