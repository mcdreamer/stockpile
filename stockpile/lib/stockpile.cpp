#include "include/stockpile.h"

#include "pilereader.h"
#include "piledefinitionloader.h"
#include "piledefinition.h"
#include "pilecreator.h"
#include "pilewriter.h"

namespace stockpile {

//-----------------------------------------------------------------
std::unique_ptr<Pile> loadPile(const std::string& pilePath)
{
	PileReader reader;
	return reader.readPile(pilePath);
}

//-----------------------------------------------------------------
Pile createPileFromDefinitionFile(const std::string& definitionPath)
{
	PileDefinitionLoader loader;
	PileCreator creator;

	const auto pileDef = loader.loadFromFile(definitionPath);

	return creator.createPile(*pileDef);
}

//-----------------------------------------------------------------
void createPileFromDefinitionFileAndWrite(const std::string& definitionPath, const std::string& outputPath)
{
	const auto pile = stockpile::createPileFromDefinitionFile(definitionPath);

	stockpile::PileWriter pileWriter;
	pileWriter.writePile(pile, outputPath);
}

}
