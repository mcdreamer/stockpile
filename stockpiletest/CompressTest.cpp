#include "CompressTest.h"
#include "gtest/gtest.h"
#include "stockpile/stockpile.h"

#include "json.hpp"
#include "snappy.h"
#include "picosha2.h"

#include <memory>

namespace stockpile {

//--------------------------------------------------------
class PileDefinitionLoader
{
	using json = nlohmann::json;

public:
	//--------------------------------------------------------
	std::unique_ptr<PileDefinition> loadFromFile(const std::string& path) const
	{
		std::unique_ptr<PileDefinition> pile;
	
		std::ifstream input(path);
		if (input.fail())
		{
			return {};
		}
		
		json pileDefJson;
		input >> pileDefJson;
		input.close();
	
		std::vector<ChunkDefinition> chunkDefs;
	
		readChunks(pileDefJson, chunkDefs);
	
		pile.reset(new PileDefinition(chunkDefs));
		
		return pile;
	}
	
private:
	//--------------------------------------------------------
	void readChunks(const json& pileDefJson, std::vector<ChunkDefinition>& chunkDefs) const
	{
		for (const auto& chunkDefJson : pileDefJson["chunks"])
		{
			chunkDefs.push_back(readChunk(chunkDefJson));
		}
	}
	
	//--------------------------------------------------------
	ChunkDefinition readChunk(const json& chunkDefJson) const
	{
		std::vector<ResourceDefinition> resourceDefs;
		resourceDefs.reserve(chunkDefJson["resources"].size());
	
		for (const auto& resourceDefJson : chunkDefJson["resources"])
		{
			resourceDefs.push_back(readResource(resourceDefJson));
		}
		
		return ChunkDefinition(chunkDefJson["name"], resourceDefs);
	}
	
	//--------------------------------------------------------
	ResourceDefinition readResource(const json& resourceDefJson) const
	{
		const auto file = getValueForKey(resourceDefJson, "file");
		const auto path = getValueForKey(resourceDefJson, "path");
		
		return ResourceDefinition(file, ResourcePath(path.empty() ? file : path));
	}
	
	//--------------------------------------------------------
	std::string getValueForKey(const json& Json, const std::string& key) const
	{
		auto valueIt = Json.find(key);
		return valueIt != Json.end() ? valueIt->get<std::string>() : "";
	}
};

}

//--------------------------------------------------------
TEST(PileDefinitionLoaderTest,testLoadFromPath)
{
	stockpile::PileDefinitionLoader loader;
	auto pileDef = loader.loadFromFile("pile.json");
	ASSERT_TRUE(pileDef.get());
	
	ASSERT_EQ((size_t)1, pileDef->getChunkDefinitions().size());
	
	const auto& chunkDef = pileDef->getChunkDefinitions().front();
	EXPECT_EQ(std::string("testchunk"), chunkDef.getChunkName());
	ASSERT_EQ((size_t)2, chunkDef.getResourcesDefinitions().size());
	
	EXPECT_EQ(std::string("test.txt"), chunkDef.getResourcesDefinitions()[0].getFilename());
	EXPECT_EQ(stockpile::ResourcePath("testdata"), chunkDef.getResourcesDefinitions()[0].getTargetPath());
	
	EXPECT_EQ(std::string("test1.txt"), chunkDef.getResourcesDefinitions()[1].getFilename());
	EXPECT_EQ(stockpile::ResourcePath("test1.txt"), chunkDef.getResourcesDefinitions()[1].getTargetPath());
}

namespace stockpile {

//--------------------------------------------------------
class PileCreator
{
public:
	Pile createPile(const PileDefinition& pileDef) const
	{
		std::map<std::string, Chunk> chunks;
	
		for (const auto& chunkDef : pileDef.getChunkDefinitions())
		{
			std::map<ResourcePath, ResourceData> resources;
		
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
				
				std::string resourceData;
				snappy::Compress(&inputData[0], inputData.size(), &resourceData);
			
				resources[resourceDef.getTargetPath()] = ResourceData(resourceData);
			}
			
			chunks[chunkDef.getChunkName()] = Chunk(resources);
		}
	
		Pile pile(chunks);
		return pile;
	}
};

}

//--------------------------------------------------------
TEST(PileCreator,testCreatePile)
{
	stockpile::PileDefinitionLoader loader;
	auto pileDef = loader.loadFromFile("pile.json");
	ASSERT_TRUE(pileDef.get());
	
	stockpile::PileCreator creator;
	const auto pile = creator.createPile(*pileDef);
	
	ASSERT_EQ((size_t)1, pile.getChunks().size());
	
	const auto& chunk = *pile.getChunks().begin();
	EXPECT_EQ(std::string("testchunk"), chunk.first);
	EXPECT_EQ((size_t)2, chunk.second.getResources().size());
	
	EXPECT_EQ(stockpile::ResourcePath("test1.txt"), chunk.second.getResources().begin()->first);
	EXPECT_NE(std::string("More test data\n"), chunk.second.getResources().begin()->second.getData());
	
	std::string reversed;
	snappy::Uncompress(chunk.second.getResources().begin()->second.getData().data(), chunk.second.getResources().begin()->second.getData().size(), &reversed);
	
	EXPECT_EQ(std::string("More test data\n"), reversed);
}

namespace stockpile {

//--------------------------------------------------------
struct PileHeader
{
	enum ComponentType
	{
		kChunkName,
		kResourceName,
		kResourceData
	};

	struct Component
	{
		char			type;
		unsigned long	length;
	};
	
	std::vector<Component> components;
};

//--------------------------------------------------------
PileHeader createHeader(const Pile& pile)
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

//--------------------------------------------------------
class PileHasher
{
public:
	//--------------------------------------------------------
	std::string getHash(const Pile& pile) const
	{
		picosha2::hash256_one_by_one hasher;
		
		const auto header = createHeader(pile);
		for (const auto& pileComp : header.components)
		{
			std::array<char, sizeof(char) + sizeof(unsigned long) + 1> val;
			val.fill(0);
			val[0] = pileComp.type;
			*(&val[1]) = pileComp.length;
			hasher.process(val.begin(), val.end());
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
};

//--------------------------------------------------------
class PileWriter
{
public:
	void writePile(const Pile& pile, const std::string& path) const
	{
		const auto header = createHeader(pile);
		
		std::ofstream output;
		output.open(path);
		
		output << header.components.size();
		
		for (const auto& pileComp : header.components)
		{
			output << pileComp.type << pileComp.length;
		}
		
		for (const auto& chunk : pile.getChunks())
		{
			output << chunk.first;
			
			for (const auto& resource : chunk.second.getResources())
			{
				output << resource.first.toString() << resource.second.getData();
			}
		}
		
		PileHasher pileHasher;
		output << pileHasher.getHash(pile);
		
		output.close();
	}
};

//--------------------------------------------------------
class PileReader
{
	struct State
	{
		std::map<std::string, Chunk> chunks;
		std::map<ResourcePath, ResourceData> resources;
		std::string chunkName;
		std::string resourceName;
		picosha2::hash256_one_by_one hasher;
	};
	
public:
	//--------------------------------------------------------
	Pile readPile(const std::string& path) const
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
				s.resources[ResourcePath(s.resourceName)] = readData(pileComp, input);
			}
		}
		
		completeChunk(s);

		std::string fileHash;
		input >> fileHash;
		
		input.close();
		
		const Pile pile(s.chunks);
		
		PileHasher pileHasher;
		const auto hash = pileHasher.getHash(pile);
		
		return pile;
	}
	
private:
	//--------------------------------------------------------
	PileHeader readHeader(std::ifstream& input) const
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
	std::string readData(const PileHeader::Component& pileComp, std::ifstream& input) const
	{
		std::string data(pileComp.length, '\0');
		
		input.read(&data[0], pileComp.length);
		
		return data;
	}
	
	//--------------------------------------------------------
	void completeChunk(State& s) const
	{
		if (!s.chunkName.empty() && !s.resources.empty())
		{
			s.chunks[s.chunkName] = Chunk(s.resources);
		}
		
		s.resources.clear();
	}
};

}

//--------------------------------------------------------
TEST(PileCreator,testWritePile)
{
	stockpile::PileDefinitionLoader loader;
	auto pileDef = loader.loadFromFile("pile.json");
	ASSERT_TRUE(pileDef.get());
	
	stockpile::PileCreator creator;
	const auto pile = creator.createPile(*pileDef);

	stockpile::PileWriter writer;
	writer.writePile(pile, "test.pile");
	
	stockpile::PileReader reader;
	const auto loadedPile = reader.readPile("test.pile");
	
	ASSERT_EQ((size_t)1, loadedPile.getChunks().size());
	
	const auto& chunk = *loadedPile.getChunks().begin();
	EXPECT_EQ(std::string("testchunk"), chunk.first);
	EXPECT_EQ((size_t)2, chunk.second.getResources().size());
	
	EXPECT_EQ(stockpile::ResourcePath("test1.txt"), chunk.second.getResources().begin()->first);
	EXPECT_NE(std::string("More test data\n"), chunk.second.getResources().begin()->second.getData());
	
	std::string reversed;
	snappy::Uncompress(chunk.second.getResources().begin()->second.getData().data(), chunk.second.getResources().begin()->second.getData().size(), &reversed);
	
	EXPECT_EQ(std::string("More test data\n"), reversed);
	
	stockpile::PileHasher hasher;
	EXPECT_EQ(hasher.getHash(pile), hasher.getHash(loadedPile));
}
