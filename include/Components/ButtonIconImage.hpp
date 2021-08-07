#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

#include "UnityEngine/UI/Image.hpp"
#include <string_view>
DECLARE_CLASS_CODEGEN(SongBrowser::Components, ButtonIconImage, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Image*, image);

    void SetIcon(std::string_view path);
)