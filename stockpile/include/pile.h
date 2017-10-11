#pragma once

#include "resourcepath.h"

#include <string>
#include <string_view>
#include <map>
#include <memory>
#include <functional>
#include <vector>

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
struct ChunkResourceAddress
{
	ChunkResourceAddress()
	: startPos(0), length(0)
	{}
	
	ChunkResourceAddress(const ResourcePath& path_, int startPos_, int length_)
	: path(path_), startPos(startPos_), length(length_)
	{}

	ResourcePath path;
	int startPos;
	int length;
};

using ChunkResourceAddressArray = std::vector<ChunkResourceAddress>;

//--------------------------------------------------------
struct ChunkData
{
	ChunkResourceAddressArray resources;
	std::string	data;
};

//--------------------------------------------------------
class Chunk
{
public:
	using VisitResourceFunc = std::function<void (const ResourcePath&, ResourceData)>;

	Chunk(std::unique_ptr<ChunkData>& data);
	~Chunk();
	
	ResourceData getResource(const ResourcePath& path) const;
	std::size_t resourceCount() const;
	void forEachResource(const VisitResourceFunc& func) const;
	
private:
	std::unique_ptr<ChunkData> m_Data;
};

//--------------------------------------------------------
struct PileData
{
	std::map<ResourcePath, std::unique_ptr<Chunk>> chunks;
};

//--------------------------------------------------------
class Pile
{
public:
	using VisitChunkFunc = std::function<void (const ResourcePath&, const Chunk& chunk)>;

	Pile(std::unique_ptr<PileData>& data);
	Pile(Pile&& other);
	~Pile();
	
	const Chunk* getChunk(const ResourcePath& chunkPath) const;
	Chunk* getChunk(const ResourcePath& chunkPath);
	std::size_t chunkCount() const;
	void forEachChunk(const VisitChunkFunc& func) const;
	
private:
	std::unique_ptr<PileData> m_Data;
};

}
