#include "UI/ProgressBar.hpp"
#include "UnityEngine/GameObject.hpp"

DEFINE_TYPE(SongBrowser::UI, ProgressBar);

namespace SongBrowser::UI
{
    ProgressBar* ProgressBar::Create()
    {
        return UnityEngine::GameObject::New_ctor()->AddComponent<ProgressBar*>();
    }

    void ProgressBar::ShowMessage(const std::string_view& message)
    {
        #warning not implemented
    }

    void ProgressBar::ShowMessage(const std::string_view& message, float time)
    {
        #warning not implemented
    }

    void ProgressBar::OnEnable()
    {
        #warning not implemented
    }

    void ProgressBar::OnDisable()
    {
        #warning not implemented
    }

    void ProgressBar::Awake()
    {
        #warning not implemented
    }

    void ProgressBar::Update()
    {
        #warning not implemented
    }

    void ProgressBar::SongLoaderOnLoadingStartedEvent()
    {
        #warning not implemented
    }

    void ProgressBar::SongBrowserFinishedProcessingSongs(const std::vector<GlobalNamespace::CustomPreviewBeatmapLevel*>& levels)
    {
        #warning not implemented
    }

    void ProgressBar::SceneManagerOnActiveSceneChanged(UnityEngine::SceneManagement::Scene oldScene, UnityEngine::SceneManagement::Scene newScene)
    {
        #warning not implemented
    }

    custom_types::Helpers::Coroutine ProgressBar::DisableCanvasRoutine(float time)
    {
        #warning not implemented
        co_yield nullptr;
        co_return;
    }


}