#include "UI/Browser/BeatSaberUIController.hpp"
#include "beatsaber-hook/shared/utils/typedefs-array.hpp"
#include "GlobalNamespace/IBeatmapLevelCollection.hpp"

#include "Utils/EventUtils.hpp"
#include "Utils/ArrayUtil.hpp"
#include "Utils/EnumToStringUtils.hpp"

#include "logging.hpp"

#include "HMUI/ScrollView.hpp"
#include "HMUI/TableView.hpp"
#include "HMUI/TableView_ScrollPositionType.hpp"
#include "HMUI/IconSegmentedControl.hpp"
#include "UnityEngine/Resources.hpp"
#include "GlobalNamespace/BeatmapLevelPackCollectionSO.hpp"
#include "GlobalNamespace/SelectLevelCategoryViewController_LevelCategoryInfo.hpp"

#include "DataAccess/SongBrowserModel.hpp"

DEFINE_TYPE(SongBrowser::DataAccess, BeatSaberUIController);

namespace SongBrowser::DataAccess
{
    void BeatSaberUIController::ctor(GlobalNamespace::LevelSelectionFlowCoordinator* flowCoordinator)
    {
        INFO("Collecting all BeatSaberUI Elements...");
        detectedTwitchPluginQueue = false;
        checkedForTwitchPlugin = false;

        LevelSelectionFlowCoordinator = flowCoordinator;

        // gather flow coordinator elements
        LevelSelectionNavigationController = LevelSelectionFlowCoordinator->levelSelectionNavigationController;
        INFO("Acquired LevelSelectionNavigationController [%d]", LevelSelectionNavigationController->GetInstanceID());

        LevelFilteringNavigationController = LevelSelectionNavigationController->levelFilteringNavigationController;
        INFO("Acquired LevelFilteringNavigationController [%d]", LevelFilteringNavigationController->GetInstanceID());

        LevelCollectionNavigationController = LevelSelectionNavigationController->levelCollectionNavigationController;
        INFO("Acquired LevelCollectionNavigationController [%d]", LevelCollectionNavigationController->GetInstanceID());

        LevelCollectionViewController = LevelCollectionNavigationController->levelCollectionViewController;
        INFO("Acquired LevelPackLevelsViewController [%d]", LevelCollectionViewController->GetInstanceID());

        LevelDetailViewController = LevelCollectionNavigationController->levelDetailViewController;
        INFO("Acquired StandardLevelDetailViewController [%d]", LevelDetailViewController->GetInstanceID());

        LevelCollectionTableView = this->LevelCollectionViewController->levelCollectionTableView;
        INFO("Acquired LevelPackLevelsTableView [%d]", LevelCollectionTableView->GetInstanceID());

        StandardLevelDetailView = LevelDetailViewController->standardLevelDetailView;
        INFO("Acquired StandardLevelDetailView [%d]", StandardLevelDetailView->GetInstanceID());

        BeatmapCharacteristicSelectionViewController = StandardLevelDetailView->beatmapCharacteristicSegmentedControlController;
        INFO("Acquired BeatmapCharacteristicSegmentedControlController [%d]", BeatmapCharacteristicSelectionViewController->GetInstanceID());

        LevelDifficultyViewController = StandardLevelDetailView->beatmapDifficultySegmentedControlController;
        INFO("Acquired BeatmapDifficultySegmentedControlController [%d]", LevelDifficultyViewController->GetInstanceID());

        LevelCollectionTableViewTransform = reinterpret_cast<UnityEngine::RectTransform*>(LevelCollectionTableView->get_transform());
        INFO("Acquired TableViewRectTransform from LevelPackLevelsTableView [%d]", LevelCollectionTableViewTransform->GetInstanceID());

        AnnotatedBeatmapLevelCollectionsViewController = LevelFilteringNavigationController->annotatedBeatmapLevelCollectionsViewController;
        INFO("Acquired AnnotatedBeatmapLevelCollectionsViewController from LevelFilteringNavigationController [%d]", AnnotatedBeatmapLevelCollectionsViewController->GetInstanceID());

        auto tableView = LevelCollectionTableView->tableView;
        auto scrollView = tableView->scrollView;
        TableViewPageUpButton = scrollView->pageUpButton;
        TableViewPageDownButton = scrollView->pageDownButton;
        INFO("Acquired Page Up and Down buttons...");

        ActionButtons = StandardLevelDetailView->GetComponentsInChildren<UnityEngine::RectTransform*>().First([](auto x) {
            static auto ActionButtons = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("ActionButtons");
            return x->get_name()->Equals(ActionButtons);
        });
        INFO("Acquired ActionButtons [%d]", ActionButtons->GetInstanceID());

        ScreenSystem = UnityEngine::Resources::FindObjectsOfTypeAll<HMUI::ScreenSystem*>().Last();
        INFO("Acquired ScreenSystem [%d]", ScreenSystem->GetInstanceID());

        SimpleDialogPromptViewControllerPrefab = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::SimpleDialogPromptViewController*>().Last();
        INFO("Acquired SimpleDialogPromptViewControllerPrefab [%d]", SimpleDialogPromptViewControllerPrefab->GetInstanceID());

        BeatmapLevelsModel = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::BeatmapLevelsModel*>().Last();
        INFO("Acquired BeatmapLevelsModel [%d]", BeatmapLevelsModel->GetInstanceID());
        
        EventUtils::Init(this);
    }

    GlobalNamespace::IBeatmapLevelPack* BeatSaberUIController::GetCurrentSelectedLevelPack()
    {
        if (!LevelCollectionNavigationController)
            return nullptr;

        return LevelCollectionNavigationController->levelPack;
    }

    GlobalNamespace::IAnnotatedBeatmapLevelCollection* BeatSaberUIController::GetCurrentSelectedPlaylist()
    {
        if (!AnnotatedBeatmapLevelCollectionsViewController)
            return nullptr;

        return AnnotatedBeatmapLevelCollectionsViewController->get_selectedAnnotatedBeatmapLevelCollection();
    }

    GlobalNamespace::IAnnotatedBeatmapLevelCollection* BeatSaberUIController::GetCurrentSelectedAnnotatedBeatmapLevelCollection()
    {
        auto* collection = reinterpret_cast<GlobalNamespace::IAnnotatedBeatmapLevelCollection*>(GetCurrentSelectedLevelPack());
        if (!collection) {
            collection = GetCurrentSelectedPlaylist();
        }

        return collection;
    }

    GlobalNamespace::IAnnotatedBeatmapLevelCollection* BeatSaberUIController::GetLevelCollectionByName(const std::string& levelCollectionName)
    {
        auto levelCollectionNameCS = il2cpp_utils::newcsstr(levelCollectionName);
        GlobalNamespace::IAnnotatedBeatmapLevelCollection* levelCollection = nullptr;

        // search level packs
        auto beatMapLevelPackCollection = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::BeatmapLevelPackCollectionSO*>().Last();
        auto levelPacks = beatMapLevelPackCollection->allBeatmapLevelPacks;
        int length = levelPacks->max_length;
        for (int i = 0; i < length; i++)
        {
            auto o = levelPacks->values[i];
            if (o->get_packName()->Equals(levelCollectionNameCS))
            {
                levelCollection = reinterpret_cast<GlobalNamespace::IAnnotatedBeatmapLevelCollection *>(o);
                break;
            }
        }

        // search playlists
        if (!levelCollection)
        {
            auto annotatedBeatmapLevelCollections = reinterpret_cast<Array<GlobalNamespace::IAnnotatedBeatmapLevelCollection *>*>(AnnotatedBeatmapLevelCollectionsViewController->annotatedBeatmapLevelCollections);
            length = annotatedBeatmapLevelCollections->max_length;
            for (int i = 0; i < length; i++)
            {
                auto c = annotatedBeatmapLevelCollections->values[i];
                if (c->get_collectionName()->Equals(levelCollectionNameCS))
                {
                    levelCollection = c;
                    break;
                }
            }
        }

        return levelCollection;
    }

    Array<GlobalNamespace::IPreviewBeatmapLevel*>* BeatSaberUIController::GetCurrentLevelCollectionLevels()
    {
        auto levelCollection = GetCurrentSelectedAnnotatedBeatmapLevelCollection();
        if (!levelCollection)
        {
            ERROR("Current selected level collection is null for some reason...");
            return nullptr;
        }

        return SongBrowserModel::GetLevelsForLevelCollection(levelCollection);
    }

    List<GlobalNamespace::IPreviewBeatmapLevel*>* BeatSaberUIController::GetCurrentLevelCollectionLevelsList()
    {
        auto levelCollection = GetCurrentSelectedAnnotatedBeatmapLevelCollection();
        if (!levelCollection)
        {
            ERROR("Current selected level collection is null for some reason...");
            return nullptr;
        }

        return SongBrowserModel::GetLevelsListForLevelCollection(levelCollection);
    }


    bool BeatSaberUIController::SelectLevelCategory(const std::string& input)
    {
        try
        {
            std::string levelCategoryName = input;
            if (levelCategoryName == "")
                // hack for now, just assume custom levels if a user has an old settings file, corrects itself first time they change level packs.
                levelCategoryName = "CustomSongs";

            GlobalNamespace::SelectLevelCategoryViewController::LevelCategory category;
            category = StringToLevelCategory(levelCategoryName);

            if (category == LevelFilteringNavigationController->get_selectedLevelCategory())
            {
                INFO("Level category [%d] is already selected", category.value);
                return false;
            }

            INFO("Selecting level category: %s", levelCategoryName.c_str());

            auto selectLeveCategoryViewController = LevelFilteringNavigationController->GetComponentInChildren<GlobalNamespace::SelectLevelCategoryViewController*>();
            auto iconSegmentController = selectLeveCategoryViewController->GetComponentInChildren<HMUI::IconSegmentedControl*>();

            int selectCellNumber = 0;
            // By default, we increment 1 too many, so we remove it if it wasn't 0
            selectCellNumber = selectCellNumber > 0 ? selectCellNumber - 1 : 0;

            iconSegmentController->SelectCellWithNumber(selectCellNumber);
            selectLeveCategoryViewController->LevelFilterCategoryIconSegmentedControlDidSelectCell(iconSegmentController, selectCellNumber);
            LevelFilteringNavigationController->UpdateSecondChildControllerContent(category);

            INFO("Done selecting level category.");

            return true;
        }
        catch (const std::exception& e)
        {
            ERROR("%s", e.what());
            return false;
        }
    }

    void BeatSaberUIController::SelectLevelCollection(const std::string& levelCollectionName)
    {
        try
        {
            auto collection = GetLevelCollectionByName(levelCollectionName);
            if (!collection)
            {
                INFO("Could not locate requested level collection...");
                return;
            }

            INFO("Selecting level collection: %s", levelCollectionName.c_str());

            LevelFilteringNavigationController->SelectAnnotatedBeatmapLevelCollection(reinterpret_cast<GlobalNamespace::IBeatmapLevelPack*>(collection));
            LevelFilteringNavigationController->HandleAnnotatedBeatmapLevelCollectionsViewControllerDidSelectAnnotatedBeatmapLevelCollection(collection);

            INFO("Done selecting level collection!");
        }
        catch (const std::exception& e)
        {
            ERROR("%s", e.what());
        }
    }

    void BeatSaberUIController::SelectAndScrollToLevel(const std::string& levelID)
    {
        INFO("Scrolling to LevelID: %s", levelID.c_str());

        // Check once per load
        if (!checkedForTwitchPlugin)
        {
            INFO("Checking for BeatSaber Twitch Integration Plugin...");
            // there is no twitch plugin, so we assume false for now
            detectedTwitchPluginQueue = false;//Resources.FindObjectsOfTypeAll<HMUI.ViewController>().Any(x => x.name == "RequestInfo");
            INFO("BeatSaber Twitch Integration plugin detected: %d", detectedTwitchPluginQueue);

            checkedForTwitchPlugin = true;
        }

        // Skip scrolling to level if twitch plugin has queue active.
        if (detectedTwitchPluginQueue)
        {
            INFO("Skipping SelectAndScrollToLevel() because we detected Twitch Integration Plugin has a Queue active...");
            return;
        }

        // try to find the index and scroll to it
        int selectedIndex;
        auto levels = GetCurrentLevelCollectionLevelsList();
        if (levels->size <= 0)
            return;

        auto levelID_cs = il2cpp_utils::newcsstr(levelID);
        // acquire the index or try the last row
        selectedIndex = ArrayUtil::FirstIndexOf(levels, [&](auto x) {
            return x && x->get_levelID()->Equals(levelID_cs); 
        });
        
        if (selectedIndex < 0)
        {
            // this might look like an off by one error but the _level list we keep is missing the header entry BeatSaber.
            // so the last row is +1 the max index, the count.
            int maxCount = levels->size;

            int selectedRow = LevelCollectionTableView->selectedRow;

            INFO("Song is not in the level pack, cannot scroll to it...  Using last known row %d/%d", selectedRow, maxCount);
            selectedIndex = maxCount > selectedRow ? selectedRow : maxCount;
        }
        else if (LevelCollectionViewController->showHeader)
        {
            // the header counts as an index, so if the index came from the level array we have to add 1.
            selectedIndex = 1;
        }

        ScrollToLevelByRow(selectedIndex);
    }

    void BeatSaberUIController::ScrollToLevelByRow(const int& selectedIndex)
    {
        INFO("Scrolling level list to idx: %d", selectedIndex);

        auto tableView = LevelCollectionTableView->tableView;
        int selectedRow = LevelCollectionTableView->selectedRow;
        if (selectedRow != selectedIndex && LevelCollectionTableView->get_isActiveAndEnabled())
        {
            LevelCollectionTableView->HandleDidSelectRowEvent(tableView, selectedIndex);
        }

        tableView->ScrollToCellWithIdx(selectedIndex, HMUI::TableView::ScrollPositionType::Beginning, true);
        tableView->SelectCellWithIdx(selectedIndex, false);
    }

    void BeatSaberUIController::RefreshSongList(const std::string& currentSelectedLevelId, bool scrollToLevel)
    {
        try
        {
            auto levels = GetCurrentLevelCollectionLevelsList();
            if (!levels)
            {
                INFO("Nothing to refresh yet.");
                return;
            }

            INFO("Checking if TableView is initialized...");
            auto tableView = LevelCollectionTableView->tableView;

            INFO("Reloading SongList TableView");
            tableView->ReloadData();

            INFO("Attempting to scroll to level [%s]", currentSelectedLevelId.c_str());
            std::string selectedLevelID = currentSelectedLevelId;
            if (selectedLevelID.empty())
            {
                if (levels->size > 0)
                {
                    INFO("Currently selected level ID does not exist, picking the first...");
                    auto firstLevelId = levels->get_Item(0)->get_levelID();
                    selectedLevelID = firstLevelId != nullptr && firstLevelId->get_Length() > 0 ? to_utf8(csstrtostr(firstLevelId)) : "";
                }
            }

            if (scrollToLevel && !selectedLevelID.empty())
            {
                SelectAndScrollToLevel(selectedLevelID);
            }
        }
        catch (const std::exception& e)
        {
            ERROR("%s", e.what());
        }
    }
}