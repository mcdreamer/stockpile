#include "pile.h"
#include "piledata.h"

namespace stockpile {

//--------------------------------------------------------
Chunk::Chunk(std::unique_ptr<ChunkData>& data)
: m_Data(data.release())
{}

//--------------------------------------------------------
Chunk::~Chunk()
{}

//--------------------------------------------------------
ResourceData Chunk::getResource(const ResourcePath& path) const
{
	auto it = std::find_if(m_Data->resources.begin(), m_Data->resources.end(), [&](const auto& a) { return a.path == path; });
	return it != m_Data->resources.end() ? ResourceData(&m_Data->data[it->startPos], it->length) : ResourceData();
}

//--------------------------------------------------------
std::size_t Chunk::resourceCount() const
{
	return m_Data->resources.size();
}

//--------------------------------------------------------
void Chunk::forEachResource(const VisitResourceFunc& func) const
{
	for (const auto& resource : m_Data->resources)
	{
		func(resource.path, ResourceData(&m_Data->data[resource.startPos], resource.length));
	}
}

namespace
{
	//--------------------------------------------------------
	template <typename TChunkMap>
	auto* getChunkInternal(TChunkMap& chunkMap, const ResourcePath& chunkPath)
	{
		auto it = chunkMap.find(chunkPath);
		return it != chunkMap.end() ? it->second.get() : nullptr;
	}
}

//--------------------------------------------------------
Pile::Pile(std::unique_ptr<PileData>& data)
: m_Data(data.release())
{}

//--------------------------------------------------------
Pile::Pile(Pile&& other)
: m_Data(other.m_Data.release())
{}

//--------------------------------------------------------
Pile::~Pile()
{}

//--------------------------------------------------------
const Chunk* Pile::getChunk(const ResourcePath& chunkPath) const
{
	return getChunkInternal(m_Data->chunks, chunkPath);
}
	
//--------------------------------------------------------
Chunk* Pile::getChunk(const ResourcePath& chunkPath)
{
	return getChunkInternal(m_Data->chunks, chunkPath);
}

//--------------------------------------------------------
std::size_t Pile::chunkCount() const
{
	return m_Data->chunks.size();
}

//--------------------------------------------------------
void Pile::forEachChunk(const VisitChunkFunc& func) const
{
	for (const auto& chunk : m_Data->chunks)
	{
		func(chunk.first, *chunk.second);
	}
}

}
