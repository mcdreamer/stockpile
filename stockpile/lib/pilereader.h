#pragma once

#include <string>
#include <memory>

namespace stockpile {

class Pile;

//--------------------------------------------------------
class PileReader
{
public:
	std::unique_ptr<Pile> readPile(const std::string& path) const;
};

}
