#pragma once

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/CustomPreviewBeatmapLevel.hpp"

#include "DataAccess/SongBrowserModel.hpp"
#include "UI/ProgressBar.hpp"
#include "UI/Browser/SongBrowserUI.hpp"

#include <vector>

#define SELF_DECLARE_STATIC_FIELD(type_, name_) \
private: \
struct ___StaticFieldRegistrator_##name_ : ::custom_types::StaticFieldRegistrator { \
    size_t oft; \
    ___StaticFieldRegistrator_##name_() { \
        oft = ___TargetType::___TypeRegistration::addStaticField(size()); \
        ___TargetType::___TypeRegistration::addStaticFieldInstance(this); \
    } \
    constexpr const char* name() const override { \
        return #name_; \
    } \
    const Il2CppType* type() const override { \
        ::il2cpp_functions::Init(); \
        return ::il2cpp_functions::class_get_type(___TypeRegistration::klass_ptr); \
    } \
    constexpr uint16_t fieldAttributes() const override { \
        return FIELD_ATTRIBUTE_PUBLIC | FIELD_ATTRIBUTE_STATIC; \
    } \
    constexpr size_t size() const override { \
        return sizeof(type_); \
    } \
    int32_t offset() const override { \
        return oft; \
    } \
}; \
static inline ___StaticFieldRegistrator_##name_ ___##name_##_StaticFieldRegistrator; \
public: \
static type_& name_() { \
    CRASH_UNLESS(___TargetType::___TypeRegistration::st_fields); \
    return *reinterpret_cast<type_*>(&___TargetType::___TypeRegistration::st_fields[___##name_##_StaticFieldRegistrator.offset()]); \
}

DECLARE_CLASS_CODEGEN(SongBrowser, SongBrowserApplication, UnityEngine::MonoBehaviour,
    //SELF_DECLARE_STATIC_FIELD(SongBrowser::SongBrowserApplication*, instance);

    DECLARE_INSTANCE_FIELD(SongBrowser::UI::SongBrowserUI*, songBrowserUI);
    DECLARE_INSTANCE_FIELD(SongBrowser::SongBrowserModel*, songBrowserModel);

    //DECLARE_STATIC_FIELD(SongBrowser::UI::ProgressBar*, mainProgressBar);

    DECLARE_INSTANCE_FIELD(bool, hasShownProgressBar);
    
    public:
        static void OnLoad();

    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, Start);
    private:
        /* private void OnScoreSaberDataDownloaded() */
        void OnSongLoaderLoadedSongs(const std::vector<GlobalNamespace::CustomPreviewBeatmapLevel*>& levels);
    public:
        static SongBrowser::UI::ProgressBar* mainProgressBar;
        static SongBrowser::SongBrowserApplication* instance;
        void HandleSoloModeSelection();
        void HandlePartyModeSelection();
        void HandleCampaignModeSelection();
        void HandleMultiplayerModeSelection();
        void HandleModeSelection(GlobalNamespace::MainMenuViewController::MenuButton mode);
    private:
        /* coro, in C# IEnumerator */
        custom_types::Helpers::Coroutine UpdateBrowserUI();
)