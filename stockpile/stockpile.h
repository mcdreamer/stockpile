#pragma once

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

}
