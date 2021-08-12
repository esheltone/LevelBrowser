#pragma once
#include <string>
#include "DataAccess/config.hpp"

std::string SongFilterModeToString(const SongFilterMode& mode);
std::string SongSortModeToString(const SongSortMode& mode);

std::string LevelCategoryToString(int cat);
int StringToLevelCategory(std::string_view str);