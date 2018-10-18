#include <iostream>

#include "include/stockpile.h"

#include "args.hxx"

//--------------------------------------------------------
int main(int argc, const char * argv[])
{
	args::ArgumentParser parser("", "");
	args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help"} );
	args::ValueFlag<std::string> definitionFile(parser, "input", "Input pile definition file name", {'i', "input" });
	args::ValueFlag<std::string> outputPileFile(parser, "output", "Output pile file name", {'o', "output" });

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

	if (definitionFile && outputPileFile)
	{
		stockpile::createPileFromDefinitionFileAndWrite(definitionFile.Get(), outputPileFile.Get());
	}
	else
	{
		std::cout << parser;
	}

	return 0;
}
