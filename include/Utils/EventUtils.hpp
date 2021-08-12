#pragma once

#include "beatsaber-hook/shared/utils/typedefs-wrappers.hpp"
#include "UI/Browser/BeatSaberUIController.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/UI/Toggle.hpp"

#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/IAnnotatedBeatmapLevelCollection.hpp"
#include "GlobalNamespace/LevelFilteringNavigationController.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"
#include "GlobalNamespace/LevelCollectionViewController.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"

namespace EventUtils
{
    using OnActiveSceneChangedEvent = UnorderedEventCallback<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::Scene>;
    using DidSelectAnnotatedBeatmapLevelCollectionEvent = UnorderedEventCallback<GlobalNamespace::LevelFilteringNavigationController *, GlobalNamespace::IAnnotatedBeatmapLevelCollection *, UnityEngine::GameObject *, GlobalNamespace::BeatmapCharacteristicSO *>;
    using DidSelectLevelEvent = UnorderedEventCallback<GlobalNamespace::LevelCollectionViewController*, GlobalNamespace::IPreviewBeatmapLevel*>;
    using DidChangeContentEvent = UnorderedEventCallback<GlobalNamespace::StandardLevelDetailViewController*, GlobalNamespace::StandardLevelDetailViewController::ContentType>;
    using DidChangeDifficultyBeatmapEvent = UnorderedEventCallback<GlobalNamespace::StandardLevelDetailViewController*, GlobalNamespace::IDifficultyBeatmap*>;
    using DidSelectAnnotatedBeatmapLevelCollectionEvent_1Arg = UnorderedEventCallback<GlobalNamespace::IAnnotatedBeatmapLevelCollection*>;
    using DidSelectBeatmapCharacteristicEvent = UnorderedEventCallback<GlobalNamespace::BeatmapCharacteristicSegmentedControlController*, GlobalNamespace::BeatmapCharacteristicSO*>;
    using DidFavoriteToggleChangeEvent = UnorderedEventCallback<GlobalNamespace::StandardLevelDetailView*, UnityEngine::UI::Toggle*>;

    OnActiveSceneChangedEvent& OnActiveSceneChanged();
    DidSelectAnnotatedBeatmapLevelCollectionEvent& DidSelectAnnotatedBeatmapLevelCollection();
    DidSelectLevelEvent& DidSelectLevel();
    DidChangeContentEvent& DidChangeContent();
    DidChangeDifficultyBeatmapEvent& DidChangeDifficultyBeatmap();
    DidSelectAnnotatedBeatmapLevelCollectionEvent_1Arg& DidSelectAnnotatedBeatmapLevelCollection_1Arg();
    DidSelectBeatmapCharacteristicEvent& DidSelectBeatmapCharacteristic();
    DidFavoriteToggleChangeEvent& DidFavoriteToggleChange(); 
    
    void Init(SongBrowser::DataAccess::BeatSaberUIController* beatUi);
}