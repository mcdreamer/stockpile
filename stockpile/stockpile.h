#pragma once

#include "json.hpp"
#include "snappy.h"
#include "picosha2.h"

#include <fstream>
#include <string>
#include <vector>
#include <map>

namespace stockpile {

//--------------------------------------------------------
class ResourcePath
{
public:
	explicit ResourcePath(const std::string& path)
	: m_Path(path)
	{}
	
	bool isEmpty() const { return m_Path.empty(); }
	bool operator==(const ResourcePath& other) const { return m_Path == other.m_Path; }
	bool operator<(const ResourcePath& other) const { return m_Path < other.m_Path; }
	std::string toString() const { return m_Path; }
	
private:
	std::string	m_Path;
};

//--------------------------------------------------------
class ResourceDefinition
{
public:
	ResourceDefinition(const std::string& filename,
					   const ResourcePath& targetPath)
	: m_Filename(filename),
	m_TargetPath(targetPath)
	{}

	const std::string& getFilename() const { return m_Filename; }
	const ResourcePath& getTargetPath() const { return m_TargetPath; }
	
private:
	std::string m_Filename;
	ResourcePath m_TargetPath;
};

//--------------------------------------------------------
class ChunkDefinition
{
public:
	ChunkDefinition(const std::string& chunkName,
					const std::vector<ResourceDefinition>& resourceDefinitions)
	: m_ChunkName(chunkName),
	m_ResourceDefinitions(resourceDefinitions)
	{}
	
	const std::string& getChunkName() const { return m_ChunkName; }
	const std::vector<ResourceDefinition>& getResourcesDefinitions() const { return m_ResourceDefinitions; }
	
private:
	std::string m_ChunkName;
	std::vector<ResourceDefinition>	m_ResourceDefinitions;
};

//--------------------------------------------------------
class PileDefinition
{
public:
	PileDefinition(const std::vector<ChunkDefinition>& chunkDefinitions)
	: m_ChunkDefinitions(chunkDefinitions)
	{}
	
	const std::vector<ChunkDefinition>& getChunkDefinitions() const { return m_ChunkDefinitions; }
	
private:
	std::vector<ChunkDefinition> m_ChunkDefinitions;
};




//--------------------------------------------------------
class ResourceData
{
public:
	ResourceData()
	{}
	
	ResourceData(const std::string& data)
	: m_Data(data)
	{}
	
	const std::string&  getData() const { return m_Data; }
	
private:
	std::string  m_Data;
};

//--------------------------------------------------------
class Chunk
{
public:
	Chunk()
	{}
	
	Chunk(const std::map<ResourcePath, ResourceData>& resources)
	: m_Resources(resources)
	{}
	
	const std::map<ResourcePath, ResourceData>& getResources() const { return m_Resources; }
	
private:
	std::map<ResourcePath, ResourceData>	m_Resources;
};

//--------------------------------------------------------
class Pile
{
public:
	Pile(const std::map<std::string, Chunk>& chunks)
	: m_Chunks(chunks)
	{}

	const std::map<std::string, Chunk>& getChunks() const { return m_Chunks; }

private:
	std::map<std::string, Chunk>	m_Chunks;
};

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
		if (hash != fileHash)
		{
			return Pile({});
		}
		
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
