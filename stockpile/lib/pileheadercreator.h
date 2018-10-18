#pragma once

namespace stockpile {

struct PileHeader;
class Pile;

//--------------------------------------------------------
class PileHeaderCreator
{
public:
	PileHeader createHeader(const Pile& pile) const;
};

}
