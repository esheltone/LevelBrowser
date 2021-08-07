#include "Utils/SpriteUtils.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include <string_view>
#include <string>

namespace SpriteUtils
{
    UnityEngine::Sprite* StarFullIcon = nullptr;
    UnityEngine::Sprite* SpeedIcon = nullptr;
    UnityEngine::Sprite* GraphIcon = nullptr;
    UnityEngine::Sprite* DeleteIcon = nullptr;
    UnityEngine::Sprite* XIcon = nullptr;
    UnityEngine::Sprite* DoubleArrow = nullptr;
    UnityEngine::Sprite* RandomIcon = nullptr;
    UnityEngine::Sprite* NoteStartOffsetIcon = nullptr;
    UnityEngine::Sprite* PlaylistIcon = nullptr;

    UnityEngine::Sprite* get_StarFullIcon()
    {
        return StarFullIcon;
    }

    UnityEngine::Sprite* get_SpeedIcon()
    {
        return SpeedIcon;
    }

    UnityEngine::Sprite* get_GraphIcon()
    {
        return GraphIcon;
    }

    UnityEngine::Sprite* get_DeleteIcon()
    {
        return DeleteIcon;
    }

    UnityEngine::Sprite* get_XIcon()
    {
        return XIcon;
    }

    UnityEngine::Sprite* get_DoubleArrow()
    {
        return DoubleArrow;
    }

    UnityEngine::Sprite* get_RandomIcon()
    {
        return RandomIcon;
    }

    UnityEngine::Sprite* get_NoteStartOffsetIcon()
    {
        return NoteStartOffsetIcon;
    }

    UnityEngine::Sprite* get_PlaylistIcon()
    {
        return PlaylistIcon;
    }

    void Reset()
    {
        StarFullIcon = nullptr;
        SpeedIcon = nullptr;
        GraphIcon = nullptr;
        DeleteIcon = nullptr;
        XIcon = nullptr;
        DoubleArrow = nullptr;
        RandomIcon = nullptr;
        NoteStartOffsetIcon = nullptr;
        PlaylistIcon = nullptr;
    }
    
    UnityEngine::Sprite* LoadSprite(std::string_view image)
    {
        std::string path = string_format("/sdcard/ModData/com.beatgames.beatsaber/Mods/SongBrowser/Icons/%s", image.data());
        return QuestUI::BeatSaberUI::FileToSprite(path);
    }

    void Init()
    {
        SpeedIcon = LoadSprite("Speed.png");
        GraphIcon = LoadSprite("Graph.png");
        XIcon = LoadSprite("X.png");
        StarFullIcon = LoadSprite("StarFull.png");
        DeleteIcon = LoadSprite("DeleteIcon.png");
        DoubleArrow = LoadSprite("DoubleArrow.png");
        RandomIcon = LoadSprite("RandomIcon.png");
        NoteStartOffsetIcon = LoadSprite("NoteStartOffset.png");
        PlaylistIcon = LoadSprite("PlaylistIcon.png");
    }
}