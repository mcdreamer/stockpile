#pragma once

#include <string>
#include <memory>

namespace stockpile {

class PileDefinition;

//--------------------------------------------------------
class PileDefinitionLoader
{
public:
	std::unique_ptr<PileDefinition> loadFromFile(const std::string& path) const;
};

}
