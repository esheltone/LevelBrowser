#include "UI/Browser/SongBrowserUI.hpp"

#include "UnityEngine/Object.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/WaitForEndOfFrame.hpp"

#include "HMUI/CurvedCanvasSettings.hpp"
#include "HMUI/ScrollView.hpp"

#include "GlobalNamespace/SoloFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/PartyFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/MultiplayerLevelSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/LevelParamsPanel.hpp"

#include "Utils/ArrayUtil.hpp"
#include "Utils/UIUtils.hpp"
#include "Utils/SpriteUtils.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "logging.hpp"

#include <map>

using namespace UnityEngine;
using namespace UnityEngine::UI;

DEFINE_TYPE(SongBrowser::UI, SongBrowserUI);
DEFINE_TYPE(SongBrowser::UI, SongBrowserViewController);

namespace SongBrowser::UI
{
    void SongBrowserUI::Show()
    {
        #warning not implemented
    }

    void SongBrowserUI::Hide()
    {
        #warning not implemented
    }

    void SongBrowserUI::UpdateLevelDataModel()
    {
        #warning not implemented
    }

    void SongBrowserUI::UpdateLevelCollectionSelection()
    {
        #warning not implemented
    }

    void SongBrowserUI::RefreshSongList()
    {
        #warning not implemented
    }

    // builds the ui
    void SongBrowserUI::CreateUI(GlobalNamespace::MainMenuViewController::MenuButton mode)
    {
        GlobalNamespace::LevelSelectionFlowCoordinator* flowCoordinator = nullptr;
        switch(mode)
        {
            case GlobalNamespace::MainMenuViewController::MenuButton::SoloFreePlay:
                flowCoordinator = ArrayUtil::Last(Resources::FindObjectsOfTypeAll<GlobalNamespace::SoloFreePlayFlowCoordinator*>());
                break;
            case GlobalNamespace::MainMenuViewController::MenuButton::Party:
                flowCoordinator = ArrayUtil::Last(Resources::FindObjectsOfTypeAll<GlobalNamespace::PartyFreePlayFlowCoordinator*>());
                break;
            case GlobalNamespace::MainMenuViewController::MenuButton::Multiplayer:
                flowCoordinator = ArrayUtil::Last(Resources::FindObjectsOfTypeAll<GlobalNamespace::MultiplayerLevelSelectionFlowCoordinator*>());
                break;
            default:
                return;
        }

        beatUi = *il2cpp_utils::New<SongBrowser::DataAccess::BeatSaberUIController*>(flowCoordinator);
        lastLevelCollection = nullptr;
        
        auto screenContainer = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Transform*>(), [](Transform* x) -> bool {
                static Il2CppString* screenContainerName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("ScreenContainer");
                if (x->get_gameObject()->get_name()->Equals(screenContainerName)) return true;
                return false;
            });
        auto curvedCanvasSettings = screenContainer->GetComponent<HMUI::CurvedCanvasSettings*>();

        if (uiCreated)
        {
            auto vcCanvasSettings = viewController->GetComponent<HMUI::CurvedCanvasSettings*>();
            vcCanvasSettings->SetRadius(curvedCanvasSettings->get_radius());
            return;
        }

        if (viewController)
        {
            Object::Destroy(viewController);
        }

        viewController = UIUtils::CreateCurvedViewController<SongBrowser::UI::SongBrowserViewController*>("SongBrowserViewController", curvedCanvasSettings->get_radius());
        auto rectTransform = viewController->get_rectTransform();
        rectTransform->set_anchorMin(Vector2(0.0f, 0.0f));
        rectTransform->set_anchorMax(Vector2(1.0f, 1.0f));
        rectTransform->set_anchoredPosition(Vector2(0.0f, 0.0f));
        rectTransform->set_sizeDelta(Vector2(curvedCanvasSettings->get_radius(), 25));
        viewController->get_gameObject()->SetActive(true);

        CreateOuterUI();
        CreateSortButtons();
        CreateFilterButtons();
        CreateDeleteUI();
        CreateFastPageButtons();

        InstallHandlers();

        ModifySongStatsPanel();
        ResizeSongUI();

        uiCreated = true;

        RefreshSortButtonUI();
    }
#pragma region creation
    void SongBrowserUI::CreateOuterUI()
    {
        #warning not implemented
        static constexpr const float clearButtonX = -72.5f;
        static constexpr const float clearButtonY = CLEAR_BUTTON_Y;
        static constexpr const float buttonY = BUTTON_ROW_Y;
        static constexpr const float buttonHeight = 5.0f;
        static constexpr const float sortByButtonX = -62.5f + buttonHeight;
        static constexpr const float outerButtonFontSize = 3.0f;
        static constexpr const float displayButtonFontSize = 2.5f;
        static constexpr const float outerButtonWidth = 24.0f;
        static constexpr const float randomButtonWidth = 10.0f;
        
                    // clear button
        clearSortFilterButton = UIUtils::CreateIconButton( "ClearSortAndFilterButton", viewController->get_transform(), "PracticeButton", Vector2(clearButtonX, clearButtonY), Vector2(randomButtonWidth, randomButtonWidth),
            [&]() {
                if (currentUiState == UIState::FilterBy || currentUiState == UIState::SortBy)
                {
                    RefreshOuterUIState(UIState::Main);
                }
                else
                {
                    OnClearButtonClickEvent();
                }
            },
            SpriteUtils::get_XIcon(),
            "Clear");
        UIUtils::SetButtonBackgroundActive(clearSortFilterButton, false);

        // create SortBy button and its display
        float curX = sortByButtonX;

        INFO("Creating Sort By...");
        sortByButton = UIUtils::CreateUIButton("sortBy", viewController->get_transform(), "PracticeButton", Vector2(curX, buttonY), Vector2(outerButtonWidth, buttonHeight), 
            [&]() {
                    RefreshOuterUIState(UIState::SortBy);
        }, "Sort By");
        UIUtils::SetButtonTextSize(sortByButton, outerButtonFontSize);
        UIUtils::ToggleWordWrapping(sortByButton, false);

        curX += outerButtonWidth;

        INFO("Creating Sort By Display...");
        sortByDisplay = UIUtils::CreateUIButton("sortByValue", viewController->get_transform(), "PracticeButton", Vector2(curX, buttonY), Vector2(outerButtonWidth, buttonHeight), 
        [&]() {
            OnSortButtonClickEvent(config.sortMode);
        }, "");
        UIUtils::SetButtonTextSize(sortByDisplay, displayButtonFontSize);
        UIUtils::ToggleWordWrapping(sortByDisplay, false);

        curX += outerButtonWidth;

        // create FilterBy button and its display
        INFO("Creating Filter By...");
        filterByButton = UIUtils::CreateUIButton("filterBy", viewController->get_transform(), "PracticeButton", Vector2(curX, buttonY), Vector2(outerButtonWidth, buttonHeight), 
        [&]() {
            RefreshOuterUIState(UIState::FilterBy);
        }, "Filter By");
        UIUtils::SetButtonTextSize(filterByButton, outerButtonFontSize);
        UIUtils::ToggleWordWrapping(filterByButton, false);

        curX += outerButtonWidth;

        INFO("Creating Filter By Display...");
        filterByDisplay = UIUtils::CreateUIButton("filterValue", viewController->get_transform(), "PracticeButton", Vector2(curX, buttonY), Vector2(outerButtonWidth, buttonHeight), 
        [&]() {
            config.filterMode = SongFilterMode::None;
            CancelFilter();
            ProcessSongList();
            RefreshSongUI();
        }, "");
        UIUtils::SetButtonTextSize(filterByDisplay, displayButtonFontSize);
        UIUtils::ToggleWordWrapping(filterByDisplay, false);

        curX += (outerButtonWidth / 2.0f);

        // random button
        INFO("Creating Random Button...");
        randomButton = UIUtils::CreateIconButton("randomButton", viewController->get_transform(), "PracticeButton", Vector2(curX + (randomButtonWidth / 4.0f), clearButtonY), Vector2(randomButtonWidth, randomButtonWidth), 
        [&]() {
            OnSortButtonClickEvent(SongSortMode::Random);
        }, SpriteUtils::get_RandomIcon(), "Random");
        UIUtils::SetButtonBackgroundActive(randomButton, false);

        curX += (randomButtonWidth / 4.0f) * 2.0f;

        // playlist export
        INFO("Creating playlist export button...");
        playlistExportButton =  UIUtils::CreateIconButton("playlistExportButton", viewController->get_transform(), "PracticeButton", Vector2(curX + (randomButtonWidth / 4.0f), clearButtonY), Vector2(randomButtonWidth, randomButtonWidth), 
        [&]() {
            ShowInputKeyboard(std::bind(&SongBrowserUI::CreatePlaylistButtonPressed, this, std::placeholders::_1));
        }, SpriteUtils::get_PlaylistIcon(), "Export Playlist");
        UIUtils::SetButtonBackgroundActive(playlistExportButton, false);
    }

    void SongBrowserUI::CreateSortButtons()
    {
        INFO("Create sort buttons...");
        static constexpr const float sortButtonFontSize = 2.0f;
        static constexpr const float sortButtonX = -63.0f;
        static constexpr const float sortButtonWidth = 11.75f;
        static constexpr const float buttonSpacing = 0.25f;
        static constexpr const float buttonY = BUTTON_ROW_Y;
        static constexpr const float buttonHeight = 5.0f;

        std::map<std::string, SongSortMode> sortModes = {
            {"Title", SongSortMode::Default},
            {"Author", SongSortMode::Author},
            {"Newest", SongSortMode::Newest},
            {"#Plays", SongSortMode::YourPlayCount},
            {"BPM", SongSortMode::Bpm},
            {"Time", SongSortMode::Length},
            {"PP", SongSortMode::PP},
            {"Stars", SongSortMode::Stars},
            {"UpVotes", SongSortMode::UpVotes},
            {"Rating", SongSortMode::Rating},
            {"Heat", SongSortMode::Heat}
        };

        sortButtonGroup = List<SongSortButton*>::New_ctor();
        int i = 0;
        for (auto& p : sortModes)
        {
            float curButtonX = sortButtonX + (sortButtonWidth * i) + (buttonSpacing * i);
            auto sortButton = *il2cpp_utils::New<SongSortButton*>();
            sortButton->sortMode = p.second;
            sortButton->button = UIUtils::CreateUIButton(string_format("Sort%sButton", p.first.c_str()), viewController->get_transform(), "PracticeButton",
                Vector2(curButtonX, buttonY), Vector2(sortButtonWidth, buttonHeight),
                [&, sortButton]() -> void {
                    OnSortButtonClickEvent(sortButton->sortMode);
                    RefreshOuterUIState(UIState::Main);
                }, p.first);
            UIUtils::SetButtonTextSize(sortButton->button, sortButtonFontSize);
            UIUtils::ToggleWordWrapping(sortButton->button, false);

            sortButtonGroup->Add(sortButton);
            i++;
        }
    }

    void SongBrowserUI::CreateFilterButtons()
    {
        INFO("Creating filter buttons...");
        static constexpr const float filterButtonFontSize = 2.25f;
        static constexpr const float filterButtonX = -63.0f;
        static constexpr const float filterButtonWidth = 14.25f;
        static constexpr const float buttonSpacing = 0.5f;
        static constexpr const float buttonY = BUTTON_ROW_Y;
        static constexpr const float buttonHeight = 5.0f;

        std::map<std::string, SongFilterMode> filterModes = {
            {"Search", SongFilterMode::Search},
            {"Ranked", SongFilterMode::Ranked},
            {"Unranked", SongFilterMode::Unranked},
            {"Requirements", SongFilterMode::Requirements}
        };

        filterButtonGroup = List<SongFilterButton*>::New_ctor();
        int i = 0;
        for (auto& p : filterModes)
        {
            float curButtonX = filterButtonX + (filterButtonWidth * i) + (buttonSpacing * i);
            auto filterButton = *il2cpp_utils::New<SongFilterButton*>();
            filterButton->filterMode = p.second;
            filterButton->button = UIUtils::CreateUIButton(string_format("Filter%sButton", p.first.c_str()), viewController->get_transform(), "PracticeButton",
                Vector2(curButtonX, buttonY), Vector2(filterButtonWidth, buttonHeight),
                [&, filterButton]() -> void {
                    OnFilterButtonClickEvent(filterButton->filterMode);
                    RefreshOuterUIState(UIState::Main);
                }, p.first);
            UIUtils::SetButtonTextSize(filterButton->button, filterButtonFontSize);
            UIUtils::ToggleWordWrapping(filterButton->button, false);

            if (i == 3)
            {
                #warning keeping the req filtering off for now
                filterButton->button->set_interactable(false);
            }

            filterButtonGroup->Add(filterButton);
            i++;
        }
    }

    void SongBrowserUI::CreateFastPageButtons()
    {
        INFO("Creating fast scroll button...");
        pageUpFastButton = UIUtils::CreatePageButton("PageUpFast",
            beatUi->LevelCollectionNavigationController->get_transform(), "UpButton",
            Vector2(2.0f, 24.0f), Vector2(8.0f, 8.0f),
            [&]() -> void {
                JumpSongList(-1, SEGMENT_PERCENT);
            }, SpriteUtils::get_DoubleArrow());

        pageDownFastButton = UIUtils::CreatePageButton("PageDownFast",
            beatUi->LevelCollectionNavigationController->get_transform(), "DownButton",
            Vector2(2.0f, -24.0f), Vector2(8.0f, 8.0f),
            [&]() -> void {
                JumpSongList(1, SEGMENT_PERCENT);
            }, SpriteUtils::get_DoubleArrow());
    }

    void SongBrowserUI::CreateDeleteUI()
    {
        INFO("Creating delete dialog...");
        deleteDialog = Object::Instantiate<GlobalNamespace::SimpleDialogPromptViewController*>(beatUi->SimpleDialogPromptViewControllerPrefab);
        deleteDialog->GetComponent<VRUIControls::VRGraphicRaycaster*>()->physicsRaycaster = UIUtils::get_PhysicsRaycasterWithCache();
        deleteDialog->set_name(il2cpp_utils::newcsstr("DeleteDialogPromptViewController"));
        deleteDialog->get_gameObject()->SetActive(false);

        INFO("Creating delete button...");
        deleteButton = UIUtils::CreateIconButton("DeleteLevelButton", reinterpret_cast<UnityEngine::Transform*>(beatUi->ActionButtons), "PracticeButton", SpriteUtils::get_DeleteIcon(), "Delete Level");
        deleteButton->get_transform()->SetAsFirstSibling();

        std::function<void(void)> fun = std::bind(&SongBrowserUI::HandleDeleteSelectedLevel, this);
        deleteButton->get_onClick()->AddListener(il2cpp_utils::MakeDelegate<Events::UnityAction*>(classof(Events::UnityAction*), fun));
    }
#pragma endregion
    void SongBrowserUI::ModifySongStatsPanel()
    {
        // modify stat panel, inject extra row of stats
        INFO("Resizing Stats Panel...");

        auto statsPanel = beatUi->StandardLevelDetailView->levelParamsPanel;
        reinterpret_cast<RectTransform*>(statsPanel->get_transform())->Translate(0.0f, 0.05f, 0.0f);

        auto NPS_cs = il2cpp_utils::newcsstr("NPS");
        auto NotesCount_cs = il2cpp_utils::newcsstr("NotesCount");
        auto ObstaclesCount_cs = il2cpp_utils::newcsstr("ObstaclesCount");
        auto BombsCount_cs = il2cpp_utils::newcsstr("BombsCount");

        ppStatButton = reinterpret_cast<UnityEngine::RectTransform*>(UIUtils::CreateStatIcon("PPStatLabel",
            reinterpret_cast<UnityEngine::Transform*>(ArrayUtil::First(statsPanel->get_gameObject()->GetComponentsInChildren<RectTransform*>(), [NPS_cs](auto x) {
                return x->get_name()->Equals(NPS_cs); 
            })),
            statsPanel->get_transform(),
            SpriteUtils::get_GraphIcon(),
            "PP Value"));

        starStatButton = reinterpret_cast<UnityEngine::RectTransform*>(UIUtils::CreateStatIcon("StarStatLabel",
            reinterpret_cast<UnityEngine::Transform*>(ArrayUtil::First(statsPanel->get_gameObject()->GetComponentsInChildren<RectTransform*>(), [NotesCount_cs](auto x) {
                return x->get_name()->Equals(NotesCount_cs); 
            })),
            statsPanel->get_transform(),
            SpriteUtils::get_StarFullIcon(),
            "Star Difficulty Rating"));

        njsStatButton = reinterpret_cast<UnityEngine::RectTransform*>(UIUtils::CreateStatIcon("NoteJumpSpeedLabel",
            reinterpret_cast<UnityEngine::Transform*>(ArrayUtil::First(statsPanel->get_gameObject()->GetComponentsInChildren<RectTransform*>(), [ObstaclesCount_cs](auto x) {
                return x->get_name()->Equals(ObstaclesCount_cs); 
            })),
            statsPanel->get_transform(),
            SpriteUtils::get_SpeedIcon(),
            "Note Jump Speed"));

        noteJumpStartBeatOffsetLabel = reinterpret_cast<UnityEngine::RectTransform*>(UIUtils::CreateStatIcon("NoteJumpStartBeatOffsetLabel",
            reinterpret_cast<UnityEngine::Transform*>(ArrayUtil::First(statsPanel->get_gameObject()->GetComponentsInChildren<RectTransform*>(), [BombsCount_cs](auto x) {
                return x->get_name()->Equals(BombsCount_cs); 
            })),
            statsPanel->get_transform(),
            SpriteUtils::get_NoteStartOffsetIcon(),
                "Note Jump Start Beat Offset"));
    }

    void SongBrowserUI::ResizeSongUI()
    {
        // shrink play button container
        beatUi->ActionButtons->set_localScale(Vector3(0.875f, 0.875f, 0.875f));
    }

    void SongBrowserUI::InstallHandlers()
    {
        #warning not implemented
    }

    void SongBrowserUI::OnDidFavoriteToggleChangeEvent(GlobalNamespace::StandardLevelDetailView* arg1, UnityEngine::UI::Toggle* arg2)
    {
        #warning not implemented
        if (config.currentLevelCategoryName == "Favorites")
        {
            // TODO - still scrolls to top in this view
        }
        else
        {
            StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(AsyncForceScrollToPosition(model->lastScrollIndex))));
        }
    }
    custom_types::Helpers::Coroutine SongBrowserUI::AsyncForceScrollToPosition(float position)
    {
        INFO("Will attempt force scrolling to position [%.2f] at end of frame.", position);

        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForEndOfFrame::New_ctor());

        auto tv = beatUi->LevelCollectionTableView->tableView;
        auto sv = tv->scrollView;
        INFO("Force scrolling to %.2f", position);
        sv->ScrollTo(position, false);
        co_return;
    }

    custom_types::Helpers::Coroutine SongBrowserUI::AsyncWaitForSongUIUpdate()
    {
        if (asyncUpdating || !uiCreated || !model->get_sortWasMissingData())
            co_return;

        asyncUpdating = true;

        while (beatUi && (
               beatUi->LevelSelectionNavigationController->get_isInTransition() ||
               beatUi->LevelDetailViewController->get_isInTransition() ||
               !beatUi->LevelSelectionNavigationController->get_isInViewControllerHierarchy() ||
               !beatUi->LevelDetailViewController->get_isInViewControllerHierarchy() ||
               !beatUi->LevelSelectionNavigationController->get_isActiveAndEnabled() ||
               !beatUi->LevelDetailViewController->get_isActiveAndEnabled()))
        {
            co_yield nullptr;
        }
        #warning no check for if data available
        if (NeedsScoreSaberData(config.sortMode))// && SongDataCore.Plugin.Songs.IsDataAvailable())
        {
            ProcessSongList();
            RefreshSongUI();
        }

        asyncUpdating = false;
        co_return;
    }

    void SongBrowserUI::RefreshSongUI(bool scrollToLevel)
    {
        if (!uiCreated)
            return;

        RefreshSongList();
        RefreshSortButtonUI();
        
        if (!scrollToLevel)
            beatUi->ScrollToLevelByRow(0);

        RefreshQuickScrollButtons();
        RefreshCurrentSelectionDisplay();
    }

    void SongBrowserUI::ProcessSongList()
    {
        if (!uiCreated)
            return;
        model->ProcessSongList(lastLevelCollection, beatUi->LevelSelectionNavigationController);
    }

    void SongBrowserUI::CancelFilter()
    {
        #warning not implemented
    }

    void SongBrowserUI::handleDidSelectAnnotatedBeatmapLevelCollection(GlobalNamespace::IAnnotatedBeatmapLevelCollection* annotatedBeatmapLevelCollection)
    {
        #warning not implemented
    }

    void SongBrowserUI::_levelFilteringNavController_didSelectAnnotatedBeatmapLevelCollectionEvent(GlobalNamespace::LevelFilteringNavigationController* arg1, GlobalNamespace::IAnnotatedBeatmapLevelCollection* arg2,
            GameObject* arg3, GlobalNamespace::BeatmapCharacteristicSO* arg4)
    {
        #warning not implemented
    }

    void SongBrowserUI::SelectLevelCollection(GlobalNamespace::IAnnotatedBeatmapLevelCollection* levelCollection)
    {
        #warning not implemented
    }

    custom_types::Helpers::Coroutine SongBrowserUI::ProcessSongListEndOfFrame()
    {
        #warning not implemented
        co_return;
    }

    custom_types::Helpers::Coroutine SongBrowserUI::RefreshSongListEndOfFrame()
    {
        #warning not implemented
        co_return;
    }

    void SongBrowserUI::OnClearButtonClickEvent()
    {
        #warning not implemented
    }

    void SongBrowserUI::OnSortButtonClickEvent(SongSortMode sortMode)
    {
        #warning not implemented
    }

    void SongBrowserUI::OnFilterButtonClickEvent(SongFilterMode mode)
    {
        #warning not implemented
    }

    void SongBrowserUI::OnSearchButtonClickEvent()
    {
        #warning not implemented
    }

    void SongBrowserUI::OnDidSelectLevelEvent(GlobalNamespace::LevelCollectionViewController* view, GlobalNamespace::IPreviewBeatmapLevel* level)
    {
        #warning not implemented
    }

    void SongBrowserUI::OnDidSelectBeatmapCharacteristic(GlobalNamespace::BeatmapCharacteristicSegmentedControlController* view, GlobalNamespace::BeatmapCharacteristicSO* bc)
    {
        #warning not implemented
    }

    void SongBrowserUI::OnDidChangeDifficultyEvent(GlobalNamespace::StandardLevelDetailViewController* view, GlobalNamespace::IDifficultyBeatmap* beatmap)
    {
        #warning not implemented
    }

    void SongBrowserUI::OnDidPresentContentEvent(GlobalNamespace::StandardLevelDetailViewController* view, GlobalNamespace::StandardLevelDetailViewController::ContentType type)
    {
        #warning not implemented
    }

    void SongBrowserUI::HandleDidSelectLevelRow(GlobalNamespace::IPreviewBeatmapLevel* level)
    {
        #warning not implemented
    }

    void SongBrowserUI::HandleDeleteSelectedLevel()
    {
        #warning not implemented
    }

    void SongBrowserUI::ShowSearchKeyboard()
    {
        #warning not implemented
    }

    void SongBrowserUI::ShowInputKeyboard(std::function<void(Il2CppString*)> enterPressedHandler)
    {
        #warning not implemented
    }

    void SongBrowserUI::SearchViewControllerSearchButtonPressed(Il2CppString* searchFor)
    {
        #warning not implemented
    }

    void SongBrowserUI::CreatePlaylistButtonPressed(Il2CppString* playlistName)
    {
        #warning not implemented
    }

    void SongBrowserUI::JumpSongList(int numJumps, float segmentPercent)
    {
        #warning not implemented
    }

    void SongBrowserUI::RefreshScoreSaberData(GlobalNamespace::IPreviewBeatmapLevel* level)
    {
        #warning not implemented
    }

    void SongBrowserUI::RefreshNoteJumpSpeed(float noteJumpMovementSpeed, float noteJumpStartBeatOffset)
    {
        #warning not implemented
    }

    void SongBrowserUI::RefreshQuickScrollButtons()
    {
        #warning not implemented
    }

    custom_types::Helpers::Coroutine SongBrowserUI::RefreshQuickScrollButtonsAsync()
    {
        #warning not implemented
        co_return;
    }

    void SongBrowserUI::UpdateDeleteButtonState(Il2CppString* levelId)
    {
        #warning not implemented
    }

    void SongBrowserUI::SetVisibility(bool visible)
    {
        #warning not implemented
    }

    void SongBrowserUI::RefreshOuterUIState(UIState state)
    {
        #warning not implemented
    }

    void SongBrowserUI::RefreshCurrentSelectionDisplay()
    {
        #warning not implemented
    }

    void SongBrowserUI::RefreshSortButtonUI()
    {
        #warning not implemented
    }
}