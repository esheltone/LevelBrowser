#include "UI/ProgressBar.hpp"
#include "UnityEngine/GameObject.hpp"
#include "Utils/EventUtils.hpp"
#include "Utils/UIUtils.hpp"

#include "DataAccess/SongBrowserModel.hpp"

#include "UnityEngine/WaitForSecondsRealtime.hpp"
#include "UnityEngine/RenderMode.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Rect.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "HMUI/CurvedCanvasSettings.hpp"

DEFINE_TYPE(SongBrowser::UI, ProgressBar);

namespace SongBrowser::UI
{
    ProgressBar* ProgressBar::Create()
    {
        return UnityEngine::GameObject::New_ctor(il2cpp_utils::newcsstr("SongBrowserLoadingStatus"))->AddComponent<ProgressBar*>();
    }

    void ProgressBar::ShowMessage(const std::string_view& message, float time)
    {
        StopAllCoroutines();
        if (!inited) return;
        showingMessage = true;
        headerText->set_text(il2cpp_utils::newcsstr(message));
        loadingBar->set_enabled(false);
        loadingBackground->set_enabled(false);
        get_gameObject()->SetActive(true);
        StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(DisableCanvasRoutine(time))));
    }

    void ProgressBar::ShowMessage(const std::string_view& message)
    {
        StopAllCoroutines();
        if (!inited) return;
        showingMessage = true;
        headerText->set_text(il2cpp_utils::newcsstr(message));
        loadingBar->set_enabled(false);
        loadingBackground->set_enabled(false);
        get_gameObject()->SetActive(true);
    }

    void ProgressBar::OnEnable()
    {
        EventUtils::OnActiveSceneChanged() += {&ProgressBar::SceneManagerOnActiveSceneChanged, this};
        SongBrowser::SongBrowserModel::didFinishProcessingSongs += {&ProgressBar::SongBrowserFinishedProcessingSongs, this};
    }

    void ProgressBar::OnDisable()
    {
        EventUtils::OnActiveSceneChanged() -= {&ProgressBar::SceneManagerOnActiveSceneChanged, this};
        SongBrowser::SongBrowserModel::didFinishProcessingSongs -= {&ProgressBar::SongBrowserFinishedProcessingSongs, this};
    }

    void ProgressBar::Awake()
    {
        get_gameObject()->get_transform()->set_position(Position);
        get_gameObject()->get_transform()->set_eulerAngles(Rotation);
        get_gameObject()->get_transform()->set_localScale(Scale);

        canvas = get_gameObject()->AddComponent<UnityEngine::Canvas*>();
        canvas->set_renderMode(UnityEngine::RenderMode::WorldSpace);
        get_gameObject()->AddComponent<HMUI::CurvedCanvasSettings*>()->SetRadius(0.0f);
        get_gameObject()->SetActive(false);

        auto ct = canvas->get_transform();
        ct->set_position(Position);
        ct->set_localScale(Scale);

        auto rectTransform = reinterpret_cast<UnityEngine::RectTransform*>(ct);
        rectTransform->set_sizeDelta(CanvasSize);

        // why set everything after creating it in the first place ?
        authorNameText = UIUtils::CreateText(ct, AuthorNameText, AuthorNameFontSize, AuthorNamePosition, HeaderSize);
        pluginNameText = UIUtils::CreateText(ct, PluginNameText, PluginNameFontSize, PluginNamePosition, HeaderSize);
        headerText = UIUtils::CreateText(ct, HeaderText, HeaderFontSize, HeaderPosition, HeaderSize);

        loadingBackground = UnityEngine::GameObject::New_ctor(il2cpp_utils::newcsstr("Background"))->AddComponent<UnityEngine::UI::Image*>();
        rectTransform = reinterpret_cast<UnityEngine::RectTransform*>(loadingBackground->get_transform());
        rectTransform->SetParent(ct, false); 
        rectTransform->set_sizeDelta(LoadingBarSize);
        loadingBackground->set_color(BackgroundColor);

        loadingBar = UnityEngine::GameObject::New_ctor(il2cpp_utils::newcsstr("Loading Bar"))->AddComponent<UnityEngine::UI::Image*>();
        rectTransform = reinterpret_cast<UnityEngine::RectTransform*>(loadingBar->get_transform());
        rectTransform->SetParent(ct, false);
        rectTransform->set_sizeDelta(LoadingBarSize);
        auto tex = UnityEngine::Texture2D::get_whiteTexture();
        auto sprite = UnityEngine::Sprite::Create(tex, UnityEngine::Rect(0, 0, tex->get_width(), tex->get_height()), {0.5f, 0.5f}, 100, 1, UnityEngine::SpriteMeshType::FullRect, UnityEngine::Vector4::get_zero(), false);
        loadingBar->set_sprite(sprite);
        loadingBar->set_type(UnityEngine::UI::Image::Type::Filled);
        loadingBar->set_fillMethod(UnityEngine::UI::Image::FillMethod::Horizontal);
        loadingBar->set_color({1.0f, 1.0f, 1.0f, 0.5f});

        UnityEngine::Object::DontDestroyOnLoad(get_gameObject());
        inited = true;
    }

    void ProgressBar::Update()
    {
        // idk if this is even possible due to our runtime loader, might need to PR into that api for getting progress loading ?
        /*
        if (!canvas || !canvas->get_enabled()) return;
        loadingBar->set_fillAmount(loader.LoadingProgress);

        _loadingBar.color = HSBColor.ToColor(new HSBColor(Mathf.PingPong(Time.time * 0.35f, 1), 1, 1));
        _headerText.color = HSBColor.ToColor(new HSBColor(Mathf.PingPong(Time.time * 0.35f, 1), 1, 1));
        */
    }

    void ProgressBar::SongLoaderOnLoadingStartedEvent()
    {
        StopAllCoroutines();
        if (!inited) return;
        showingMessage = false;
        headerText->set_text(il2cpp_utils::newcsstr(HeaderText));
        loadingBar->set_enabled(true);
        loadingBackground->set_enabled(true);
        get_gameObject()->SetActive(true);
    }

    void ProgressBar::SongBrowserFinishedProcessingSongs(const std::vector<GlobalNamespace::CustomPreviewBeatmapLevel*>& levels)
    {
        if (!inited) return;
        showingMessage = false;
        headerText->set_text(il2cpp_utils::newcsstr(string_format("%lu songs processed.", levels.size())));
        loadingBar->set_enabled(false);
        loadingBackground->set_enabled(false);
        StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(DisableCanvasRoutine(7.0f))));
    }

    void ProgressBar::SceneManagerOnActiveSceneChanged(UnityEngine::SceneManagement::Scene oldScene, UnityEngine::SceneManagement::Scene newScene)
    {
        if (!inited) return;
        static auto MenuCore = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("MenuCore"); 
        get_gameObject()->SetActive(MenuCore->Equals(newScene.get_name()) && showingMessage);
    }

    custom_types::Helpers::Coroutine ProgressBar::DisableCanvasRoutine(float time)
    {
        if (!inited) co_return;
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForSecondsRealtime::New_ctor(time));
        get_gameObject()->SetActive(false);
        showingMessage = false;
        co_return;
    }
}