#include "hooks.hpp"
#include "logging.hpp"
#include "modloader/shared/modloader.hpp"

ModInfo modInfo = {ID, VERSION};

extern "C" void setup(ModInfo& info)
{
    info.id = modInfo.id;
    info.version = modInfo.version;
}

extern "C" void load()
{
    INFO("Loading songbrowser...");
    Hooks::InstallHooks(SongBrowser::Logging::getLogger());
    
    
    INFO("Loaded songbrowser!");
}