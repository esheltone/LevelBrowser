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
        INFO("Application load complete!");
    }

    void SongBrowserApplication::Awake()
    {
        INFO("Awake");
        instance = this;
        INFO("instance: %p", instance);

        songBrowserModel = il2cpp_utils::New<SongBrowserModel*>().value();
        INFO("songBrowserModel: %p", songBrowserModel);

        songBrowserModel->Init();
        INFO("get_gameObject: %p", get_gameObject());

        songBrowserUI = get_gameObject()->AddComponent<UI::SongBrowserUI*>();
        INFO("songBrowserUI: %p", songBrowserUI);
        songBrowserUI->model = songBrowserModel;
        INFO("songBrowserUI->model: %p", songBrowserUI->model);
    }

    void SongBrowserApplication::Start()
    {
        //scoresaber, but we do not have scoresaber API
        //SongDataCore.Plugin.Songs.OnDataFinishedProcessing += OnScoreSaberDataDownloaded;

        // I can't just check if songs are loaded, so lets hope we are adding this callback before songs are loaded
        if (RuntimeSongLoader::API::HasLoadedSongs()) SongBrowserApplication::OnSongLoaderLoadedSongs(RuntimeSongLoader::API::GetLoadedSongs());
        else RuntimeSongLoader::API::AddSongsLoadedEvent(std::bind(&SongBrowserApplication::OnSongLoaderLoadedSongs, this, std::placeholders::_1));
    }

    void SongBrowserApplication::OnSongLoaderLoadedSongs(const std::vector<GlobalNamespace::CustomPreviewBeatmapLevel*>& levels)
    {
        songBrowserUI->UpdateLevelDataModel();
        songBrowserUI->RefreshSongList();
    }

    void SongBrowserApplication::HandleSoloModeSelection()
    {
        INFO("Solo Mode");
        HandleModeSelection(GlobalNamespace::MainMenuViewController::MenuButton::SoloFreePlay);
        INFO("Show %p", songBrowserUI);
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
            mainProgressBar->ShowMessage("");
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