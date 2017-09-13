#include "stockpile.h"
#include "PileReader.h"

namespace stockpile {

//-----------------------------------------------------------------
Pile loadPile(const std::string& pilePath)
{
	stockpile::PileReader reader;
	return reader.readPile(pilePath);
}

}
