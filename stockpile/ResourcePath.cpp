#include "ResourcePath.h"

namespace stockpile {
	
//--------------------------------------------------------
bool ResourcePath::isEmpty() const
{
	return m_Path.empty();
}

//-----------------------------------------------------------------
bool ResourcePath::operator==(const ResourcePath& other) const
{
	return m_Path == other.m_Path;
}

//-----------------------------------------------------------------
bool ResourcePath::operator<(const ResourcePath& other) const
{
	return m_Path < other.m_Path;
}

//-----------------------------------------------------------------
std::string ResourcePath::toString() const
{
	return m_Path;
}

}
