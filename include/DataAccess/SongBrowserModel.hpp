#pragma once

#include "beatsaber-hook/shared/utils/typedefs-wrappers.hpp"
#include "custom-types/shared/macros.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"
#include <functional>

#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"
#include "GlobalNamespace/IAnnotatedBeatmapLevelCollection.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/CustomPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/LevelSelectionNavigationController.hpp"

#include "System/DateTime.hpp"

#include "song-details/shared/SongDetails.hpp"

#include <string_view>
#include <string>
#include <map>

DECLARE_CLASS_CODEGEN(SongBrowser, SongBrowserModel, Il2CppObject,
    DECLARE_CTOR(ctor);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::BeatmapCharacteristicSO*, currentBeatmapCharacteristicSO);
    DECLARE_INSTANCE_FIELD(System::DateTime, EPOCH);
    DECLARE_INSTANCE_FIELD(double, customSongDirLastWriteTime);

    public:
        static constexpr const char* filteredSongsCollectionName = "custom_levelPack_SongBrowser_FilteredSongPack";
        static constexpr const char* playlistSongsCollectionName = "SongBrowser_PlaylistPack";
        
        static std::function<List<GlobalNamespace::IPreviewBeatmapLevel*>*(Array<GlobalNamespace::IPreviewBeatmapLevel*>*)> customFilterHandler;
        static std::function<List<GlobalNamespace::IPreviewBeatmapLevel*>*(List<GlobalNamespace::IPreviewBeatmapLevel*>*)> customSortHandler;
        static UnorderedEventCallback<const std::vector<GlobalNamespace::CustomPreviewBeatmapLevel*>&> didFinishProcessingSongs;

        std::map<std::string, double> cachedLastWriteTimes = {};
        std::map<std::string, int> levelIdToPlayCount = {};

        bool get_sortWasMissingData();
        void Init();
        void ToggleInverting();
        void UpdateLevelRecords();
        void RemoveSongFromLevelCollection(GlobalNamespace::IAnnotatedBeatmapLevelCollection* levelCollection, std::string_view levelId);
        void UpdatePlayCounts();
        void ProcessSongList(GlobalNamespace::IAnnotatedBeatmapLevelCollection* selectedBeatmapCollection, GlobalNamespace::LevelSelectionNavigationController* navController);
        static Array<GlobalNamespace::IPreviewBeatmapLevel*>* GetLevelsForLevelCollection(GlobalNamespace::IAnnotatedBeatmapLevelCollection* levelCollection);
        static List<GlobalNamespace::IPreviewBeatmapLevel*>* GetLevelsListForLevelCollection(GlobalNamespace::IAnnotatedBeatmapLevelCollection* levelCollection);
        const SongDetailsCache::Song& GetSongForLevel(GlobalNamespace::IPreviewBeatmapLevel* level);
        float lastScrollIndex = 0.0f;
        std::string lastSelectedLevelId = "";
        inline static SongDetailsCache::SongDetails* songDetails;
    private:
        
        /* -- Filtering --*/
        List<GlobalNamespace::IPreviewBeatmapLevel*>* FilterOriginal(Array<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        // not accessible VV
        List<GlobalNamespace::IPreviewBeatmapLevel*>* FilterFavorites(Array<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* FilterSearch(Array<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* FilterRanked(Array<GlobalNamespace::IPreviewBeatmapLevel*>* levels, bool includeRanked, bool includeUnranked);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* FilterRequirements(Array<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* FilterUnplayed(Array<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
		
		std::u16string LevelTitleWithoutBeginningArticle(GlobalNamespace::IPreviewBeatmapLevel* level);
        
		/* -- Sorting --*/
        List<GlobalNamespace::IPreviewBeatmapLevel*>* SortOriginal(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* SortNewest(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* SortAuthor(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* SortPlayCount(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* SortPerformancePoints(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* SortStars(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* SortRandom(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* SortSongName(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* SortSongBpm(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* SortSongNJS(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* SortSongLength(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* SortUpVotes(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        // not accessible VV
        List<GlobalNamespace::IPreviewBeatmapLevel*>* SortBeatSaverPlayCount(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* SortBeatSaverDownloads(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* SortBeatSaverRating(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        List<GlobalNamespace::IPreviewBeatmapLevel*>* SortBeatSaverHeat(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels);
        
        double GetSongUserDate(GlobalNamespace::CustomPreviewBeatmapLevel* level);
        bool sortWasMissingData;
)