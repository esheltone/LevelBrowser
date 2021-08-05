#include "UI/Browser/SongBrowserUI.hpp"

#include "UnityEngine/Object.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"

#include "HMUI/CurvedCanvasSettings.hpp"
#include "GlobalNamespace/SoloFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/PartyFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/MultiplayerLevelSelectionFlowCoordinator.hpp"

#include "Utils/ArrayUtil.hpp"

#include "questui/shared/BeatSaberUI.hpp"

using namespace UnityEngine;

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
                flowCoordinator = Resources::FindObjectsOfTypeAll<GlobalNamespace::SoloFreePlayFlowCoordinator*>()->Last();
                break;
            case GlobalNamespace::MainMenuViewController::MenuButton::Party:
                flowCoordinator = Resources::FindObjectsOfTypeAll<GlobalNamespace::PartyFreePlayFlowCoordinator*>()->Last();
                break;
            case GlobalNamespace::MainMenuViewController::MenuButton::Multiplayer:
                flowCoordinator = Resources::FindObjectsOfTypeAll<GlobalNamespace::MultiplayerLevelSelectionFlowCoordinator*>()->Last();
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

        viewController = QuestUI::BeatSaberUI::CreateViewController<SongBrowser::UI::SongBrowserViewController*>();
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

    void SongBrowserUI::CreateOuterUI()
    {
        #warning not implemented
    }

    void SongBrowserUI::CreateSortButtons()
    {
        #warning not implemented
    }

    void SongBrowserUI::CreateFilterButtons()
    {
        #warning not implemented
    }

    void SongBrowserUI::CreateFastPageButtons()
    {
        #warning not implemented
    }

    void SongBrowserUI::CreateDeleteUI()
    {
        #warning not implemented
    }

    void SongBrowserUI::ModifySongStatsPanel()
    {
        #warning not implemented
    }

    void SongBrowserUI::ResizeSongUI()
    {
        #warning not implemented
    }

    void SongBrowserUI::InstallHandlers()
    {
        #warning not implemented
    }

    void SongBrowserUI::OnDidFavoriteToggleChangeEvent(GlobalNamespace::StandardLevelDetailView* arg1, UI::Toggle* arg2)
    {
        #warning not implemented
    }
    custom_types::Helpers::Coroutine SongBrowserUI::AsyncForceScrollToPosition(float position)
    {
        #warning not implemented
        co_return;
    }

    custom_types::Helpers::Coroutine SongBrowserUI::AsyncWaitForSongUIUpdate()
    {
        #warning not implemented
        co_return;
    }

    void SongBrowserUI::RefreshSongUI(bool scrollToLevel)
    {
        #warning not implemented
    }

    void SongBrowserUI::ProcessSongList()
    {
        #warning not implemented
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

    void SongBrowserUI::ShowInputKeyboard(System::Action_1<Il2CppString*>* enterPressedHandler)
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