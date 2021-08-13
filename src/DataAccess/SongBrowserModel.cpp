#include "DataAccess/SongBrowserModel.hpp"
#include "DataAccess/config.hpp"
#include "logging.hpp"

#include "GlobalNamespace/BeatmapLevelCollection.hpp"
#include "GlobalNamespace/BeatmapLevelPack.hpp"
#include "GlobalNamespace/IBeatmapLevelCollection.hpp"
#include "GlobalNamespace/PlayerDataModel.hpp"
#include "GlobalNamespace/PlayerData.hpp"
#include "GlobalNamespace/PlayerLevelStatsData.hpp"
#include "GlobalNamespace/LevelCollectionNavigationController.hpp"

#include "System/Collections/Generic/HashSet_1.hpp"
#include "System/Diagnostics/Stopwatch.hpp"
#include "System/IO/File.hpp"
#include "System/IO/Path.hpp"
#include "System/TimeSpan.hpp"
#include "System/Linq/Enumerable.hpp"
#include "System/Random.hpp"

#include "songloader/shared/API.hpp"

#include "Utils/ArrayUtil.hpp"
#include "Utils/SongDataCoreUtils.hpp"
#include "Utils/EnumToStringUtils.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Sprite.hpp"

#include <algorithm>
DEFINE_TYPE(SongBrowser, SongBrowserModel);

using Stopwatch = System::Diagnostics::Stopwatch;
using namespace UnityEngine;

std::vector<std::string> split(std::string string, const std::string& delimiter)
{
    // not sure if this is correct, but it might be /shrug
    std::vector<std::string> result = {};

    int pos = string.find(delimiter);
    while (pos != std::string::npos)
    {
        result.push_back(string.substr(0, pos));
        string.erase(0, pos + delimiter.size());    
        pos = string.find(delimiter);
    }

    return result;
}

namespace SongBrowser
{
    std::function<List<GlobalNamespace::IPreviewBeatmapLevel*>*(Array<GlobalNamespace::IPreviewBeatmapLevel*>*)> SongBrowserModel::customFilterHandler;
    std::function<List<GlobalNamespace::IPreviewBeatmapLevel*>*(List<GlobalNamespace::IPreviewBeatmapLevel*>*)> SongBrowserModel::customSortHandler;
    UnorderedEventCallback<const std::vector<GlobalNamespace::CustomPreviewBeatmapLevel*>&> SongBrowserModel::didFinishProcessingSongs;

    void SongBrowserModel::ctor()
    {
        INVOKE_CTOR();
        EPOCH = System::DateTime(1970, 1, 1);
        customFilterHandler = nullptr;
        customSortHandler = nullptr;
    }

    void SongBrowserModel::Init()
    {
        LoadConfig();
        INFO("Config loaded, sort mode is %d", config.sortMode);
    }

    bool SongBrowserModel::get_sortWasMissingData()
    {
        return sortWasMissingData;
    }

    void SongBrowserModel::ToggleInverting()
    {
        config.invertSortResults = !config.invertSortResults;
    }

    void SongBrowserModel::UpdateLevelRecords()
    {
        auto timer = Stopwatch::New_ctor();
        timer->Start();

        // Calculate some information about the custom song dir
        static std::string customSongsPath = RuntimeSongLoader::API::GetCustomLevelsPath();
        static auto customSongsPath_cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>(customSongsPath);
        double currentCustomSongDirLastWriteTIme = (System::IO::File::GetLastWriteTime(customSongsPath_cs) - EPOCH).get_TotalMilliseconds();
        bool customSongDirChanged = false;
        if (customSongDirLastWriteTime != currentCustomSongDirLastWriteTIme)
        {
            customSongDirChanged = true;
            customSongDirLastWriteTime = currentCustomSongDirLastWriteTIme;
        }

        if (!direxists(customSongsPath))
        {
            ERROR("CustomSong directory is missing...");
            return;
        }

        // Map some data for custom songs
        auto lastWriteTimer = Stopwatch::New_ctor();
        lastWriteTimer->Start();
        auto customLevels = RuntimeSongLoader::API::GetLoadedSongs();
        for (auto level : customLevels)
        {
            // If we already know this levelID, don't both updating it.
            // SongLoader should filter duplicates but in case of failure we don't want to crash
            std::string levelID = to_utf8(csstrtostr(level->get_levelID()));
            auto it = cachedLastWriteTimes.find(levelID); 
            if (it != cachedLastWriteTimes.end() || customSongDirChanged)
            {
                double lastWriteTime = GetSongUserDate(level);
                cachedLastWriteTimes[levelID] = lastWriteTime;
            }
        }

        lastWriteTimer->Stop();
        INFO("Determining song download time and determining mappings took %ld ms", lastWriteTimer->get_ElapsedMilliseconds());

        // Update song Infos, directory tree, and sort
        this->UpdatePlayCounts();

        // Signal complete
        if (customLevels.size() > 0)
        {
            didFinishProcessingSongs.invoke(customLevels);
        }

        timer->Stop();
        INFO("Updating songs infos took %ld ms", timer->get_ElapsedMilliseconds());
    }

    void SongBrowserModel::RemoveSongFromLevelCollection(GlobalNamespace::IAnnotatedBeatmapLevelCollection* levelCollection, std::string_view levelId)
    {
        auto levelList = System::Linq::Enumerable::ToList(reinterpret_cast<System::Collections::Generic::IEnumerable_1<GlobalNamespace::IPreviewBeatmapLevel*>*>(levelCollection->get_beatmapLevelCollection()->get_beatmapLevels()));
        
        std::vector<GlobalNamespace::IPreviewBeatmapLevel*> remove = {};
        auto levelID_cs = il2cpp_utils::newcsstr(levelId);
        int length = levelList->get_Count();
        for (int i = 0; i < length; i++)
        {
            auto level = levelList->items->values[i];
            if (level->get_levelID()->Equals(levelID_cs)) remove.push_back(level);
        }

        for (auto level : remove)
        {
            levelList->Remove(level);
        }
    }

    void SongBrowserModel::UpdatePlayCounts()
    {
                // Reset current playcounts
        levelIdToPlayCount.clear();

        // Build a map of levelId to sum of all playcounts and sort.
        auto playerData = ArrayUtil::First(Resources::FindObjectsOfTypeAll<GlobalNamespace::PlayerDataModel*>());
        auto levelsStatsData = playerData->get_playerData()->get_levelsStatsData();
        int length = levelsStatsData->get_Count();
        
        for (int i = 0; i < length; i++)
        {
            auto levelData = levelsStatsData->items->values[i];
            std::string levelID = to_utf8(csstrtostr(levelData->get_levelID()));
            auto itr = levelIdToPlayCount.find(levelID);
            // if not found, just set it
            if (itr == levelIdToPlayCount.end())
            {
                levelIdToPlayCount[levelID] = levelData->get_playCount();
                continue;
            }
            // if found, add to it
            itr->second += levelData->get_playCount();
        }
    }

    void SongBrowserModel::ProcessSongList(GlobalNamespace::IAnnotatedBeatmapLevelCollection* selectedBeatmapCollection, GlobalNamespace::LevelSelectionNavigationController* navController)
    {
        Array<GlobalNamespace::IPreviewBeatmapLevel*>* unsortedSongs = nullptr;
        List<GlobalNamespace::IPreviewBeatmapLevel*>* filteredSongs = nullptr;
        List<GlobalNamespace::IPreviewBeatmapLevel*>* sortedSongs = nullptr;

        // Abort
        if (!selectedBeatmapCollection)
        {
            INFO("Cannot process songs yet, no level collection selected...");
            return;
        }
        std::string selectedCollectionName = selectedBeatmapCollection->get_collectionName() ? to_utf8(csstrtostr(selectedBeatmapCollection->get_collectionName())) : "";

        INFO("Using songs from level collection: %s [num=%lu]", selectedCollectionName.c_str(), selectedBeatmapCollection->get_beatmapLevelCollection()->get_beatmapLevels()->Length());
        unsortedSongs = GetLevelsForLevelCollection(selectedBeatmapCollection);

        // filter
        INFO("Starting filtering songs by %s", SongFilterModeToString(config.filterMode).c_str());
        auto stopwatch = System::Diagnostics::Stopwatch::New_ctor();
        stopwatch->Start();

        if (config.filterMode == SongFilterMode::Requirements)
        {
            auto modList = Modloader::getMods();
            auto itr = modList.find("CustomJSONData");
            if (itr == modList.end() || !itr->second.get_loaded())
            {
                // if cjd not loaded
                config.filterMode = SongFilterMode::None;
            }
        }

        switch (config.filterMode)
        {
            case SongFilterMode::Favorites:
                filteredSongs = FilterFavorites(unsortedSongs);
                break;
            case SongFilterMode::Search:
                filteredSongs = FilterSearch(unsortedSongs);
                break;
            case SongFilterMode::Ranked:
                filteredSongs = FilterRanked(unsortedSongs, true, false);
                break;
            case SongFilterMode::Unranked:
                filteredSongs = FilterRanked(unsortedSongs, false, true);
                break;
            case SongFilterMode::Requirements:
                filteredSongs = FilterRequirements(unsortedSongs);
                break;
            case SongFilterMode::CustomFilter:
                INFO("Song filter mode set to custom. Deferring filter behaviour to another mod.");
                filteredSongs = customFilterHandler ? customFilterHandler(unsortedSongs) : FilterOriginal(unsortedSongs);
                break;
            case SongFilterMode::None:
            default:
                INFO("No song filter selected...");
                filteredSongs = FilterOriginal(unsortedSongs);
                break;
        }

        stopwatch->Stop();
        INFO("Filtering songs took %ld ms", stopwatch->get_ElapsedMilliseconds());

        // sort
        INFO("Starting to sort songs by %s", SongSortModeToString(config.sortMode).c_str());
        stopwatch = stopwatch = System::Diagnostics::Stopwatch::New_ctor();
        stopwatch->Start();
        sortWasMissingData = false;
        switch (config.sortMode)
        {
            case SongSortMode::Original:
                sortedSongs = SortOriginal(filteredSongs);
                break;
            case SongSortMode::Newest:
                sortedSongs = SortNewest(filteredSongs);
                break;
            case SongSortMode::Author:
                sortedSongs = SortAuthor(filteredSongs);
                break;
            case SongSortMode::UpVotes:
                sortedSongs = SortUpVotes(filteredSongs);
                break;
            case SongSortMode::PlayCount:
                sortedSongs = SortBeatSaverPlayCount(filteredSongs);
                break;
            case SongSortMode::Rating:
                sortedSongs = SortBeatSaverRating(filteredSongs);
                break;
            case SongSortMode::Heat:
                sortedSongs = SortBeatSaverHeat(filteredSongs);
                break;
            case SongSortMode::YourPlayCount:
                sortedSongs = SortPlayCount(filteredSongs);
                break;
            case SongSortMode::PP:
                sortedSongs = SortPerformancePoints(filteredSongs);
                break;
            case SongSortMode::Stars:
                sortedSongs = SortStars(filteredSongs);
                break;
            case SongSortMode::Random:
                sortedSongs = SortRandom(filteredSongs);
                break;
            case SongSortMode::Bpm:
                sortedSongs = SortSongBpm(filteredSongs);
                break;
            case SongSortMode::Length:
                sortedSongs = SortSongLength(filteredSongs);
                break;
            case SongSortMode::CustomSort:
                sortedSongs = customSortHandler ? customSortHandler(filteredSongs) : filteredSongs;
                break;
            case SongSortMode::Default:
            default:
                sortedSongs = SortSongName(filteredSongs);
                break;
        }

        if (config.invertSortResults && config.sortMode != SongSortMode::Random)
        {
            sortedSongs->Reverse();
        }

        stopwatch->Stop();
        INFO("Sorting songs took %lu ms", stopwatch->get_ElapsedMilliseconds());

        // Still hacking in a custom level pack
        // Asterisk the pack name so it is identifable as filtered.
        if (!selectedCollectionName.ends_with('*') && config.filterMode != SongFilterMode::None)
        {
            selectedCollectionName += "*";
        }

        // Some level categories have a null cover image, supply something, it won't show it anyway
        auto coverImage = selectedBeatmapCollection->get_coverImage();
        /*
        // we dont have bsml so get fucked
        // if image bad just get fucked lul
        if (!coverImage)
        {
            coverImage = BeatSaberMarkupLanguage.Utilities.ImageResources.BlankSprite;
        }
        */
        INFO("Creating filtered level pack...");
        static auto collectionName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>(filteredSongsCollectionName);
                                                                        
        auto levelCollection = reinterpret_cast<GlobalNamespace::IBeatmapLevelCollection*>(GlobalNamespace::BeatmapLevelCollection::New_ctor(sortedSongs->items));

        auto levelPack = GlobalNamespace::BeatmapLevelPack::New_ctor(   collectionName, 
                                                                        il2cpp_utils::newcsstr(selectedCollectionName),
                                                                        selectedBeatmapCollection->get_collectionName(), 
                                                                        coverImage, 
                                                                        levelCollection);
        /*
         public virtual void SetData(
            IAnnotatedBeatmapLevelCollection annotatedBeatmapLevelCollection,
            bool showPackHeader, bool showPlayerStats, bool showPracticeButton,
            string actionButtonText,
            GameObject noDataInfoPrefab, BeatmapDifficultyMask allowedBeatmapDifficultyMask, BeatmapCharacteristicSO[] notAllowedCharacteristics);
        */
        INFO("Acquiring necessary fields to call SetData(pack)...");
        auto lcnvc = navController->levelCollectionNavigationController;
        auto showPlayerStatsInDetailView = navController->showPlayerStatsInDetailView;
        auto hidePracticeButton = navController->hidePracticeButton;
        auto actionButtonText = navController->actionButtonText;
        auto allowedBeatmapDifficultyMask = navController->allowedBeatmapDifficultyMask;
        auto notAllowedCharacteristics = navController->notAllowedCharacteristics;

        INFO("Calling lcnvc.SetData...");
        lcnvc->SetData(reinterpret_cast<GlobalNamespace::IAnnotatedBeatmapLevelCollection*>(levelPack),
            true,
            showPlayerStatsInDetailView,
            !hidePracticeButton,
            actionButtonText,
            nullptr,
            allowedBeatmapDifficultyMask,
            notAllowedCharacteristics);
    }

    std::string SongBrowserModel::GetSongHash(std::string_view levelId)
    {
        std::string id(levelId);
        if (id.starts_with("custom_level_")) return id.substr(13);
        else return id;
    }

    Array<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::GetLevelsForLevelCollection(GlobalNamespace::IAnnotatedBeatmapLevelCollection* levelCollection)
    {
        return levelCollection->get_beatmapLevelCollection()->get_beatmapLevels();        
    }

#pragma region Filtering
    /* -- Filtering --*/
    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::FilterOriginal(Array<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        List<GlobalNamespace::IPreviewBeatmapLevel*>* filtered = List<GlobalNamespace::IPreviewBeatmapLevel*>::New_ctor();
        ArrayUtil::ForEach(levels, [&](auto x){
            filtered->Add(x);
        });
        return filtered;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::FilterFavorites(Array<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        // new list
        List<GlobalNamespace::IPreviewBeatmapLevel*>* filtered = List<GlobalNamespace::IPreviewBeatmapLevel*>::New_ctor();
        
        // get the favourited level IDs
        auto playerData = ArrayUtil::First(Resources::FindObjectsOfTypeAll<GlobalNamespace::PlayerDataModel*>());
        auto favLevelIds = playerData->get_playerData()->get_favoritesLevelIds();
        // for each item in the list
        ArrayUtil::ForEach(levels, [&](auto x) {
            // if level ID is favd, add to the filtered list
            if (favLevelIds->Contains(x->get_levelID())) filtered->Add(x);
        });
        // return the resulting list  
        return filtered;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::FilterSearch(Array<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        
        // Make sure we can actually search.
        if (config.searchTerms.size() <= 0)
        {
            ERROR("Tried to search for a song with no valid search terms...");
            return SortSongName(FilterOriginal(levels));
        }

        std::string searchTerm = config.searchTerms[0];
        if (searchTerm == "")
        {
            ERROR("Empty search term entered.");
            return SortSongName(FilterOriginal(levels));
        }

        INFO("Filtering song list by search term: %s", searchTerm.c_str());


        List<GlobalNamespace::IPreviewBeatmapLevel*>* filtered = List<GlobalNamespace::IPreviewBeatmapLevel*>::New_ctor();
        auto terms =  split(searchTerm, " ");
        int length = levels->Length();

        // for every level passed in
        for (int i = 0; i < length; i++)
        {
            auto level = levels->values[i];
            std::string searchString = "";
            searchString += to_utf8(csstrtostr(level->get_songName()));
            searchString += ' ';
            searchString += to_utf8(csstrtostr(level->get_songSubName()));
            searchString += ' ';
            searchString += to_utf8(csstrtostr(level->get_songAuthorName()));
            searchString += ' ';
            searchString += to_utf8(csstrtostr(level->get_levelAuthorName()));

            #warning add search for song key with songdatacore
            for (auto term : terms)
            {   
                // if the term is found, it's a match!
                if (searchString.find(term) != std::string::npos) filtered->Add(level);
            }
        }

        return filtered;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::FilterRanked(Array<GlobalNamespace::IPreviewBeatmapLevel*>* levels, bool includeRanked, bool includeUnranked)
    {
        List<GlobalNamespace::IPreviewBeatmapLevel*>* filtered = List<GlobalNamespace::IPreviewBeatmapLevel*>::New_ctor();

        ArrayUtil::ForEach(levels, [&](auto x){
            std::string levelId = to_utf8(csstrtostr(x->get_levelID()));
            auto hash = GetSongHash(levelId);
            auto song = SongDataCoreUtils::GetSong(hash);
            if (!song) return;
            auto diffVec = SongDataCoreUtils::GetDiffVec(song);

            bool isRanked = false;
            for (auto diff : diffVec)
            {
                // if only 1 is ranked
                if (diff->ranked)
                {
                    isRanked = true;
                    break;
                }
            }

            // if ranked and we want ranked, add
            if (isRanked && includeRanked)
            {
                filtered->Add(x);
            }
            // else if not ranked and we want not ranked, add
            else if (!isRanked && includeUnranked)
            {
                filtered->Add(x);
            }
        });
        return filtered;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::FilterRequirements(Array<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning no pinkcore yet so no requirements to be neatly done
        List<GlobalNamespace::IPreviewBeatmapLevel*>* filtered = List<GlobalNamespace::IPreviewBeatmapLevel*>::New_ctor();
        ArrayUtil::ForEach(levels, [&](auto x){
            filtered->Add(x);
        });
        return filtered;
        /*
        return levels.Where(x =>
        {
            if (x is CustomPreviewBeatmapLevel customLevel)
            {
                var saveData = customLevel.standardLevelInfoSaveData as CustomLevelInfoSaveData;

                foreach (CustomLevelInfoSaveData.DifficultyBeatmapSet difficulties in saveData.difficultyBeatmapSets)
                {
                    var hasRequirements = difficulties.difficultyBeatmaps.Any(d =>
                    {
                        var difficulty = d as CustomLevelInfoSaveData.DifficultyBeatmap;

                        if (difficulty == null)
                        {
                            return false;
                        }

                        if (difficulty.customData.ContainsKey("_requirements"))
                        {
                            return ((IList<object>)difficulty.customData["_requirements"]).Count > 0;
                        }

                        return false;
                    });

                    if (hasRequirements)
                    {
                        return true;
                    }
                }
            }

            return false;
        }).ToList();
        */
    }
#pragma endregion

#pragma region Sorting
    /* -- Sorting --*/
    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortOriginal(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortNewest(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        // this is sketch but I think it should work
        std::sort(&levels->items->values[0], &levels->items->values[levels->get_Count()], [&](auto x, auto y) {
            std::string firstLevelID = to_utf8(csstrtostr(x->get_levelID()));
            std::string secondLevelID = to_utf8(csstrtostr(y->get_levelID()));

            auto firstItr = cachedLastWriteTimes.find(firstLevelID);
            auto secondItr = cachedLastWriteTimes.find(secondLevelID);

            if (secondItr == cachedLastWriteTimes.end())
                return false;
            if (firstItr == cachedLastWriteTimes.end())
                return true;
            
            return firstItr->second < secondItr->second;
        });
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortAuthor(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        // this is sketch but I think it should work
        std::sort(&levels->items->values[0], &levels->items->values[levels->get_Count()], [&](auto x, auto y) {
            std::string firstsongAuthorName = to_utf8(csstrtostr(x->get_songAuthorName()));
            std::string secondsongAuthorName = to_utf8(csstrtostr(y->get_songAuthorName()));

            if (firstsongAuthorName == secondsongAuthorName)
            {
                std::string firstsongName = to_utf8(csstrtostr(x->get_songName()));
                std::string secondsongName = to_utf8(csstrtostr(y->get_songName()));
                return firstsongName < secondsongName;
            }
            else return firstsongAuthorName < secondsongAuthorName;
        });
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortPlayCount(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        // this is sketch but I think it should work
        std::sort(&levels->items->values[0], &levels->items->values[levels->get_Count()], [&](auto x, auto y) {
            std::string firstlevelID = to_utf8(csstrtostr(x->get_levelID()));
            std::string secondlevelID = to_utf8(csstrtostr(y->get_levelID()));
            
            auto firstItr = levelIdToPlayCount.find(firstlevelID);
            auto secondItr = levelIdToPlayCount.find(secondlevelID);

            // if played the same amount
            if (firstItr->second == secondItr->second)
            {
                std::string firstsongName = to_utf8(csstrtostr(x->get_songName()));
                std::string secondsongName = to_utf8(csstrtostr(y->get_songName()));
                return firstsongName < secondsongName;
            }
            else return firstItr->second < secondItr->second;
        });
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortPerformancePoints(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        ERROR("No songdata core");
        sortWasMissingData = true;
        return levels;
        /*
        if (!SongDataCore.Plugin.Songs.IsDataAvailable())
        {
            SortWasMissingData = true;
            return levels;
        }

        return levels
            .OrderByDescending(x =>
            {
                var hash = SongBrowserModel.GetSongHash(x.levelID);
                if (SongDataCore.Plugin.Songs.Data.Songs.ContainsKey(hash))
                {
                    return SongDataCore.Plugin.Songs.Data.Songs[hash].diffs.Max(y => y.pp);
                }
                else
                {
                    return 0;
                }
            })
            .ToList();
        */
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortStars(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        ERROR("No songdata core");
        sortWasMissingData = true;
        return levels;
        /*
                    Logger.Info("Sorting song list by star points...");

            if (!SongDataCore.Plugin.Songs.IsDataAvailable())
            {
                SortWasMissingData = true;
                return levels;
            }

            return levels
                .OrderByDescending(x =>
                {
                    var hash = SongBrowserModel.GetSongHash(x.levelID);
                    var stars = 0.0;
                    if (SongDataCore.Plugin.Songs.Data.Songs.ContainsKey(hash))
                    {
                        var diffs = SongDataCore.Plugin.Songs.Data.Songs[hash].diffs;
                        stars = diffs.Max(y => y.star);
                    }

                    //Logger.Debug("Stars={0}", stars);
                    if (stars != 0)
                    {
                        return stars;
                    }

                    if (_settings.invertSortResults)
                    {
                        return double.MaxValue;
                    }
                    else
                    {
                        return double.MinValue;
                    }
                })
                .ToList();
                */
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortRandom(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        INFO("Sorting by random seed (seed=%d)", config.randomSongSeed);

        auto rnd = System::Random::New_ctor(config.randomSongSeed);
        std::sort(&levels->items->values[0], &levels->items->values[levels->get_Count()], [&](auto x, auto y) {
            return rnd->Next() < rnd->Next();
        });

        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortSongName(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        // this is sketch but I think it should work
        std::sort(&levels->items->values[0], &levels->items->values[levels->get_Count()], [&](auto x, auto y) {
            std::string firstsongName = to_utf8(csstrtostr(x->get_songName()));
            std::string secondsongName = to_utf8(csstrtostr(y->get_songName()));

            if (firstsongName == secondsongName)
            {
                std::string firstsongAuthorName = to_utf8(csstrtostr(x->get_songAuthorName()));
                std::string secondsongAuthorName = to_utf8(csstrtostr(y->get_songAuthorName()));
                return firstsongAuthorName < secondsongAuthorName;
            }
            else return firstsongName < secondsongName;
        });
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortSongBpm(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        // this is sketch but I think it should work
        std::sort(&levels->items->values[0], &levels->items->values[levels->get_Count()], [&](auto x, auto y) {
            float firstbpm = x->get_beatsPerMinute();
            float secondbpm = y->get_beatsPerMinute();

            if (firstbpm == secondbpm)
            {
                std::string firstsongName = to_utf8(csstrtostr(x->get_songName()));
                std::string secondsongName = to_utf8(csstrtostr(y->get_songName()));
                return firstsongName < secondsongName;
            }
            else return firstbpm < secondbpm;
        });
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortSongLength(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        // this is sketch but I think it should work
        std::sort(&levels->items->values[0], &levels->items->values[levels->get_Count()], [&](auto x, auto y) {
            float firstLength = x->get_songDuration();
            float secondLength = y->get_songDuration();

            if (firstLength == secondLength)
            {
                std::string firstsongName = to_utf8(csstrtostr(x->get_songName()));
                std::string secondsongName = to_utf8(csstrtostr(y->get_songName()));
                return firstsongName < secondsongName;
            }
            else return firstLength < secondLength;
        });
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortUpVotes(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        ERROR("No songdata core");
        sortWasMissingData = true;
        return levels;
        /*
                    Logger.Info("Sorting song list by BeatSaver UpVotes");

            // Do not always have data when trying to sort by UpVotes
            if (!SongDataCore.Plugin.Songs.IsDataAvailable())
            {
                SortWasMissingData = true;
                return levelIds;
            }

            return levelIds
                .OrderByDescending(x =>
                {
                    var hash = SongBrowserModel.GetSongHash(x.levelID);
                    if (SongDataCore.Plugin.Songs.Data.Songs.ContainsKey(hash))
                    {
                        return SongDataCore.Plugin.Songs.Data.Songs[hash].upVotes;
                    }
                    else
                    {
                        return int.MinValue;
                    }
                })
                .ToList();
                */
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortBeatSaverPlayCount(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        ERROR("No songdata core");
        sortWasMissingData = true;
        return levels;
            // Do not always have data when trying to sort by UpVotes
            /*if (!SongDataCore.Plugin.Songs.IsDataAvailable())
            {
                SortWasMissingData = true;
                return levelIds;
            }
            return levelIds
                .OrderByDescending(x => {
                    var hash = SongBrowserModel.GetSongHash(x.levelID);
                    if (SongDataCore.Plugin.Songs.Data.Songs.ContainsKey(hash))
                    {
                        return SongDataCore.Plugin.Songs.Data.Songs[hash].plays;
                    }
                    else
                    {
                        return int.MinValue;
                    }
                })
                .ToList();*/
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortBeatSaverRating(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        ERROR("No songdata core");
        sortWasMissingData = true;
        return levels;
        /*
                    Logger.Info("Sorting song list by BeatSaver Rating!");

            // Do not always have data when trying to sort by rating
            if (!SongDataCore.Plugin.Songs.IsDataAvailable())
            {
                SortWasMissingData = true;
                return levelIds;
            }

            return levelIds
                .OrderByDescending(x =>
                {
                    var hash = SongBrowserModel.GetSongHash(x.levelID);
                    if (SongDataCore.Plugin.Songs.Data.Songs.ContainsKey(hash))
                    {
                        return SongDataCore.Plugin.Songs.Data.Songs[hash].rating;
                    }
                    else
                    {
                        return int.MinValue;
                    }
                })
                .ToList();
                */
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortBeatSaverHeat(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        ERROR("No songdata core");
        sortWasMissingData = true;
        return levels;
        /*
            Logger.Info("Sorting song list by BeatSaver Heat!");

            // Do not always have data when trying to sort by heat
            if (!SongDataCore.Plugin.Songs.IsDataAvailable())
            {
                SortWasMissingData = true;
                return levelIds;
            }

            return levelIds
                .OrderByDescending(x =>
                {
                    var hash = SongBrowserModel.GetSongHash(x.levelID);
                    if (SongDataCore.Plugin.Songs.Data.Songs.ContainsKey(hash))
                    {
                        return SongDataCore.Plugin.Songs.Data.Songs[hash].heat;
                    }
                    else
                    {
                        return int.MinValue;
                    }
                })
                .ToList();
                */
    }
#pragma endregion

    double SongBrowserModel::GetSongUserDate(GlobalNamespace::CustomPreviewBeatmapLevel* level)
    {
        // not converting to cpp if all I need it for now is just c# stuff
        auto coverPath = System::IO::Path::Combine(level->get_customLevelPath(), level->get_standardLevelInfoSaveData()->get_coverImageFilename());
        System::DateTime lastTime;
        if (System::IO::File::Exists(coverPath))
        {
            auto lastWriteTime = System::IO::File::GetLastWriteTime(coverPath);
            auto lastCreateTime = System::IO::File::GetCreationTime(coverPath);
            lastTime = lastWriteTime > lastCreateTime ? lastWriteTime : lastCreateTime;
        }
        else
        {
            auto lastCreateTime = System::IO::File::GetCreationTime(level->get_customLevelPath());
            lastTime = lastCreateTime;
        }

        return (lastTime - EPOCH).get_TotalMilliseconds();
    }
}