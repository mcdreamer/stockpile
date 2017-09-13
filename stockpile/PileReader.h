#pragma once

#include <string>

namespace stockpile {

class Pile;

//--------------------------------------------------------
class PileReader
{
public:
	Pile readPile(const std::string& path) const;
};

}
