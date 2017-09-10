#include <iostream>

#include "stockpile.h"
#include "snappy.h"
#include "json.hpp"
#include "args.hxx"

//--------------------------------------------------------
int main(int argc, const char * argv[])
{
	std::cout << "stockpile v0" << std::endl;
	
	args::ArgumentParser parser("stockpile", "This goes after the options.");
	args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
	args::ValueFlag<std::string> foo(parser, "integer", "The integer flag", {'f'});
	args::ValueFlag<std::string> out(parser, "out", "The integer flag", {'o'});
	
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
	
	if (foo)
	{
		std::cerr << foo << std::endl;
	}
	
	nlohmann::json j;
	std::ifstream i(foo.Get());
	i >> j;
	
	const auto file = j["file"].get<std::string>();
	
	std::ifstream i2(file, std::ios::binary | std::ios::ate);
	auto pos = i2.tellg();
	
	std::vector<char>  result(pos);
	
	i2.seekg(0, std::ios::beg);
	i2.read(&result[0], pos);
	
	std::string output;
	
	snappy::Compress(&result[0], result.size(), &output);

	std::ofstream i3;
	i3.open("/Users/andrew/src/stockpile/stockpile/stockpile/out.pile");
	i3 << output;
	i3.close();

	std::cerr << std::string(result.begin(), result.end()) << std::endl;
	std::cerr << output << std::endl;
	
	std::string reversed;
	snappy::Uncompress(output.data(), output.size(), &reversed);
	
	std::cerr << reversed << std::endl;
	

	
	return 0;
}
