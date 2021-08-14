#include "SongBrowserApplication.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/WaitForEndOfFrame.hpp"
#include "logging.hpp"

#include "songloader/shared/API.hpp"
#include "Utils/SpriteUtils.hpp"
#include "Utils/SongDataCoreUtils.hpp"

DEFINE_TYPE(SongBrowser, SongBrowserApplication);


namespace SongBrowser
{
    SongBrowser::UI::ProgressBar* SongBrowserApplication::mainProgressBar = nullptr;
    SongBrowser::SongBrowserApplication* SongBrowserApplication::instance = nullptr;

    void SongBrowserApplication::OnLoad()
    {
        UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("Beat Saber SongBrowser Plugin"))->AddComponent<SongBrowserApplication*>();
        mainProgressBar = SongBrowser::UI::ProgressBar::Create();
        SpriteUtils::Init();
    }

    void SongBrowserApplication::Awake()
    {
        instance = this;

        songBrowserModel = il2cpp_utils::New<SongBrowserModel*>().value();

        songBrowserModel->Init();

        songBrowserUI = get_gameObject()->AddComponent<UI::SongBrowserUI*>();
        songBrowserUI->model = songBrowserModel;
    }

    void SongBrowserApplication::Start()
    {
        //scoresaber, but we do not have scoresaber API
        //SongDataCore.Plugin.Songs.OnDataFinishedProcessing += OnScoreSaberDataDownloaded;

        if (RuntimeSongLoader::API::HasLoadedSongs()) SongBrowserApplication::OnSongLoaderLoadedSongs(RuntimeSongLoader::API::GetLoadedSongs());
        else RuntimeSongLoader::API::AddSongsLoadedEvent(std::bind(&SongBrowserApplication::OnSongLoaderLoadedSongs, this, std::placeholders::_1));
    }

    void SongBrowserApplication::OnSongLoaderLoadedSongs(const std::vector<GlobalNamespace::CustomPreviewBeatmapLevel*>& levels)
    {
        songBrowserUI->UpdateLevelDataModel();
        songBrowserUI->RefreshSongList();
        //mainProgressBar->ShowMessage("Songloader Finished", 2.0f);
    }

    void SongBrowserApplication::HandleSoloModeSelection()
    {
        HandleModeSelection(GlobalNamespace::MainMenuViewController::MenuButton::SoloFreePlay);
        songBrowserUI->Show();    
    }

    void SongBrowserApplication::HandlePartyModeSelection()
    {
        HandleModeSelection(GlobalNamespace::MainMenuViewController::MenuButton::Party);
        songBrowserUI->Show();    
    }

    void SongBrowserApplication::HandleCampaignModeSelection()
    {
        HandleModeSelection(GlobalNamespace::MainMenuViewController::MenuButton::SoloCampaign);
        songBrowserUI->Hide();    
    }

    void SongBrowserApplication::HandleMultiplayerModeSelection()
    {
        HandleModeSelection(GlobalNamespace::MainMenuViewController::MenuButton::Multiplayer);
        songBrowserUI->Hide();    
    }

    void SongBrowserApplication::HandleModeSelection(GlobalNamespace::MainMenuViewController::MenuButton mode)
    {
        INFO("Creating UI");
        songBrowserUI->CreateUI(mode);

        if (!hasShownProgressBar)
        {
            INFO("Showing progress bar");
            mainProgressBar->ShowMessage("", 5.0f);
            hasShownProgressBar = true;
        }
    }

    custom_types::Helpers::Coroutine SongBrowserApplication::UpdateBrowserUI()
    {
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForEndOfFrame::New_ctor());
        
        songBrowserUI->UpdateLevelDataModel();
        songBrowserUI->UpdateLevelCollectionSelection();
        songBrowserUI->RefreshSongList();
        co_return;
    }
}