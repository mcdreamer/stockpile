#pragma once

#include "pile.h"

#include <string>
#include <memory>

namespace stockpile {

std::unique_ptr<Pile> loadPile(const std::string& pilePath);

}
