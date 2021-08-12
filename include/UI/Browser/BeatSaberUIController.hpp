#pragma once

#include "custom-types/shared/macros.hpp"
#include "GlobalNamespace/LevelSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/LevelSelectionNavigationController.hpp"
#include "GlobalNamespace/LevelFilteringNavigationController.hpp"
#include "GlobalNamespace/LevelCollectionNavigationController.hpp"
#include "GlobalNamespace/LevelCollectionViewController.hpp"
#include "GlobalNamespace/LevelCollectionTableView.hpp"
#include "GlobalNamespace/StandardLevelDetailViewController.hpp"
#include "GlobalNamespace/StandardLevelDetailView.hpp"
#include "GlobalNamespace/BeatmapDifficultySegmentedControlController.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSegmentedControlController.hpp"
#include "GlobalNamespace/AnnotatedBeatmapLevelCollectionsViewController.hpp"
#include "GlobalNamespace/SimpleDialogPromptViewController.hpp"
#include "GlobalNamespace/BeatmapLevelsModel.hpp"
#include "HMUI/ScreenSystem.hpp"

#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/RectTransform.hpp"

#include "GlobalNamespace/IBeatmapLevelPack.hpp"
#include "GlobalNamespace/IAnnotatedBeatmapLevelCollection.hpp"
#include "GlobalNamespace/IPlaylist.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"

DECLARE_CLASS_CODEGEN(SongBrowser::DataAccess, BeatSaberUIController, Il2CppObject,
    DECLARE_INSTANCE_FIELD(GlobalNamespace::LevelSelectionFlowCoordinator*, LevelSelectionFlowCoordinator);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::LevelSelectionNavigationController*, LevelSelectionNavigationController);

    DECLARE_INSTANCE_FIELD(GlobalNamespace::LevelFilteringNavigationController*, LevelFilteringNavigationController);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::LevelCollectionNavigationController*, LevelCollectionNavigationController);

    DECLARE_INSTANCE_FIELD(GlobalNamespace::LevelCollectionViewController*, LevelCollectionViewController);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::LevelCollectionTableView*, LevelCollectionTableView);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::StandardLevelDetailViewController*, LevelDetailViewController);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::StandardLevelDetailView*, StandardLevelDetailView);

    DECLARE_INSTANCE_FIELD(GlobalNamespace::BeatmapDifficultySegmentedControlController*, LevelDifficultyViewController);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::BeatmapCharacteristicSegmentedControlController*, BeatmapCharacteristicSelectionViewController);

    DECLARE_INSTANCE_FIELD(GlobalNamespace::AnnotatedBeatmapLevelCollectionsViewController*, AnnotatedBeatmapLevelCollectionsViewController);

    DECLARE_INSTANCE_FIELD(UnityEngine::RectTransform*, LevelCollectionTableViewTransform);

    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, TableViewPageUpButton);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, TableViewPageDownButton);

    DECLARE_INSTANCE_FIELD(UnityEngine::RectTransform*, ActionButtons);

    DECLARE_INSTANCE_FIELD(HMUI::ScreenSystem*, ScreenSystem);

    DECLARE_INSTANCE_FIELD(GlobalNamespace::SimpleDialogPromptViewController*, SimpleDialogPromptViewControllerPrefab);

    DECLARE_INSTANCE_FIELD(GlobalNamespace::BeatmapLevelsModel*, BeatmapLevelsModel);

    DECLARE_CTOR(ctor, GlobalNamespace::LevelSelectionFlowCoordinator* flowCoordinator);
    
    private:
        bool detectedTwitchPluginQueue = false;
        bool checkedForTwitchPlugin = false;

        GlobalNamespace::IBeatmapLevelPack* GetCurrentSelectedLevelPack();
        GlobalNamespace::IPlaylist* GetCurrentSelectedPlaylist();
    public:
        GlobalNamespace::IAnnotatedBeatmapLevelCollection* GetCurrentSelectedAnnotatedBeatmapLevelCollection();
        GlobalNamespace::IAnnotatedBeatmapLevelCollection* GetLevelCollectionByName(const std::string& levelCollectionName);
        Array<GlobalNamespace::IPreviewBeatmapLevel*>* GetCurrentLevelCollectionLevels();
        bool SelectLevelCategory(const std::string& levelCategoryName);
        void SelectLevelCollection(const std::string& levelCollectionName);
        void SelectAndScrollToLevel(const std::string& levelID);
        void ScrollToLevelByRow(const int& selectedIndex);
        void RefreshSongList(const std::string& currentSelectedLevelId, bool scrollToLevel = true);
)