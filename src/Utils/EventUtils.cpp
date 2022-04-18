#include "Utils/EventUtils.hpp"
#include "logging.hpp"

#include "UnityEngine/SceneManagement/SceneManager.hpp"

#include "System/Action_1.hpp"
#include "System/Action_2.hpp"
#include "System/Action_4.hpp"

#include <functional>

template <class T, typename Method>
static T MakeDelegate(Method fun)
{
    return il2cpp_utils::MakeDelegate<T>(classof(T), fun);
}

namespace EventUtils
{
    OnActiveSceneChangedEvent onActiveSceneChanged;
    DidSelectAnnotatedBeatmapLevelCollectionEvent didSelectAnnotatedBeatmapLevelCollectionEvent;
    DidSelectLevelEvent didSelectLevelEvent;
    DidChangeContentEvent didChangeContentEvent;
    DidChangeDifficultyBeatmapEvent didChangeDifficultyBeatmapEvent;
    DidSelectAnnotatedBeatmapLevelCollectionEvent_1Arg didSelectAnnotatedBeatmapLevelCollectionEvent_1Arg;
    DidSelectBeatmapCharacteristicEvent didSelectBeatmapCharacteristicEvent;
    DidFavoriteToggleChangeEvent didFavoriteToggleChangeEvent;
    DidSelectLevelCategoryEvent didSelectLevelCategoryEvent;

    OnActiveSceneChangedEvent& OnActiveSceneChanged()
    {
        return onActiveSceneChanged;
    }

    DidSelectAnnotatedBeatmapLevelCollectionEvent& DidSelectAnnotatedBeatmapLevelCollection()
    {
        return didSelectAnnotatedBeatmapLevelCollectionEvent;
    }

    DidSelectLevelEvent& DidSelectLevel()
    {
        return didSelectLevelEvent;
    }

    DidChangeContentEvent& DidChangeContent()
    {
        return didChangeContentEvent;
    }

    DidChangeDifficultyBeatmapEvent& DidChangeDifficultyBeatmap()
    {
        return didChangeDifficultyBeatmapEvent;
    }

    DidSelectAnnotatedBeatmapLevelCollectionEvent_1Arg& DidSelectAnnotatedBeatmapLevelCollection_1Arg()
    {
        return didSelectAnnotatedBeatmapLevelCollectionEvent_1Arg;
    }

    DidSelectBeatmapCharacteristicEvent& DidSelectBeatmapCharacteristic()
    {
        return didSelectBeatmapCharacteristicEvent;
    }

    DidFavoriteToggleChangeEvent& DidFavoriteToggleChange()
    {
        return didFavoriteToggleChangeEvent;
    }

    DidSelectLevelCategoryEvent& DidSelectLevelCategory()
    {
        return didSelectLevelCategoryEvent;
    }
    
    void Reset()
    {
        onActiveSceneChanged.clear();
        didSelectAnnotatedBeatmapLevelCollectionEvent.clear();
        didSelectLevelEvent.clear();
        didChangeContentEvent.clear();
        didChangeDifficultyBeatmapEvent.clear();
        didSelectAnnotatedBeatmapLevelCollectionEvent_1Arg.clear();
        didSelectBeatmapCharacteristicEvent.clear();
        didFavoriteToggleChangeEvent.clear();
        didSelectLevelCategoryEvent.clear();
    }

    void Init(SongBrowser::DataAccess::BeatSaberUIController* beatUi)
    {
        INFO("Initing events");
        
        INFO("LevelFilteringNavigationController->add_didSelectAnnotatedBeatmapLevelCollectionEvent");
        std::function<void(GlobalNamespace::LevelFilteringNavigationController *, GlobalNamespace::IAnnotatedBeatmapLevelCollection *, UnityEngine::GameObject *, GlobalNamespace::BeatmapCharacteristicSO *)> didSelectAnnotatedBeatmapLevelCollectionFun = std::bind(&DidSelectAnnotatedBeatmapLevelCollectionEvent::invoke, &didSelectAnnotatedBeatmapLevelCollectionEvent, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        auto didSelectAnnotatedBeatmapLevelCollectionDelegate = MakeDelegate<System::Action_4<GlobalNamespace::LevelFilteringNavigationController *, GlobalNamespace::IAnnotatedBeatmapLevelCollection *, UnityEngine::GameObject *, GlobalNamespace::BeatmapCharacteristicSO *>*>(didSelectAnnotatedBeatmapLevelCollectionFun);
        beatUi->LevelFilteringNavigationController->add_didSelectAnnotatedBeatmapLevelCollectionEvent(didSelectAnnotatedBeatmapLevelCollectionDelegate);
        
        INFO("LevelCollectionViewController->add_didSelectLevelEvent");
        std::function<void(GlobalNamespace::LevelCollectionViewController*, GlobalNamespace::IPreviewBeatmapLevel*)> didSelectLevelFun = std::bind(&DidSelectLevelEvent::invoke, &didSelectLevelEvent, std::placeholders::_1, std::placeholders::_2);
        auto didSelectLevelDelegate = MakeDelegate<System::Action_2<GlobalNamespace::LevelCollectionViewController*, GlobalNamespace::IPreviewBeatmapLevel*>*>(didSelectLevelFun);
        beatUi->LevelCollectionViewController->add_didSelectLevelEvent(didSelectLevelDelegate);
        
        INFO("LevelDetailViewController->add_didChangeContentEvent");
        std::function<void(GlobalNamespace::StandardLevelDetailViewController*, GlobalNamespace::StandardLevelDetailViewController::ContentType)> didChangeContentFun = std::bind(&DidChangeContentEvent::invoke, &didChangeContentEvent, std::placeholders::_1, std::placeholders::_2);
        auto didChangeContentDelegate = MakeDelegate<System::Action_2<GlobalNamespace::StandardLevelDetailViewController*, GlobalNamespace::StandardLevelDetailViewController::ContentType>*>(didChangeContentFun);
        beatUi->LevelDetailViewController->add_didChangeContentEvent(didChangeContentDelegate);

        INFO("LevelDetailViewController->add_didChangeDifficultyBeatmapEvent");
        std::function<void(GlobalNamespace::StandardLevelDetailViewController*, GlobalNamespace::IDifficultyBeatmap*)> didChangeDifficultyBeatmapFun = std::bind(&DidChangeDifficultyBeatmapEvent::invoke, &didChangeDifficultyBeatmapEvent, std::placeholders::_1, std::placeholders::_2);
        auto didChangeDifficultyBeatmapDelegate = MakeDelegate<System::Action_2<GlobalNamespace::StandardLevelDetailViewController*, GlobalNamespace::IDifficultyBeatmap*>*>(didChangeDifficultyBeatmapFun);
        beatUi->LevelDetailViewController->add_didChangeDifficultyBeatmapEvent(didChangeDifficultyBeatmapDelegate);
        
        INFO("AnnotatedBeatmapLevelCollectionsViewController->add_didSelectAnnotatedBeatmapLevelCollectionEvent");
        std::function<void(GlobalNamespace::IAnnotatedBeatmapLevelCollection*)> didSelectAnnotatedBeatmapLevelCollectionEvent_1ArgFun = std::bind(&DidSelectAnnotatedBeatmapLevelCollectionEvent_1Arg::invoke, &didSelectAnnotatedBeatmapLevelCollectionEvent_1Arg, std::placeholders::_1);
        auto didSelectAnnotatedBeatmapLevelCollectionEvent_1ArgDelegate = MakeDelegate<System::Action_1<GlobalNamespace::IAnnotatedBeatmapLevelCollection*>*>(didSelectAnnotatedBeatmapLevelCollectionEvent_1ArgFun);
        beatUi->AnnotatedBeatmapLevelCollectionsViewController->add_didSelectAnnotatedBeatmapLevelCollectionEvent(didSelectAnnotatedBeatmapLevelCollectionEvent_1ArgDelegate);
        
        INFO("BeatmapCharacteristicSelectionViewController->add_didSelectBeatmapCharacteristicEvent");
        std::function<void(GlobalNamespace::BeatmapCharacteristicSegmentedControlController*, GlobalNamespace::BeatmapCharacteristicSO*)> didSelectBeatmapCharacteristicEventFun = std::bind(&DidSelectBeatmapCharacteristicEvent::invoke, &didSelectBeatmapCharacteristicEvent, std::placeholders::_1, std::placeholders::_2);
        auto didSelectBeatmapCharacteristicEventDelegate = MakeDelegate<System::Action_2<GlobalNamespace::BeatmapCharacteristicSegmentedControlController*, GlobalNamespace::BeatmapCharacteristicSO*>*>(didSelectBeatmapCharacteristicEventFun);
        beatUi->BeatmapCharacteristicSelectionViewController->add_didSelectBeatmapCharacteristicEvent(didSelectBeatmapCharacteristicEventDelegate);
        
        INFO("StandardLevelDetailView->add_didFavoriteToggleChangeEvent");
        std::function<void(GlobalNamespace::StandardLevelDetailView*, UnityEngine::UI::Toggle*)> didFavoriteToggleChangeEventFun = std::bind(&DidFavoriteToggleChangeEvent::invoke, &didFavoriteToggleChangeEvent, std::placeholders::_1, std::placeholders::_2);
        auto didFavoriteToggleChangeDelegate = MakeDelegate<System::Action_2<GlobalNamespace::StandardLevelDetailView*, UnityEngine::UI::Toggle*>*>(didFavoriteToggleChangeEventFun);
        beatUi->StandardLevelDetailView->add_didFavoriteToggleChangeEvent(didFavoriteToggleChangeDelegate);

        INFO("beatUi->LevelFilteringNavigationController->selectLevelCategoryViewController->add_didSelectLevelCategoryEvent");
        std::function<void(GlobalNamespace::SelectLevelCategoryViewController*, GlobalNamespace::SelectLevelCategoryViewController::LevelCategory)> didSelectLevelCategoryEventFun = std::bind(&DidSelectLevelCategoryEvent::invoke, &didSelectLevelCategoryEvent, std::placeholders::_1, std::placeholders::_2);
        auto didSelectLevelCategoryEventDelegate = MakeDelegate<System::Action_2<GlobalNamespace::SelectLevelCategoryViewController*, GlobalNamespace::SelectLevelCategoryViewController::LevelCategory>*>(didSelectLevelCategoryEventFun);
        beatUi->LevelFilteringNavigationController->dyn__selectLevelCategoryViewController()->add_didSelectLevelCategoryEvent(didSelectLevelCategoryEventDelegate);
    }
}