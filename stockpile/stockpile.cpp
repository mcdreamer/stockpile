#include "stockpile.h"
#include "PileReader.h"

namespace stockpile {

//-----------------------------------------------------------------
std::unique_ptr<Pile> loadPile(const std::string& pilePath)
{
	PileReader reader;
	return reader.readPile(pilePath);
}

}
