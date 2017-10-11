#include <iostream>

#include "stockpile.h"
#include "pilewriter.h"
#include "piledefinitionloader.h"
#include "piledefinition.h"
#include "pilecreator.h"

#include "args.hxx"

namespace stockpile
{
	//-----------------------------------------------------------------
	Pile createPileFromDefinitionFile(const std::string& definitionPath)
	{
		PileDefinitionLoader loader;
		PileCreator creator;

		const auto pileDef = loader.loadFromFile(definitionPath);

		return creator.createPile(*pileDef);
	}
}

//--------------------------------------------------------
int main(int argc, const char * argv[])
{
	args::ArgumentParser parser("", "");
	args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help"} );
	args::ValueFlag<std::string> input(parser, "input", "The integer flag", {'i', "input" });
	args::ValueFlag<std::string> output(parser, "output", "The integer flag", {'o', "output" });

	try
	{
		parser.ParseCLI(argc, argv);
	}
	catch (args::Help)
	{
		std::cout << parser;

		return 0;
	}
	catch (args::ParseError e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;

		return 1;
	}

	if (input && output)
	{
		const auto pile = stockpile::createPileFromDefinitionFile(input.Get());

		stockpile::PileWriter pileWriter;
		pileWriter.writePile(pile, output.Get());
	}
	else
	{
		std::cout << parser;
	}

	return 0;
}
