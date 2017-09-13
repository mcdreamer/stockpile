#pragma once

#include <string>

namespace stockpile {

class Pile;

//--------------------------------------------------------
class PileWriter
{
public:
	void writePile(const Pile& pile, const std::string& path) const;
};

}
