#include "Utils/EnumToStringUtils.hpp"

std::string SongFilterModeToString(const SongFilterMode& mode)
{
    switch (mode)
    {
        case SongFilterMode::None: return "None";
        case SongFilterMode::Favorites: return "Favorites";
        case SongFilterMode::Playlist: return "Playlist";
        case SongFilterMode::Search: return "Search";
        case SongFilterMode::Ranked: return "Ranked";
        case SongFilterMode::Unranked: return "Unranked";
        case SongFilterMode::Requirements: return "Requirements";
        case SongFilterMode::Unplayed: return "Unplayed";
        case SongFilterMode::CustomFilter: return "Custom";
    }
}

std::string SongSortModeToString(const SongSortMode& mode)
{
    switch (mode)
    {
        case SongSortMode::Default: return "Default";
        case SongSortMode::Author: return "Author";
        case SongSortMode::Original: return "Original";
        case SongSortMode::Newest: return "Newest";
        case SongSortMode::YourPlayCount: return "Plays";
        case SongSortMode::Difficulty: return "Difficulty";
        case SongSortMode::Random: return "Random";
        case SongSortMode::PP: return "PP";
        case SongSortMode::UpVotes: return "UpVotes";
        case SongSortMode::Rating: return "Rating";
        case SongSortMode::Heat: return "Heat";
        case SongSortMode::PlayCount: return "PlayCount";
        case SongSortMode::Stars: return "Stars";
        case SongSortMode::Bpm: return "Bpm";
        case SongSortMode::NJS: return "NJS";
        case SongSortMode::Length: return "Length";
        case SongSortMode::Downloads: return "Downloads";
        case SongSortMode::CustomSort: return "Custom";
    }
}

std::string LevelCategoryToString(int cat)
{
    switch (cat)
    {
        case 0: return "None";
        case 1: return "OstAndExtras";
        case 2: return "MusicPacks";
        case 3: return "CustomSongs";
        case 4: return "Favorites";
        case 5: return "All";
        default: return "";
    }
}

int StringToLevelCategory(std::string_view str)
{
    if (str.size() == 0) return 0;
    switch (str.data()[0])
    {
        case 'N': return 0;
        case 'O': return 1;
        case 'M': return 2;
        case 'C': return 3;
        case 'F': return 4;
        case 'A': return 5;
        default: return 0;
    }
}

std::string BeatmapDifficultyToString(int value)
{
    switch (value)
    {
        case 0: return "Easy";
	    case 1: return "Normal";
	    case 2: return "Hard";
	    case 3: return "Expert";
	    case 4: return "ExpertPlus";
        default: return "Normal";
    }
}