#pragma once

#include "include/ResourcePath.h"

#include <string>
#include <vector>

namespace stockpile {

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

}
