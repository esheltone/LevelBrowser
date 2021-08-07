#include "DataAccess/SongBrowserModel.hpp"
#include "DataAccess/config.hpp"

#include "GlobalNamespace/IBeatmapLevelCollection.hpp"

DEFINE_TYPE(SongBrowser, SongBrowserModel);

namespace SongBrowser
{
    void SongBrowserModel::ctor()
    {
        #warning not implemented
        INVOKE_CTOR();
    }

    void SongBrowserModel::Init()
    {
        #warning not implemented

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
        #warning not implemented
    }

    void SongBrowserModel::RemoveSongFromLevelCollection(GlobalNamespace::IAnnotatedBeatmapLevelCollection* levelCollection, std::string_view levelId)
    {
        #warning not implemented
    }

    void SongBrowserModel::UpdatePlayCounts()
    {
        #warning not implemented
    }

    void SongBrowserModel::ProcessSongList(GlobalNamespace::IAnnotatedBeatmapLevelCollection* selectedBeatmapCollection, GlobalNamespace::LevelSelectionNavigationController* navController)
    {
        #warning not implemented
    }

    std::string SongBrowserModel::GetSongHash(std::string_view levelId)
    {
        std::string id(levelId);
        if (id.find("custom_level_")) return id.substr(13);
        else return id;
    }

    Array<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::GetLevelsForLevelCollection(GlobalNamespace::IAnnotatedBeatmapLevelCollection* levelCollection)
    {
        return levelCollection->get_beatmapLevelCollection()->get_beatmapLevels();        
    }

#pragma region Filtering
    /* -- Filtering --*/
    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::FilterFavorites(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning not implemented
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::FilterSearch(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning not implemented
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::FilterRanked(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels, bool includeRanked, bool includeUnranked)
    {
        #warning not implemented
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::FilterRequirements(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning not implemented
        return levels;
    }
#pragma endregion

#pragma region Sorting
    /* -- Sorting --*/
    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortOriginal(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning not implemented
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortNewest(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning not implemented
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortAuthor(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning not implemented
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortPlayCount(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning not implemented
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortPerformancePoints(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning not implemented
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortStars(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning not implemented
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortRandom(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning not implemented
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortSongName(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning not implemented
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortSongBpm(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning not implemented
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortSongLength(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning not implemented
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortUpVotes(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning not implemented
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortBeatSaverPlayCount(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning not implemented
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortBeatSaverRating(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning not implemented
        return levels;
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* SongBrowserModel::SortBeatSaverHeat(List<GlobalNamespace::IPreviewBeatmapLevel*>* levels)
    {
        #warning not implemented
        return levels;
    }
#pragma endregion
}