#pragma once

#include "ResourcePath.h"

#include <string>
#include <map>

namespace stockpile {

//--------------------------------------------------------
class ResourceData
{
public:
	ResourceData()
	{}
	
	ResourceData(const std::string& data)
	: m_Data(data)
	{}
	
	const std::string& getData() const { return m_Data; }
	std::string getUncompressedData() const;
	
private:
	std::string m_Data;
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
	
	const ResourceData* getResource(const ResourcePath& path) const;
	std::size_t resourceCount() const;
	
	template <typename TFunc>
	void forEachResource(const TFunc& func) const
	{
		for (const auto& resource : m_Resources)
		{
			func(ResourcePath(resource.first), resource.second);
		}
	}
	
private:
	std::map<ResourcePath, ResourceData> m_Resources;
};

//--------------------------------------------------------
class Pile
{
public:
	Pile(const std::map<std::string, Chunk>& chunks)
	: m_Chunks(chunks)
	{}
	
	const Chunk* getChunk(const ResourcePath& chunkPath) const;
	Chunk* getChunk(const ResourcePath& chunkPath);
	std::size_t chunkCount() const;
	
	template <typename TFunc>
	void forEachChunk(const TFunc& func) const
	{
		for (const auto& chunk : m_Chunks)
		{
			func(ResourcePath(chunk.first), chunk.second);
		}
	}
	
private:
	std::map<std::string, Chunk> m_Chunks;
};

}
