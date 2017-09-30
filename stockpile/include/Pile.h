#pragma once

#include "resourcepath.h"

#include <string>
#include <string_view>
#include <map>
#include <memory>
#include <functional>

namespace stockpile {

//--------------------------------------------------------
class ResourceData
{
public:
	ResourceData()
	{}
	
	ResourceData(const std::string_view& data)
	: m_Data(data)
	{}
	
	const std::string_view& getData() const { return m_Data; }
	bool empty() const { return m_Data.empty(); };
	
private:
	std::string_view m_Data;
};

//--------------------------------------------------------
class Chunk
{
public:
	using VisitResourceFunc = std::function<void (const ResourcePath&, ResourceData)>;

	Chunk()
	{}
	
	Chunk(const std::map<ResourcePath, std::string>& resources)
	: m_Resources(resources)
	{}
	
	ResourceData getResource(const ResourcePath& path) const;
	std::size_t resourceCount() const;
	void forEachResource(const VisitResourceFunc& func) const;
	
private:
	std::map<ResourcePath, std::string> m_Resources;
};

//--------------------------------------------------------
class Pile
{
public:
	using VisitChunkFunc = std::function<void (const ResourcePath&, const Chunk& chunk)>;

	Pile()
	{}

	Pile(const std::map<ResourcePath, Chunk>& chunks)
	: m_Chunks(chunks)
	{}
	
	const Chunk* getChunk(const ResourcePath& chunkPath) const;
	Chunk* getChunk(const ResourcePath& chunkPath);
	std::size_t chunkCount() const;
	void forEachChunk(const VisitChunkFunc& func) const;
	
private:
	std::map<ResourcePath, Chunk> m_Chunks;
};

}
