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

#include "SongBrowserApplication.hpp"
#include "Utils/EventUtils.hpp"

ModInfo modInfo = {ID, VERSION};

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

MAKE_AUTO_HOOK_MATCH(FlowCoordinator_PresentFlowCoordinator, &HMUI::FlowCoordinator::PresentFlowCoordinator, void, HMUI::FlowCoordinator* self, HMUI::FlowCoordinator* flowCoordinator, System::Action* finishedCallback, HMUI::ViewController::AnimationDirection animationDirection, bool immediately, bool replaceTopViewController)
{
    FlowCoordinator_PresentFlowCoordinator(self, flowCoordinator, finishedCallback, animationDirection, immediately, replaceTopViewController);
    INFO("Checking if instance exists");
    if (!SongBrowser::SongBrowserApplication::instance) SongBrowser::SongBrowserApplication::OnLoad();
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
    //SongBrowser::SongBrowserApplication::OnLoad();
    MainFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
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