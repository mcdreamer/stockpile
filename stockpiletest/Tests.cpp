#include "Tests.h"
#include "gtest/gtest.h"

#include "stockpile/include/stockpile.h"

#include "stockpile/PileDefinition.h"
#include "stockpile/PileDefinitionLoader.h"
#include "stockpile/PileCreator.h"
#include "stockpile/PileWriter.h"
#include "stockpile/PileReader.h"
#include "stockpile/PileHasher.h"

#include <memory>

//--------------------------------------------------------
TEST(PileDefinitionLoaderTest,testLoadFromPath)
{
	stockpile::PileDefinitionLoader loader;
	auto pileDef = loader.loadFromFile("pile.json");
	ASSERT_TRUE(pileDef.get());
	
	ASSERT_EQ((size_t)1, pileDef->getChunkDefinitions().size());
	
	const auto& chunkDef = pileDef->getChunkDefinitions().front();
	EXPECT_EQ(std::string("testchunk"), chunkDef.getChunkName());
	ASSERT_EQ((size_t)2, chunkDef.getResourcesDefinitions().size());
	
	EXPECT_EQ(std::string("test.txt"), chunkDef.getResourcesDefinitions()[0].getFilename());
	EXPECT_EQ(stockpile::ResourcePath("testdata"), chunkDef.getResourcesDefinitions()[0].getTargetPath());
	
	EXPECT_EQ(std::string("test1.txt"), chunkDef.getResourcesDefinitions()[1].getFilename());
	EXPECT_EQ(stockpile::ResourcePath("test1.txt"), chunkDef.getResourcesDefinitions()[1].getTargetPath());
}

//--------------------------------------------------------
TEST(PileCreator,testCreatePile)
{
	stockpile::PileDefinitionLoader loader;
	auto pileDef = loader.loadFromFile("pile.json");
	ASSERT_TRUE(pileDef.get());
	
	stockpile::PileCreator creator;
	const auto pile = creator.createPile(*pileDef);
	
	ASSERT_EQ((size_t)1, pile.chunkCount());
	
	const auto* chunk = pile.getChunk(stockpile::ResourcePath("testchunk"));
	ASSERT_TRUE(chunk);
	EXPECT_EQ((size_t)2, chunk->resourceCount());
	
	const auto data = chunk->getResource(stockpile::ResourcePath("test1.txt"));
	EXPECT_FALSE(data.empty());
	EXPECT_EQ(std::string("More test data\n"), data.getData());
}

//--------------------------------------------------------
TEST(PileReader,testWritePile)
{
	stockpile::PileDefinitionLoader loader;
	auto pileDef = loader.loadFromFile("pile.json");
	ASSERT_TRUE(pileDef.get());
	
	stockpile::PileCreator creator;
	const auto pile = creator.createPile(*pileDef);

	stockpile::PileWriter writer;
	writer.writePile(pile, "test.pile");
	
	stockpile::PileReader reader;
	const auto loadedPile = reader.readPile("test.pile");
	
	ASSERT_EQ((size_t)1, loadedPile.chunkCount());
	
	const auto* chunk = loadedPile.getChunk(stockpile::ResourcePath("testchunk"));
	ASSERT_TRUE(chunk);
	EXPECT_EQ((size_t)2, chunk->resourceCount());

	const auto data = chunk->getResource(stockpile::ResourcePath("test1.txt"));
	EXPECT_FALSE(data.empty());
	EXPECT_EQ(std::string("More test data\n"), data.getData());
	
	stockpile::PileHasher hasher;
	EXPECT_EQ(hasher.getHash(pile), hasher.getHash(loadedPile));
}
