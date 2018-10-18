#pragma once

#include <string>

namespace stockpile {

class Pile;

//--------------------------------------------------------
class PileHasher
{
public:
	std::string getHash(const Pile& pile) const;
};

}
