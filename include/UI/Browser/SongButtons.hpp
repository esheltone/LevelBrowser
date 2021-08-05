#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "DataAccess/config.hpp"

DECLARE_CLASS_CODEGEN(SongBrowser::UI, SongFilterButton, Il2CppObject,
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, button);
    SongFilterMode filterMode;
)

DECLARE_CLASS_CODEGEN(SongBrowser::UI, SongSortButton, Il2CppObject,
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, button);
    SongSortMode sortMode;
)