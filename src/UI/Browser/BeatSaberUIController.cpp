#include "UI/Browser/BeatSaberUIController.hpp"

DEFINE_TYPE(SongBrowser::DataAccess, BeatSaberUIController);

namespace SongBrowser::DataAccess
{
    void BeatSaberUIController::ctor(GlobalNamespace::LevelSelectionFlowCoordinator* flowCoordinator)
    {
        #warning not implemented
    }

    GlobalNamespace::IBeatmapLevelPack* BeatSaberUIController::GetCurrentSelectedLevelPack()
    {
        #warning not implemented
        return nullptr;
    }

    GlobalNamespace::IPlaylist* BeatSaberUIController::GetCurrentSelectedPlaylist()
    {
        #warning not implemented
        return nullptr;
    }

    GlobalNamespace::IAnnotatedBeatmapLevelCollection* BeatSaberUIController::GetCurrentSelectedAnnotatedBeatmapLevelCollection()
    {
        #warning not implemented
        return nullptr;
    }

    GlobalNamespace::IAnnotatedBeatmapLevelCollection* BeatSaberUIController::GetLevelCollectionByName(const std::string& levelCollectionName)
    {
        #warning not implemented
        return nullptr;
    }

    Array<GlobalNamespace::IPreviewBeatmapLevel*>* BeatSaberUIController::GetCurrentLevelCollectionLevels()
    {
        #warning not implemented
        return nullptr;
    }

    bool BeatSaberUIController::SelectLevelCategory(const std::string& levelCategoryName)
    {
        #warning not implemented
        return false;
    }

    void BeatSaberUIController::SelectLevelCollection(const std::string& levelCollectionName)
    {
        #warning not implemented
    }

    void BeatSaberUIController::SelectAndScrollToLevel(const std::string& levelID)
    {
        #warning not implemented
    }

    void BeatSaberUIController::ScrollToLevelByRow(const int& selectedIndex)
    {
        #warning not implemented
    }

    void BeatSaberUIController::RefreshSongList(const std::string& currentSelectedLevelId, bool scrollToLevel)
    {
        #warning not implemented
    }
}