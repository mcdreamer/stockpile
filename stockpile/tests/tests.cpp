#include "tests.h"
#include "gtest/gtest.h"

#include "include/stockpile.h"

#include "lib/piledefinition.h"
#include "lib/piledefinitionloader.h"
#include "lib/pilecreator.h"
#include "lib/pilewriter.h"
#include "lib/pilereader.h"
#include "lib/pilehasher.h"

#include <memory>

#include <boost/filesystem.hpp>

//--------------------------------------------------------
TEST(PileDefinitionLoaderTest,testLoadFromPath)
{
	stockpile::PileDefinitionLoader loader;
	auto pileDef = loader.loadFromFile("pile.json");
	ASSERT_TRUE(pileDef.get());
	
	ASSERT_EQ((size_t)2, pileDef->getChunkDefinitions().size());
	
	auto chunkDef = pileDef->getChunkDefinitions()[0];
	EXPECT_EQ(std::string("testchunk"), chunkDef.getChunkName());
	EXPECT_EQ(std::string(""), chunkDef.getBasePath());
	ASSERT_EQ((size_t)2, chunkDef.getResourcesDefinitions().size());
	
	EXPECT_EQ(std::string("test.txt"), chunkDef.getResourcesDefinitions()[0].getFilename());
	EXPECT_EQ(stockpile::ResourcePath("testdata"), chunkDef.getResourcesDefinitions()[0].getTargetPath());
	
	EXPECT_EQ(std::string("test1.txt"), chunkDef.getResourcesDefinitions()[1].getFilename());
	EXPECT_EQ(stockpile::ResourcePath("test1.txt"), chunkDef.getResourcesDefinitions()[1].getTargetPath());

	chunkDef = pileDef->getChunkDefinitions()[1];
	EXPECT_EQ(std::string("anothertestchunk"), chunkDef.getChunkName());
	EXPECT_EQ(std::string("moredata"), chunkDef.getBasePath());
	ASSERT_EQ((size_t)1, chunkDef.getResourcesDefinitions().size());

	EXPECT_EQ(std::string("test3.txt"), chunkDef.getResourcesDefinitions()[0].getFilename());
	EXPECT_EQ(stockpile::ResourcePath("test3.txt"), chunkDef.getResourcesDefinitions()[0].getTargetPath());	
}

//--------------------------------------------------------
TEST(PileCreator,testCreatePile)
{
	stockpile::PileDefinitionLoader loader;
	auto pileDef = loader.loadFromFile("pile.json");
	ASSERT_TRUE(pileDef.get());
	
	stockpile::PileCreator creator;
	const auto pile = creator.createPile(*pileDef, boost::filesystem::current_path().c_str());
	
	ASSERT_EQ((size_t)2, pile.chunkCount());
	
	const auto* chunk = pile.getChunk(stockpile::ResourcePath("testchunk"));
	ASSERT_TRUE(chunk);
	EXPECT_EQ((size_t)2, chunk->resourceCount());
	
	auto data = chunk->getResource(stockpile::ResourcePath("test1.txt"));
	EXPECT_FALSE(data.empty());
	EXPECT_EQ(std::string("More test data\n"), data.getData());

	chunk = pile.getChunk(stockpile::ResourcePath("anothertestchunk"));
	ASSERT_TRUE(chunk);
	EXPECT_EQ((size_t)1, chunk->resourceCount());
	
	data = chunk->getResource(stockpile::ResourcePath("test3.txt"));
	EXPECT_FALSE(data.empty());
	EXPECT_EQ(std::string("Hello\n"), data.getData());	
}

//--------------------------------------------------------
TEST(PileReader,testWritePile)
{
	stockpile::PileDefinitionLoader loader;
	auto pileDef = loader.loadFromFile("pile.json");
	ASSERT_TRUE(pileDef.get());
	
	stockpile::PileCreator creator;
	const auto pile = creator.createPile(*pileDef, boost::filesystem::current_path().c_str());

	stockpile::PileWriter writer;
	writer.writePile(pile, "test.pile");
	
	stockpile::PileReader reader;
	const auto loadedPile = reader.readPile("test.pile");
	
	ASSERT_TRUE(loadedPile != nullptr);
	ASSERT_EQ((size_t)2, loadedPile->chunkCount());
	
	const auto* chunk = loadedPile->getChunk(stockpile::ResourcePath("testchunk"));
	ASSERT_TRUE(chunk);
	EXPECT_EQ((size_t)2, chunk->resourceCount());

	const auto data = chunk->getResource(stockpile::ResourcePath("test1.txt"));
	EXPECT_FALSE(data.empty());
	EXPECT_EQ(std::string("More test data\n"), data.getData());
	
	stockpile::PileHasher hasher;
	EXPECT_EQ(hasher.getHash(pile), hasher.getHash(*loadedPile));
}
