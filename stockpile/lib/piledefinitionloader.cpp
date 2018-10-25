#include "piledefinitionloader.h"
#include "piledefinition.h"

#include "json.hpp"

#include <fstream>

namespace stockpile {

namespace
{
	using json = nlohmann::json;

	//--------------------------------------------------------
	std::string getValueForKey(const json& Json, const std::string& key)
	{
		auto valueIt = Json.find(key);
		return valueIt != Json.end() ? valueIt->get<std::string>() : "";
	}

	//--------------------------------------------------------
	ResourceDefinition readResource(const json& resourceDefJson)
	{
		const auto file = getValueForKey(resourceDefJson, "file");
		const auto path = getValueForKey(resourceDefJson, "path");

		return ResourceDefinition(file, ResourcePath(path.empty() ? file : path));
	}

	//--------------------------------------------------------
	ChunkDefinition readChunk(const json& chunkDefJson)
	{
		std::vector<ResourceDefinition> resourceDefs;
		resourceDefs.reserve(chunkDefJson["resources"].size());

		for (const auto& resourceDefJson : chunkDefJson["resources"])
		{
			resourceDefs.push_back(readResource(resourceDefJson));
		}

		return ChunkDefinition(chunkDefJson["name"], getValueForKey(chunkDefJson, "basepath"), resourceDefs);
	}

	//--------------------------------------------------------
	void readChunks(const json& pileDefJson, std::vector<ChunkDefinition>& chunkDefs)
	{
		for (const auto& chunkDefJson : pileDefJson["chunks"])
		{
			chunkDefs.push_back(readChunk(chunkDefJson));
		}
	}
}

//--------------------------------------------------------
std::unique_ptr<PileDefinition> PileDefinitionLoader::loadFromFile(const std::string& path) const
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

}
