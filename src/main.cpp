#include "hooks.hpp"
#include "logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "custom-types/shared/register.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "System/Action.hpp"

#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"

#include "GlobalNamespace/SoloFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/MultiplayerLevelSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/PartyFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/CampaignFlowCoordinator.hpp"
#include "GlobalNamespace/MainFlowCoordinator.hpp"
#include "GlobalNamespace/LoadingControl.hpp"

#include "SongBrowserApplication.hpp"
#include "Utils/EventUtils.hpp"

#include "GlobalNamespace/MenuTransitionsHelper.hpp"
#include "Zenject/DiContainer.hpp"
#include "System/Action_1.hpp"

ModInfo modInfo = {MOD_ID, VERSION};

UnityEngine::SceneManagement::Scene oldScene;
bool firstLoad = true;
MAKE_AUTO_HOOK_MATCH(SceneManager_SetActiveScene, &UnityEngine::SceneManagement::SceneManager::SetActiveScene, bool, UnityEngine::SceneManagement::Scene newScene)
{
    bool result = SceneManager_SetActiveScene(newScene);
    if (firstLoad) firstLoad = false;
    else EventUtils::OnActiveSceneChanged().invoke(oldScene, newScene);

    oldScene = newScene;
    return result;
}

/*
// need to add these 2 hooks to remove the song loader delete button
MAKE_AUTO_HOOK_MATCH(StandardLevelDetailView_RefreshContent, &GlobalNamespace::StandardLevelDetailView::RefreshContent, void, GlobalNamespace::StandardLevelDetailView* self)
{
    StandardLevelDetailView_RefreshContent(self);
    static auto deleteLevelButtonName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("DeleteLevelButton");
    auto transform = self->practiceButton->get_transform()->get_parent()->Find(deleteLevelButtonName);
    if (transform) transform->get_gameObject()->SetActive(false);
}

MAKE_AUTO_HOOK_MATCH(StandardLevelDetailViewController_ShowContent, &GlobalNamespace::StandardLevelDetailViewController::ShowContent, void, GlobalNamespace::StandardLevelDetailViewController* self, GlobalNamespace::StandardLevelDetailViewController::ContentType contentType, Il2CppString* errorText, float downloadingProgress, Il2CppString* downloadingText)
{
    StandardLevelDetailViewController_ShowContent(self, contentType, errorText, downloadingProgress, downloadingText);
    static auto deleteLevelButtonName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("DeleteLevelButton");
    auto transform = self->loadingControl->refreshButton->get_transform()->get_parent()->Find(deleteLevelButtonName);
    if (transform) transform->get_gameObject()->SetActive(false);
}
*/

MAKE_AUTO_HOOK_MATCH(FlowCoordinator_PresentFlowCoordinator, &HMUI::FlowCoordinator::PresentFlowCoordinator, void, HMUI::FlowCoordinator* self, HMUI::FlowCoordinator* flowCoordinator, System::Action* finishedCallback, HMUI::ViewController::AnimationDirection animationDirection, bool immediately, bool replaceTopViewController)
{
    FlowCoordinator_PresentFlowCoordinator(self, flowCoordinator, finishedCallback, animationDirection, immediately, replaceTopViewController);
    INFO("Checking if instance exists");
    //if (!SongBrowser::SongBrowserApplication::instance) SongBrowser::SongBrowserApplication::OnLoad();
    INFO("Going Ahead with checks");

    // postfix
    if (il2cpp_utils::try_cast<GlobalNamespace::SoloFreePlayFlowCoordinator>(flowCoordinator))
    {
        INFO("Initializing SongBrowser for Single Player Mode");
        SongBrowser::SongBrowserApplication::instance->HandleSoloModeSelection();
    }
    else if (il2cpp_utils::try_cast<GlobalNamespace::MultiplayerLevelSelectionFlowCoordinator>(flowCoordinator))
    {
        INFO("Initializing SongBrowser for Multiplayer Mode");
        SongBrowser::SongBrowserApplication::instance->HandleMultiplayerModeSelection();
    }
    else if (il2cpp_utils::try_cast<GlobalNamespace::PartyFreePlayFlowCoordinator>(flowCoordinator))
    {
        INFO("Initializing SongBrowser for Party Mode");
        SongBrowser::SongBrowserApplication::instance->HandlePartyModeSelection();
    }
    else if (il2cpp_utils::try_cast<GlobalNamespace::CampaignFlowCoordinator>(flowCoordinator))
    {
        INFO("Initializing SongBrowser for Multiplayer Mode");
        SongBrowser::SongBrowserApplication::instance->HandleCampaignModeSelection();
    }
}

MAKE_AUTO_HOOK_MATCH(MainFlowCoordinator_DidActivate, &GlobalNamespace::MainFlowCoordinator::DidActivate, void, GlobalNamespace::MainFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    if (!SongBrowser::SongBrowserApplication::instance) SongBrowser::SongBrowserApplication::OnLoad();
    MainFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
}

MAKE_AUTO_HOOK_MATCH(MenuTransitionsHelper_RestartGame, &GlobalNamespace::MenuTransitionsHelper::RestartGame, void, GlobalNamespace::MenuTransitionsHelper* self, System::Action_1<Zenject::DiContainer*>* finishCallback)
{
    INFO("Game is soft restarting, handling it by throwing away pointers!");
    SongBrowser::SongBrowserApplication::Reset();
    EventUtils::Reset();
    MenuTransitionsHelper_RestartGame(self, finishCallback);
}

extern "C" void setup(ModInfo& info)
{
    info.id = modInfo.id;
    info.version = modInfo.version;
}

extern "C" void load()
{
    INFO("Loading songbrowser...");
    Hooks::InstallHooks(SongBrowser::Logging::getLogger());

    custom_types::Register::AutoRegister();    

    INFO("Loaded songbrowser!");
}