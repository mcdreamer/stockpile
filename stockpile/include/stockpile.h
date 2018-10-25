#pragma once

#include "pile.h"

#include <string>
#include <memory>

namespace stockpile {

std::unique_ptr<Pile> loadPile(const std::string& pilePath);

Pile createPileFromDefinitionFile(const std::string& definitionPath, const std::string& resourceRoot);
void createPileFromDefinitionFileAndWrite(const std::string& definitionPath, const std::string& resourceRoot, const std::string& outputPath);

}
