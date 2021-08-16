#include "DataAccess/config.hpp"
#include "logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

config_t config;

Configuration& getConfig()
{
    static Configuration config({ID, VERSION});
    config.Load();
    return config;
}

#define ADD_STR(str) doc.AddMember(#str, rapidjson::Value(config.str.c_str(), config.str.size(), allocator), allocator)
#define ADD_VAL(val) doc.AddMember(#val, config.val, allocator)

void SaveConfig()
{
    INFO("Saving Configuration...");
    rapidjson::Document& doc = getConfig().config;
    doc.RemoveAllMembers();
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    
    ADD_VAL(sortMode);
    ADD_VAL(filterMode);

    rapidjson::Value array(rapidjson::kArrayType);
    array.SetArray();
    for (auto& t : config.searchTerms)
    {
        rapidjson::Value val = rapidjson::Value(t.c_str(), t.size(), allocator);
        array.PushBack(val, allocator);
    }
    
    doc.AddMember("searchTerms", array, allocator);

    ADD_STR(currentLevelId);
    ADD_STR(currentDirectory);
    ADD_STR(currentLevelCollectionName);
    ADD_STR(currentLevelCategoryName);
    ADD_VAL(randomInstantQueue);
    ADD_VAL(deleteNumberedSongFolder);
    ADD_VAL(randomSongSeed);
    ADD_VAL(invertSortResults);

    getConfig().Write();
    INFO("Saved Configuration!");
}

#define FIND_JSON_VAL(name, getter) \
    auto itr_ ##name = doc.FindMember(#name); \
    if (itr_ ##name != doc.MemberEnd()) { \
        config.name = itr_ ##name->value.getter; \
    } else { \
        foundEverything = false; \
    }

bool LoadConfig()
{
    bool foundEverything = true;
    rapidjson::Document& doc = getConfig().config;

    auto itr_sortMode = doc.FindMember("sortMode");
    if (itr_sortMode != doc.MemberEnd()) {
        config.sortMode = (SongSortMode)itr_sortMode->value.GetInt();
    } else {
        foundEverything = false;
    }

    auto itr_filterMode = doc.FindMember("filterMode");
    if (itr_filterMode != doc.MemberEnd()) {
        config.filterMode = (SongFilterMode)itr_filterMode->value.GetInt();
    } else {
        foundEverything = false;
    }

    auto itr_searchTerms = doc.FindMember("searchTerms");
    if (itr_searchTerms != doc.MemberEnd()) {
        const auto& arr = itr_searchTerms->value.GetArray();
        for (const auto& t : arr)
        {
            config.searchTerms.push_back(t.GetString());
        }
    }

    //FIND_JSON_VAL(searchTerms, );
    FIND_JSON_VAL(currentLevelId, GetString());
    FIND_JSON_VAL(currentDirectory, GetString());
    FIND_JSON_VAL(currentLevelCollectionName, GetString());
    FIND_JSON_VAL(currentLevelCategoryName, GetString());
    FIND_JSON_VAL(randomInstantQueue, GetBool());
    FIND_JSON_VAL(deleteNumberedSongFolder, GetBool());
    FIND_JSON_VAL(randomSongSeed, GetInt());
    FIND_JSON_VAL(invertSortResults, GetBool());

    return foundEverything;
}

bool NeedsScoreSaberData(const SongSortMode& mode)
{
    switch (mode)
    {
        case SongSortMode::UpVotes: [[fallthrough]];
        case SongSortMode::Rating: [[fallthrough]];
        case SongSortMode::PlayCount: [[fallthrough]];
        case SongSortMode::Heat: [[fallthrough]];
        case SongSortMode::PP: [[fallthrough]];
        case SongSortMode::Stars:
            return true;
        default:
            return false;
    }
}