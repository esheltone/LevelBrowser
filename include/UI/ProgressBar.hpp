#pragma once

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"

#include "GlobalNamespace/CustomPreviewBeatmapLevel.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"

#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/UI/Image.hpp"

#include "TMPro/TMP_Text.hpp"

#include "sombrero/shared/Vector3Utils.hpp"
#include "sombrero/shared/Vector2Utils.hpp"
#include "sombrero/shared/ColorUtils.hpp"
#include <string_view>
#include <vector>

DECLARE_CLASS_CODEGEN(SongBrowser::UI, ProgressBar, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_METHOD(void, OnEnable);
    DECLARE_INSTANCE_METHOD(void, OnDisable);
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, Update);

        DECLARE_INSTANCE_FIELD(UnityEngine::Canvas*, canvas);
        DECLARE_INSTANCE_FIELD(TMPro::TMP_Text*, authorNameText);
        DECLARE_INSTANCE_FIELD(TMPro::TMP_Text*, pluginNameText);
        DECLARE_INSTANCE_FIELD(TMPro::TMP_Text*, headerText);
        DECLARE_INSTANCE_FIELD(UnityEngine::UI::Image*, loadingBackground);
        DECLARE_INSTANCE_FIELD(UnityEngine::UI::Image*, loadingBar);
    private:
        static inline Sombrero::FastVector3 Position = Sombrero::FastVector3(0, 0, 3.0f);
        static inline Sombrero::FastVector3 Rotation = Sombrero::FastVector3::zero();
        static inline Sombrero::FastVector3 Scale = Sombrero::FastVector3(0.01f, 0.01f, 0.01f);

        static inline Sombrero::FastVector2 CanvasSize = Sombrero::FastVector2(200, 50);

        static constexpr const char* AuthorNameText = "RedBrumbler";
        static constexpr const float AuthorNameFontSize = 7.0f;
        static inline Sombrero::FastVector2 AuthorNamePosition = Sombrero::FastVector2(10, 31);

        static constexpr const char* PluginNameText = "Song Browser - v<size=100%>" VERSION "</size>";
        static constexpr const float PluginNameFontSize = 9.0f;
        static inline Sombrero::FastVector2 PluginNamePosition = Sombrero::FastVector2(10, 23);

        static inline Sombrero::FastVector2 HeaderPosition = Sombrero::FastVector2(10, 15);
        static inline Sombrero::FastVector2 HeaderSize = Sombrero::FastVector2(100, 20);
        static constexpr const char* HeaderText = "Processing songs...";
        static constexpr const float HeaderFontSize = 15.0f;

        static inline Sombrero::FastVector2 LoadingBarSize = Sombrero::FastVector2(100, 10);
        static inline Sombrero::FastColor BackgroundColor = Sombrero::FastColor(0, 0, 0, 0.2f);

        bool showingMessage = false;
        bool inited = false;
    private:
        void SongLoaderOnLoadingStartedEvent();
        void SongBrowserFinishedProcessingSongs(const std::vector<GlobalNamespace::CustomPreviewBeatmapLevel*>& levels);
        void SceneManagerOnActiveSceneChanged(UnityEngine::SceneManagement::Scene oldScene, UnityEngine::SceneManagement::Scene newScene);
        custom_types::Helpers::Coroutine DisableCanvasRoutine(float time);
    public:
        static ProgressBar* Create();
        void ShowMessage(const std::string_view& message, float time);
        void ShowMessage(const std::string_view& message);
)