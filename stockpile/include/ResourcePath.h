#pragma once

#include <string>

namespace stockpile {

//--------------------------------------------------------
class ResourcePath
{
public:
	explicit ResourcePath(const std::string& path)
	: m_Path(path)
	{}
	
	bool		isEmpty() const;
	bool		operator==(const ResourcePath& other) const;
	bool		operator<(const ResourcePath& other) const;
	std::string	toString() const;
	
private:
	std::string	m_Path;
};

}
