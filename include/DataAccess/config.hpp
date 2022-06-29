#pragma once

#include <string>
#include <vector>

enum SongSortMode {
    Default,
    Author,
    Original,
    Newest,
    YourPlayCount,
    Difficulty,
    Random,
    PP,
    UpVotes,
    Rating,
    Heat,
    PlayCount,
    Stars,
    Bpm,
    NJS,
    Length,
    
    // Allow mods to extend functionality.
    CustomSort
};

enum SongFilterMode {
    None,
    Favorites,
    Playlist,
    Search,
    Ranked,
    Unranked,
    Requirements,
    Unplayed,

    // For other mods that extend SongBrowser
    CustomFilter
};

struct config_t {
    SongSortMode sortMode = SongSortMode::Default;
    SongFilterMode filterMode = SongFilterMode::None;
    std::vector<std::string> searchTerms = {};
    std::string currentLevelId = "";
    std::string currentDirectory = "";
    std::string currentLevelCollectionName = "";
    std::string currentLevelCategoryName = "";
    bool randomInstantQueue = false;
    bool deleteNumberedSongFolder = true;
    int randomSongSeed;
    bool invertSortResults = false;
};

extern config_t config;

bool LoadConfig();
void SaveConfig();

bool NeedsScoreSaberData(const SongSortMode& mode);
