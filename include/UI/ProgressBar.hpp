#pragma once

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "GlobalNamespace/CustomPreviewBeatmapLevel.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include <string_view>
#include <vector>

DECLARE_CLASS_CODEGEN(SongBrowser::UI, ProgressBar, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_METHOD(void, OnEnable);
    DECLARE_INSTANCE_METHOD(void, OnDisable);
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, Update);

    void SongLoaderOnLoadingStartedEvent();
    void SongBrowserFinishedProcessingSongs(const std::vector<GlobalNamespace::CustomPreviewBeatmapLevel*>& levels);
    void SceneManagerOnActiveSceneChanged(UnityEngine::SceneManagement::Scene oldScene, UnityEngine::SceneManagement::Scene newScene);
    static ProgressBar* Create();
    void ShowMessage(const std::string_view& message, float time);
    void ShowMessage(const std::string_view& message);
    custom_types::Helpers::Coroutine DisableCanvasRoutine(float time);
)