#include "Pile.h"

#include "snappy.h"

namespace stockpile {

//--------------------------------------------------------
std::string ResourceData::getUncompressedData() const
{
	std::string uncompressed;
	snappy::Uncompress(m_Data.data(), m_Data.size(), &uncompressed);
	return uncompressed;
}

//--------------------------------------------------------
const ResourceData* Chunk::getResource(const ResourcePath& path) const
{
	auto it = m_Resources.find(path);
	return it != m_Resources.end() ? &it->second : nullptr;
}

//--------------------------------------------------------
std::size_t Chunk::resourceCount() const
{
	return m_Resources.size();
}

namespace
{
	//--------------------------------------------------------
	template <typename TChunkMap>
	auto* getChunkInternal(TChunkMap& chunkMap, const ResourcePath& chunkPath)
	{
		auto it = chunkMap.find(chunkPath.toString());
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

}
