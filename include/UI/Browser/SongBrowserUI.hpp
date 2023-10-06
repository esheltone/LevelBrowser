#pragma once

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/UI/Toggle.hpp"

#include "HMUI/ViewController.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/SimpleDialogPromptViewController.hpp"
#include "GlobalNamespace/IAnnotatedBeatmapLevelCollection.hpp"
#include "GlobalNamespace/StandardLevelDetailView.hpp"
#include "System/Action_1.hpp"

#include "DataAccess/SongBrowserModel.hpp"

#include "UI/Browser/BeatSaberUIController.hpp"
#include "UI/Browser/SongButtons.hpp"

enum UIState {
    Disabled,
    Main,
    SortBy,
    FilterBy
};

DECLARE_CLASS_CODEGEN(SongBrowser::UI, SongBrowserViewController, HMUI::ViewController,

    /*named instance (no specific impl)*/
    
)

DECLARE_CLASS_CODEGEN(SongBrowser::UI, SongBrowserUI, UnityEngine::MonoBehaviour,
    static constexpr const char* Name = "SongBrowserUI";
    static constexpr const float SEGMENT_PERCENT = 0.1f;
    static constexpr const int LIST_ITEMS_VISIBLE_AT_ONCE = 7;
    static constexpr const float CLEAR_BUTTON_Y = -31.5f;
    static constexpr const float BUTTON_ROW_Y = -31.5f;
    
    DECLARE_INSTANCE_FIELD(SongBrowser::DataAccess::BeatSaberUIController*, beatUi);
    DECLARE_INSTANCE_FIELD(SongBrowser::UI::SongBrowserViewController*, viewController);
    
    DECLARE_INSTANCE_FIELD(List<SongBrowser::UI::SongSortButton*>*, sortButtonGroup);
    DECLARE_INSTANCE_FIELD(List<SongBrowser::UI::SongFilterButton*>*, filterButtonGroup);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, sortByButton);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, sortByDisplay);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, filterByButton);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, filterByDisplay);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, randomButton);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, playlistExportButton);

    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, clearSortFilterButton);
    
    DECLARE_INSTANCE_FIELD(GlobalNamespace::SimpleDialogPromptViewController*, deleteDialog);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, deleteButton);

    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, pageUpFastButton);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, pageDownFastButton);

    DECLARE_INSTANCE_FIELD(UnityEngine::RectTransform*, ppStatButton);
    DECLARE_INSTANCE_FIELD(UnityEngine::RectTransform*, starStatButton);
    DECLARE_INSTANCE_FIELD(UnityEngine::RectTransform*, njsStatButton);
    DECLARE_INSTANCE_FIELD(UnityEngine::RectTransform*, noteJumpStartBeatOffsetLabel);
    
    DECLARE_INSTANCE_FIELD(GlobalNamespace::IAnnotatedBeatmapLevelCollection*, lastLevelCollection);
    DECLARE_INSTANCE_FIELD(bool, selectingCategory);
    
    DECLARE_INSTANCE_FIELD(SongBrowser::SongBrowserModel*, model);
    
    DECLARE_CTOR(ctor);
    
    bool uiCreated = false;
    UIState currentUiState = UIState::Disabled;
    bool asyncUpdating = false;
    
    //DECLARE_CTOR(ctor);
    public:
        void Show();
        void Hide(bool dontHideFields = false);
        void UpdateLevelDataModel();
        bool UpdateLevelCollectionSelection();
        void RefreshSongList();
        void CreateUI(GlobalNamespace::MainMenuViewController::MenuButton mode);
        /* --Creation-- */
        void CreateOuterUI();
        void CreateSortButtons();
        void CreateFilterButtons();
        void CreateFastPageButtons();
        void CreateDeleteUI();

        /* --Modification-- */
        void ModifySongStatsPanel();
        void ResizeSongUI();
        void InstallHandlers();
        void OnDidSelectLevelCategory(GlobalNamespace::SelectLevelCategoryViewController* viewController, GlobalNamespace::SelectLevelCategoryViewController::LevelCategory levelCategory);
        void OnDidFavoriteToggleChangeEvent(GlobalNamespace::StandardLevelDetailView* arg1, UnityEngine::UI::Toggle* arg2);
        custom_types::Helpers::Coroutine AsyncForceScrollToPosition(float position);
        custom_types::Helpers::Coroutine AsyncWaitForSongUIUpdate();
        void RefreshSongUI(bool scrollToLevel = true);
        void ProcessSongList();
        void CancelFilter();

        void handleDidSelectAnnotatedBeatmapLevelCollection(GlobalNamespace::IAnnotatedBeatmapLevelCollection* annotatedBeatmapLevelCollection);
        void _levelFilteringNavController_didSelectAnnotatedBeatmapLevelCollectionEvent(GlobalNamespace::LevelFilteringNavigationController* arg1, GlobalNamespace::IAnnotatedBeatmapLevelCollection* arg2,
                UnityEngine::GameObject* arg3, GlobalNamespace::BeatmapCharacteristicSO* arg4);
        void SelectLevelCollection(GlobalNamespace::IAnnotatedBeatmapLevelCollection* levelCollection);
        custom_types::Helpers::Coroutine ProcessSongListEndOfFrame();
        custom_types::Helpers::Coroutine RefreshSongListEndOfFrame();
        /* --Events-- */
        void OnClearButtonClickEvent();
        void OnSortButtonClickEvent(SongSortMode sortMode);
        void OnFilterButtonClickEvent(SongFilterMode mode);
        void OnSearchButtonClickEvent();
        void OnDidSelectLevelEvent(GlobalNamespace::LevelCollectionViewController* view, GlobalNamespace::IPreviewBeatmapLevel* level);
        void OnDidSelectBeatmapCharacteristic(GlobalNamespace::BeatmapCharacteristicSegmentedControlController* view, GlobalNamespace::BeatmapCharacteristicSO* bc);
        void OnDidChangeDifficultyEvent(GlobalNamespace::StandardLevelDetailViewController* view, GlobalNamespace::IDifficultyBeatmap* beatmap);
        void OnDidPresentContentEvent(GlobalNamespace::StandardLevelDetailViewController* view, GlobalNamespace::StandardLevelDetailViewController::ContentType type);
        void OnDidPressActionButton(GlobalNamespace::LevelCollectionNavigationController* view);
        void HandleDidSelectLevelRow(GlobalNamespace::IPreviewBeatmapLevel* level);
        void HandleDeleteSelectedLevel();
        void ShowSearchKeyboard();
        void ShowInputKeyboard(std::function<void(Il2CppString*)> enterPressedHandler);
        void SearchViewControllerSearchButtonPressed(std::string searchFor);
        void CreatePlaylistButtonPressed(Il2CppString* playlistName);
        void JumpSongList(int numJumps, float segmentPercent);
        void RefreshScoreSaberData(GlobalNamespace::IPreviewBeatmapLevel* level);
        void RefreshNoteJumpSpeed(float noteJumpMovementSpeed, float noteJumpStartBeatOffset);
        void RefreshQuickScrollButtons();
        custom_types::Helpers::Coroutine RefreshQuickScrollButtonsAsync();
        void UpdateDeleteButtonState(Il2CppString* levelId);
        void SetVisibility(bool visible, bool fieldsVisibility = true);
        void RefreshOuterUIState(UIState state);
        void RefreshCurrentSelectionDisplay();
        void RefreshSortButtonUI();
)   

