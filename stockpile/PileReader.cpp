#include "PileReader.h"
#include "pile.h"
#include "PileHeader.h"
#include "PileHasher.h"

#include "snappy.h"

#include <fstream>
#include <sstream>

namespace stockpile {

namespace
{
	//-----------------------------------------------------------------
	struct State
	{
		State()
		: pileData(new PileData),
		chunkData(new ChunkData),
		pos(0)
		{}
	
		std::unique_ptr<PileData> pileData;
		std::unique_ptr<ChunkData> chunkData;
		int pos;
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
		if (!s.chunkName.empty() && !s.chunkData->resources.empty())
		{
			s.pileData->chunks[ResourcePath(s.chunkName)].reset(new Chunk(s.chunkData));
		}
		
		s.chunkData.reset(new ChunkData);
		s.pos = 0;
	}
}

//--------------------------------------------------------
std::unique_ptr<Pile> PileReader::readPile(const std::string& path) const
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

			s.chunkData->resources.emplace_back(ResourcePath(s.resourceName), s.pos, (int)uncompressed.size());
			s.chunkData->data += uncompressed;

			s.pos += (int)uncompressed.size();
		}
	}

	completeChunk(s);

	std::string fileHash;
	input >> fileHash;

	input.close();

	std::unique_ptr<Pile> pile(new Pile(s.pileData));

	PileHasher pileHasher;
	const auto hash = pileHasher.getHash(*pile);
	if (hash != fileHash)
	{
		return {};
	}

	return pile;
}

}
