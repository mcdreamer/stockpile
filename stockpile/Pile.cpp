#include "pile.h"

namespace stockpile {

//--------------------------------------------------------
ResourceData Chunk::getResource(const ResourcePath& path) const
{
	auto it = m_Resources.find(path);
	return it != m_Resources.end() ? std::string_view(&it->second[0], it->second.size()) : ResourceData();
}

//--------------------------------------------------------
std::size_t Chunk::resourceCount() const
{
	return m_Resources.size();
}

//--------------------------------------------------------
void Chunk::forEachResource(const VisitResourceFunc& func) const
{
	for (const auto& resource : m_Resources)
	{
		func(resource.first, std::string_view(&resource.second[0], resource.second.size()));
	}
}

namespace
{
	//--------------------------------------------------------
	template <typename TChunkMap>
	auto* getChunkInternal(TChunkMap& chunkMap, const ResourcePath& chunkPath)
	{
		auto it = chunkMap.find(chunkPath);
		return it != chunkMap.end() ? &it->second : nullptr;
	}
}

//--------------------------------------------------------
const Chunk* Pile::getChunk(const ResourcePath& chunkPath) const
{
	return getChunkInternal(m_Chunks, chunkPath);
}
	
//--------------------------------------------------------
Chunk* Pile::getChunk(const ResourcePath& chunkPath)
{
	return getChunkInternal(m_Chunks, chunkPath);
}

//--------------------------------------------------------
std::size_t Pile::chunkCount() const
{
	return m_Chunks.size();
}

//--------------------------------------------------------
void Pile::forEachChunk(const VisitChunkFunc& func) const
{
	for (const auto& chunk : m_Chunks)
	{
		func(chunk.first, chunk.second);
	}
}

}
