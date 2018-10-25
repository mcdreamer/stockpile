#pragma once

namespace stockpile {

class Pile;
class PileDefinition;

//--------------------------------------------------------
class PileCreator
{
public:
	Pile createPile(const PileDefinition& pileDef, const std::string& resourceRoot) const;
};

}
