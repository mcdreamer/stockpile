#pragma once

#include "PileDefinition.h"
#include "Pile.h"
#include "PileDefinitionLoader.h"
#include "PileCreator.h"
#include "PileReader.h"

#include <string>

namespace stockpile {

//-----------------------------------------------------------------
Pile loadPile(const std::string& pilePath)
{
	stockpile::PileReader reader;
	return reader.readPile(pilePath);
}

}
