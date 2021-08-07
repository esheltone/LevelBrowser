#include "hooks.hpp"
#include "logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "custom-types/shared/register.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "System/Action.hpp"

#include "GlobalNamespace/SoloFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/MultiplayerLevelSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/PartyFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/CampaignFlowCoordinator.hpp"

#include "SongBrowserApplication.hpp"

ModInfo modInfo = {ID, VERSION};

MAKE_AUTO_HOOK_MATCH(FlowCoordinator_PresentFlowCoordinator, &HMUI::FlowCoordinator::PresentFlowCoordinator, void, HMUI::FlowCoordinator* self, HMUI::FlowCoordinator* flowCoordinator, System::Action* finishedCallback, HMUI::ViewController::AnimationDirection animationDirection, bool immediately, bool replaceTopViewController)
{
    FlowCoordinator_PresentFlowCoordinator(self, flowCoordinator, finishedCallback, animationDirection, immediately, replaceTopViewController);
    // postfix
    if (il2cpp_utils::try_cast<GlobalNamespace::SoloFreePlayFlowCoordinator>(flowCoordinator))
    {
        INFO("Initializing SongBrowser for Single Player Mode");
        SongBrowser::SongBrowserApplication::instance()->HandleSoloModeSelection();
    }
    else if (il2cpp_utils::try_cast<GlobalNamespace::MultiplayerLevelSelectionFlowCoordinator>(flowCoordinator))
    {
        INFO("Initializing SongBrowser for Multiplayer Mode");
        SongBrowser::SongBrowserApplication::instance()->HandleMultiplayerModeSelection();
    }
    else if (il2cpp_utils::try_cast<GlobalNamespace::PartyFreePlayFlowCoordinator>(flowCoordinator))
    {
        INFO("Initializing SongBrowser for Party Mode");
        SongBrowser::SongBrowserApplication::instance()->HandlePartyModeSelection();
    }
    else if (il2cpp_utils::try_cast<GlobalNamespace::CampaignFlowCoordinator>(flowCoordinator))
    {
        INFO("Initializing SongBrowser for Multiplayer Mode");
        SongBrowser::SongBrowserApplication::instance()->HandleCampaignModeSelection();
    }
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