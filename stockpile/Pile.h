#pragma once

#include "ResourcePath.h"

#include <string>
#include <vector>
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
