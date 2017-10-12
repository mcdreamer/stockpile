#pragma once

#include "resourcepath.h"

#include <string>
#include <memory>
#include <functional>

namespace stockpile {

class ChunkData;
class PileData;

//--------------------------------------------------------
class ResourceData
{
public:
	ResourceData()
	: m_Data(nullptr), m_Size(0)
	{}

	ResourceData(const char* data, const int size)
	: m_Data(data), m_Size(size)
	{}

	const char* getData() const { return m_Data; }
	int getSize() const { return m_Size; }
	bool empty() const { return !m_Data; };

private:
	const char* m_Data;
	int m_Size;
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
