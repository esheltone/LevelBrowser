#pragma once

#include "UnityEngine/Sprite.hpp"

namespace SpriteUtils
{
    UnityEngine::Sprite* get_StarFullIcon();
    UnityEngine::Sprite* get_SpeedIcon();
    UnityEngine::Sprite* get_GraphIcon();
    UnityEngine::Sprite* get_DeleteIcon();
    UnityEngine::Sprite* get_XIcon();
    UnityEngine::Sprite* get_DoubleArrow();
    UnityEngine::Sprite* get_RandomIcon();
    UnityEngine::Sprite* get_NoteStartOffsetIcon();
    UnityEngine::Sprite* get_PlaylistIcon();
    
    void Reset();
    void Init();
}