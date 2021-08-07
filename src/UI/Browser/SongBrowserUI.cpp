#include "UI/Browser/SongBrowserUI.hpp"
#include "SongBrowserApplication.hpp"

#include "UnityEngine/Object.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/WaitForEndOfFrame.hpp"

#include "HMUI/CurvedCanvasSettings.hpp"
#include "HMUI/ScrollView.hpp"

#include "GlobalNamespace/IBeatmapLevelPackCollection.hpp"
#include "GlobalNamespace/IBeatmapLevel.hpp"
#include "GlobalNamespace/PreviewBeatmapLevelPackSO.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
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
#include "System/Enum.hpp"

#include "sombrero/shared/RandomUtils.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

DEFINE_TYPE(SongBrowser::UI, SongBrowserUI);
DEFINE_TYPE(SongBrowser::UI, SongBrowserViewController);

namespace SongBrowser::UI
{
    void SongBrowserUI::Show()
    {
        SetVisibility(true);
    }

    void SongBrowserUI::Hide()
    {
        SetVisibility(false);
    }

    void SongBrowserUI::UpdateLevelDataModel()
    {
        // get a current beatmap characteristic...
        if (!model->currentBeatmapCharacteristicSO && uiCreated)
        {
            model->currentBeatmapCharacteristicSO = beatUi->BeatmapCharacteristicSelectionViewController->selectedBeatmapCharacteristic;
        }

        model->UpdateLevelRecords();
    }

    bool SongBrowserUI::UpdateLevelCollectionSelection()
    {
        if (uiCreated)
        {
            auto currentSelected = beatUi->GetCurrentSelectedAnnotatedBeatmapLevelCollection();

            // select category
            if (config.currentLevelCategoryName != "")
            {
                selectingCategory = true;
                beatUi->SelectLevelCategory(config.currentLevelCategoryName);
                selectingCategory = false;
            }


            // select collection
            if (config.currentLevelCollectionName == "")
            {
                if (!currentSelected && config.currentLevelCategoryName == "")
                {
                    INFO("No level collection selected, acquiring the first available, likely OST1...");
                    currentSelected = reinterpret_cast<GlobalNamespace::IAnnotatedBeatmapLevelCollection*>(beatUi->BeatmapLevelsModel->get_allLoadedBeatmapLevelPackCollection()->get_beatmapLevelPacks()->values[0]);
                }
            }
            else if (!currentSelected || (to_utf8(csstrtostr(currentSelected->get_collectionName())) != config.currentLevelCollectionName))
            {
                #warning event stuff
                //beatUi->LevelFilteringNavigationController->didSelectAnnotatedBeatmapLevelCollectionEvent -= _levelFilteringNavController_didSelectAnnotatedBeatmapLevelCollectionEvent;

                lastLevelCollection = beatUi->GetLevelCollectionByName(config.currentLevelCollectionName);
                if (il2cpp_utils::try_cast<GlobalNamespace::PreviewBeatmapLevelPackSO>(lastLevelCollection))
                    Hide();
                
                beatUi->SelectLevelCollection(config.currentLevelCollectionName);
                //beatUi->LevelFilteringNavigationController->get_didSelectAnnotatedBeatmapLevelCollectionEvent() += _levelFilteringNavController_didSelectAnnotatedBeatmapLevelCollectionEvent;
            }

            if (!lastLevelCollection)
            {
                if (currentSelected)
                {
                    std::string collectionName = to_utf8(csstrtostr(currentSelected->get_collectionName()));
                    if (strcmp(collectionName.c_str(), SongBrowserModel::filteredSongsCollectionName) && strcmp(collectionName.c_str(), SongBrowserModel::playlistSongsCollectionName))
                    {
                        lastLevelCollection = currentSelected;
                    }
                }
            }

            ProcessSongList();
        }

        return false;
    }

    void SongBrowserUI::RefreshSongList()
    {
        if (!uiCreated)
            return;
        beatUi->RefreshSongList(model->lastSelectedLevelId);
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
        INFO("Cancelling filter, levelCollection {%s}", to_utf8(csstrtostr(lastLevelCollection->get_collectionName())).c_str());
        config.filterMode = SongFilterMode::None;

        auto noDataGO = beatUi->LevelCollectionViewController->noDataInfoGO;
        auto headerText = beatUi->LevelCollectionTableView->headerText;
        auto headerSprite = beatUi->LevelCollectionTableView->headerSprite;

        auto levelCollection = beatUi->GetCurrentSelectedAnnotatedBeatmapLevelCollection()->get_beatmapLevelCollection();
        beatUi->LevelCollectionViewController->SetData(levelCollection, headerText, headerSprite, false, noDataGO);
    }

    inline std::string LevelCategoryToString(int cat)
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

    void SongBrowserUI::handleDidSelectAnnotatedBeatmapLevelCollection(GlobalNamespace::IAnnotatedBeatmapLevelCollection* annotatedBeatmapLevelCollection)
    {
        lastLevelCollection = annotatedBeatmapLevelCollection;
        config.currentLevelCategoryName = LevelCategoryToString(beatUi->LevelFilteringNavigationController->get_selectedLevelCategory().value);
        SaveConfig();
        INFO("AnnotatedBeatmapLevelCollection, Selected Level Collection={%s}", to_utf8(csstrtostr(lastLevelCollection->get_collectionName())).c_str());
    }

    void SongBrowserUI::_levelFilteringNavController_didSelectAnnotatedBeatmapLevelCollectionEvent(GlobalNamespace::LevelFilteringNavigationController* arg1, GlobalNamespace::IAnnotatedBeatmapLevelCollection* arg2,
            GameObject* arg3, GlobalNamespace::BeatmapCharacteristicSO* arg4)
    {
        if (!arg2)
        {
            // Probably means we transitioned between Music Packs and Playlists
            arg2 = beatUi->GetCurrentSelectedAnnotatedBeatmapLevelCollection();
            if (!arg2)
            {
                ERROR("Nothing selected. This is likely an error.");
                return;
            }
        }

        // Do something about preview level packs, they can't be used past this point
        if (il2cpp_utils::try_cast<GlobalNamespace::PreviewBeatmapLevelPackSO>(arg2))
        {
            INFO("Hiding SongBrowser, previewing a song pack.");
            Hide();
            return;
        }

        Show();

        // category transition, just record the new collection
        if (selectingCategory)
        {
            INFO("Transitioning level category");
            lastLevelCollection = arg2;
            StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(RefreshSongListEndOfFrame())));
            return;
        }

        // Skip the first time - prevents a bunch of reload content spam
        if (!lastLevelCollection)
            return;

        SelectLevelCollection(arg2);
    }

    void SongBrowserUI::SelectLevelCollection(GlobalNamespace::IAnnotatedBeatmapLevelCollection* levelCollection)
    {
        if (!levelCollection)
        {
            INFO("No level collection selected...");
            return;
        }

        std::string collectionName = to_utf8(csstrtostr(levelCollection->get_collectionName()));
        // store the real level collection
        if (strcmp(collectionName.c_str(), SongBrowserModel::filteredSongsCollectionName) && lastLevelCollection)
        {
            INFO("Recording levelCollection: {%s}", collectionName.c_str());
            lastLevelCollection = levelCollection;
            config.currentLevelCategoryName = LevelCategoryToString(beatUi->LevelFilteringNavigationController->get_selectedLevelCategory().value);
        }

        // reset level selection
        model->lastSelectedLevelId = "";

        // save level collection
        config.currentLevelCollectionName = collectionName;
        SaveConfig();

        StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(ProcessSongListEndOfFrame())));
    }

    custom_types::Helpers::Coroutine SongBrowserUI::ProcessSongListEndOfFrame()
    {
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForEndOfFrame::New_ctor());

        bool scrollToLevel = true;
        if (lastLevelCollection && il2cpp_utils::try_cast<GlobalNamespace::IPlaylist>(lastLevelCollection))
        {
            scrollToLevel = false;
            config.sortMode = SongSortMode::Original;
            RefreshSortButtonUI();
        }

        ProcessSongList();
        RefreshSongUI(scrollToLevel);
        co_return;
    }

    custom_types::Helpers::Coroutine SongBrowserUI::RefreshSongListEndOfFrame()
    {
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForEndOfFrame::New_ctor());
        RefreshSongUI();
        co_return;
    }

    void SongBrowserUI::OnClearButtonClickEvent()
    {
        INFO("Clearing all sorts and filters.");

        config.sortMode = SongSortMode::Original;
        config.invertSortResults = false;
        config.filterMode = SongFilterMode::None;
        SaveConfig();

        CancelFilter();
        ProcessSongList();
        RefreshSongUI();
    }

    void SongBrowserUI::OnSortButtonClickEvent(SongSortMode sortMode)
    {
        INFO("Sort button - {%d} - pressed.", sortMode);

        #warning no songdatacore so no check for data available
        if (NeedsScoreSaberData(sortMode))// && !SongDataCore.Plugin.Songs.IsDataAvailable()))
        {
            INFO("Data for sort type is not available.");
            return;
        }

        // Clear current selected level id so our song list jumps to the start
        model->lastSelectedLevelId = "";

        if (config.sortMode == sortMode)
        {
            model->ToggleInverting();
        }

        config.sortMode = sortMode;

        // update the seed
        if (config.sortMode == SongSortMode::Random)
        {
            srand(time(0));
            config.randomSongSeed = rand();
        }

        SaveConfig();

        ProcessSongList();
        RefreshSongUI();
    }

    void SongBrowserUI::OnFilterButtonClickEvent(SongFilterMode mode)
    {
        INFO("FilterButton {%d} clicked.", mode);

        auto curCollection = beatUi->GetCurrentSelectedAnnotatedBeatmapLevelCollection();
        std::string collectionName = to_utf8(csstrtostr(curCollection->get_collectionName()));
        if (!lastLevelCollection ||
            (curCollection &&
            // strcmp returns 0 for same, so we need both to not be same
            strcmp(collectionName.c_str(), SongBrowserModel::filteredSongsCollectionName) &&
            strcmp(collectionName.c_str(), SongBrowserModel::playlistSongsCollectionName)))
        {
            lastLevelCollection = curCollection;
        }

        if (mode == SongFilterMode::Favorites)
        {
            beatUi->SelectLevelCategory("Favorites");
        }
        else
        {
            auto noDataGO = beatUi->LevelCollectionViewController->noDataInfoGO;
            auto headerText = beatUi->LevelCollectionTableView->headerText;
            auto headerSprite = beatUi->LevelCollectionTableView->headerSprite;

            auto levelCollection = beatUi->GetCurrentSelectedAnnotatedBeatmapLevelCollection()->get_beatmapLevelCollection();
            beatUi->LevelCollectionViewController->SetData(levelCollection, headerText, headerSprite, false, noDataGO);
        }

        // If selecting the same filter, cancel
        if (config.filterMode == mode)
        {
            config.filterMode = SongFilterMode::None;
        }
        else
        {
            config.filterMode = mode;
        }

        switch (mode)
        {
            case SongFilterMode::Search:
                OnSearchButtonClickEvent();
                break;
            default:
                SaveConfig();
                ProcessSongList();
                RefreshSongUI();
                break;
        }
    }

    void SongBrowserUI::OnSearchButtonClickEvent()
    {
        ShowSearchKeyboard();
    }

    void SongBrowserUI::OnDidSelectLevelEvent(GlobalNamespace::LevelCollectionViewController* view, GlobalNamespace::IPreviewBeatmapLevel* level)
    {
        if (!level)
        {
            INFO("No level selected?");
            return;
        }

        model->lastSelectedLevelId = to_utf8(csstrtostr(level->get_levelID()));
        HandleDidSelectLevelRow(level);
    }

    void SongBrowserUI::OnDidSelectBeatmapCharacteristic(GlobalNamespace::BeatmapCharacteristicSegmentedControlController* view, GlobalNamespace::BeatmapCharacteristicSO* bc)
    {
        model->currentBeatmapCharacteristicSO = bc;

        if (beatUi->StandardLevelDetailView)
        {
            RefreshScoreSaberData(reinterpret_cast<GlobalNamespace::IPreviewBeatmapLevel*>(beatUi->StandardLevelDetailView->get_selectedDifficultyBeatmap()->get_level()));
            RefreshNoteJumpSpeed(beatUi->StandardLevelDetailView->get_selectedDifficultyBeatmap()->get_noteJumpMovementSpeed(),
                beatUi->StandardLevelDetailView->get_selectedDifficultyBeatmap()->get_noteJumpStartBeatOffset());
        }
    }

    void SongBrowserUI::OnDidChangeDifficultyEvent(GlobalNamespace::StandardLevelDetailViewController* view, GlobalNamespace::IDifficultyBeatmap* beatmap)
    {
        if (!view->get_selectedDifficultyBeatmap())
            return;

        UpdateDeleteButtonState(reinterpret_cast<GlobalNamespace::IPreviewBeatmapLevel*>(view->get_selectedDifficultyBeatmap()->get_level())->get_levelID());
        RefreshScoreSaberData(reinterpret_cast<GlobalNamespace::IPreviewBeatmapLevel*>(view->get_selectedDifficultyBeatmap()->get_level()));
        RefreshNoteJumpSpeed(beatmap->get_noteJumpMovementSpeed(), beatmap->get_noteJumpStartBeatOffset());
    }

    void SongBrowserUI::OnDidPresentContentEvent(GlobalNamespace::StandardLevelDetailViewController* view, GlobalNamespace::StandardLevelDetailViewController::ContentType type)
    {
        if (type != GlobalNamespace::StandardLevelDetailViewController::ContentType::OwnedAndReady)
            return;

        if (!view->get_selectedDifficultyBeatmap())
            return;

        // stash the scroll index
        auto tv = beatUi->LevelCollectionTableView->tableView;
        auto sv = tv->scrollView;
        model->lastScrollIndex = sv->get_position();

        UpdateDeleteButtonState(reinterpret_cast<GlobalNamespace::IPreviewBeatmapLevel*>(view->get_selectedDifficultyBeatmap()->get_level())->get_levelID());
        RefreshScoreSaberData(reinterpret_cast<GlobalNamespace::IPreviewBeatmapLevel*>(view->get_selectedDifficultyBeatmap()->get_level()));
        RefreshNoteJumpSpeed(view->get_selectedDifficultyBeatmap()->get_noteJumpMovementSpeed(), view->get_selectedDifficultyBeatmap()->get_noteJumpStartBeatOffset());
    }

    void SongBrowserUI::HandleDidSelectLevelRow(GlobalNamespace::IPreviewBeatmapLevel* level)
    {
        UpdateDeleteButtonState(level->get_levelID());
        RefreshQuickScrollButtons();
    }

    void SongBrowserUI::HandleDeleteSelectedLevel()
    {
        #warning not implemented
    }

    void SongBrowserUI::ShowSearchKeyboard()
    {
        ShowInputKeyboard(std::bind(&SongBrowserUI::SearchViewControllerSearchButtonPressed, this, std::placeholders::_1));
    }

    void SongBrowserUI::ShowInputKeyboard(std::function<void(Il2CppString*)> enterPressedHandler)
    {
        #warning not implemented
    }

    void SongBrowserUI::SearchViewControllerSearchButtonPressed(Il2CppString* searchFor)
    {
        config.filterMode = SongFilterMode::Search;
        config.searchTerms.insert(config.searchTerms.begin(), to_utf8(csstrtostr(searchFor)));
        SaveConfig();
        model->lastSelectedLevelId = "";

        ProcessSongList();
        RefreshSongUI();
    }

    void SongBrowserUI::CreatePlaylistButtonPressed(Il2CppString* playlistName)
    {
        if (Il2CppString::IsNullOrWhiteSpace(playlistName))
            return;
        #warning playlists not properly made to work
        //BeatSaberPlaylistsLib.Types.IPlaylist playlist = Playlist.CreateNew(playlistName, _beatUi.GetCurrentLevelCollectionLevels());
        //BeatSaberPlaylistsLib.PlaylistManager.DefaultManager.RequestRefresh(Assembly.GetExecutingAssembly().FullName);
        SongBrowserApplication::mainProgressBar()->ShowMessage("Successfully Exported Playlist");
    }

    void SongBrowserUI::JumpSongList(int numJumps, float segmentPercent)
    {
        auto levels = beatUi->GetCurrentLevelCollectionLevels();
        if (levels)
            return;

        int totalSize = levels->Length();
        int segmentSize = (int)(totalSize * segmentPercent);

        // Jump at least one scree size.
        if (segmentSize < LIST_ITEMS_VISIBLE_AT_ONCE)
        {
            segmentSize = LIST_ITEMS_VISIBLE_AT_ONCE;
        }

        int currentRow = beatUi->LevelCollectionTableView->selectedRow;
        int jumpDirection = 1 - ((numJumps < 0) * 2);
        int newRow = currentRow + (jumpDirection * segmentSize);
        if (newRow <= 0)
        {
            newRow = 0;
        }
        else if (newRow >= totalSize)
        {
            newRow = totalSize - 1;
        }

        INFO("jumpDirection: {%d}, newRow: {%d}", jumpDirection, newRow);
        beatUi->ScrollToLevelByRow(newRow);
        RefreshQuickScrollButtons();
    }

    void SongBrowserUI::RefreshScoreSaberData(GlobalNamespace::IPreviewBeatmapLevel* level)
    {
        #warning not implemented
    }

    void SongBrowserUI::RefreshNoteJumpSpeed(float noteJumpMovementSpeed, float noteJumpStartBeatOffset)
    {
        UIUtils::SetStatButtonText(njsStatButton, string_format("%.2f", noteJumpMovementSpeed));
        UIUtils::SetStatButtonText(noteJumpStartBeatOffsetLabel, string_format("%.2f", noteJumpStartBeatOffset));
    }

    void SongBrowserUI::RefreshQuickScrollButtons()
    {
        if (!uiCreated)
            return;
        
        pageUpFastButton->set_interactable(beatUi->TableViewPageUpButton->get_interactable());
        pageUpFastButton->get_gameObject()->SetActive(beatUi->TableViewPageUpButton->IsActive());

        pageDownFastButton->set_interactable(beatUi->TableViewPageDownButton->get_interactable());
        pageDownFastButton->get_gameObject()->SetActive(beatUi->TableViewPageDownButton->IsActive());
    }

    custom_types::Helpers::Coroutine SongBrowserUI::RefreshQuickScrollButtonsAsync()
    {
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForEndOfFrame::New_ctor());

        RefreshQuickScrollButtons();
        co_return;
    }

    void SongBrowserUI::UpdateDeleteButtonState(Il2CppString* levelId)
    {
        if (!deleteButton)
            return;
        deleteButton->get_gameObject()->SetActive(levelId->get_Length() >= 32);
    }

    void SongBrowserUI::SetVisibility(bool visible)
    {
        if (!uiCreated)
            return;
        if (ppStatButton) ppStatButton->get_gameObject()->SetActive(visible);
        if (starStatButton) starStatButton->get_gameObject()->SetActive(visible);
        if (njsStatButton) njsStatButton->get_gameObject()->SetActive(visible);

        RefreshOuterUIState(visible ? UIState::Main : UIState::Disabled);

        if (deleteButton) deleteButton->get_gameObject()->SetActive(visible);

        if (pageUpFastButton) pageUpFastButton->get_gameObject()->SetActive(visible);
        if (pageDownFastButton) pageDownFastButton->get_gameObject()->SetActive(visible);
    }

    void SongBrowserUI::RefreshOuterUIState(UIState state)
    {
        bool sortButtons = false;
        bool filterButtons = false;
        bool outerButtons = false;
        bool clearButton = true;
        switch (state)
        {
            case UIState::SortBy:
                sortButtons = true;
                break;
            case UIState::FilterBy:
                filterButtons = true;
                break;
            case UIState::Main:
                outerButtons = true;
                break;
            default:
                clearButton = false;
                break;
        }

        int sortLength = sortButtonGroup->get_Count();
        for (int i = 0; i < sortLength; i++)
            sortButtonGroup->items->values[i]->button->get_gameObject()->SetActive(sortButtons);

        int filterLength = filterButtonGroup->get_Count();
        for (int i = 0; i < filterLength; i++)
            filterButtonGroup->items->values[i]->button->get_gameObject()->SetActive(filterButtons);

        if (sortByButton) sortByButton->get_gameObject()->SetActive(outerButtons);
        if (sortByDisplay) sortByDisplay->get_gameObject()->SetActive(outerButtons);
        if (filterByButton) filterByButton->get_gameObject()->SetActive(outerButtons);
        if (filterByDisplay) filterByDisplay->get_gameObject()->SetActive(outerButtons);
        if (clearSortFilterButton) clearSortFilterButton->get_gameObject()->SetActive(clearButton);
        if (randomButton) randomButton->get_gameObject()->SetActive(outerButtons);
        if (playlistExportButton) playlistExportButton->get_gameObject()->SetActive(outerButtons);

        RefreshCurrentSelectionDisplay();
        currentUiState = state;
    }

    void SongBrowserUI::RefreshCurrentSelectionDisplay()
    {
        std::string sortByDisplayText;
        if (config.sortMode == SongSortMode::Default)
        {
            sortByDisplayText = "Title";
        }
        else
        {
            sortByDisplayText = SongSortModeToString(config.sortMode);
        }

        UIUtils::SetButtonText(sortByDisplay, sortByDisplayText);
        if (config.filterMode != SongFilterMode::CustomFilter)
        {
            // Custom SongFilterMod implies that another mod has modified the text of this button (do not overwrite)
            UIUtils::SetButtonText(filterByDisplay, SongFilterModeToString(config.filterMode));
        }
    }

    void SongBrowserUI::RefreshSortButtonUI()
    {
        if (!uiCreated)
            return;

        int length = sortButtonGroup->get_Count();
        for (int i = 0; i < length; i++)
        {
            auto sortButton = sortButtonGroup->items->values[i];
            #warning still no song data core
            if (NeedsScoreSaberData(sortButton->sortMode))// && !SongDataCore.Plugin.Songs.IsDataAvailable())
                UIUtils::SetButtonUnderlineColor(sortButton->button, {0.5f, 0.5f, 0.5f, 1.0f});
            else
                UIUtils::SetButtonUnderlineColor(sortButton->button, {1.0f, 1.0f, 1.0f, 1.0f});

            if (sortButton->sortMode == config.sortMode)
            {
                if (config.invertSortResults)
                    UIUtils::SetButtonUnderlineColor(sortButton->button, {1.0f, 0.0f, 0.0f, 1.0f});
                else
                    UIUtils::SetButtonUnderlineColor(sortButton->button, {0.0f, 1.0f, 0.0f, 1.0f});
            }
        }

        length = filterButtonGroup->get_Count();
        for (int i = 0; i < length; i++)
        {
            auto filterButton = filterButtonGroup->items->values[i];
            if (filterButton->filterMode == config.filterMode)
                UIUtils::SetButtonUnderlineColor(filterButton->button, {0.0f, 1.0f, 0.0f, 1.0f});
            else
                UIUtils::SetButtonUnderlineColor(filterButton->button, {1.0f, 1.0f, 1.0f, 1.0f});
        }

        if (config.invertSortResults)
            UIUtils::SetButtonUnderlineColor(sortByDisplay, {1.0f, 0.0f, 0.0f, 1.0f});
        else
            UIUtils::SetButtonUnderlineColor(sortByDisplay, {0.0f, 1.0f, 0.0f, 1.0f});

        if (config.filterMode != SongFilterMode::None)
            UIUtils::SetButtonUnderlineColor(filterByDisplay, {0.0f, 1.0f, 0.0f, 1.0f});
        else
            UIUtils::SetButtonUnderlineColor(filterByDisplay, {1.0f, 1.0f, 1.0f, 1.0f});
    }
}