#include "Utils/SpriteUtils.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include <string_view>
#include <string>
#include <fstream>

#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/TextureWrapMode.hpp"
#include "UnityEngine/ImageConversion.hpp"

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
    
    UnityEngine::Sprite* FileToSprite(std::string_view path)
    {
        std::ifstream instream(std::string(path.data()), std::ios::in | std::ios::binary);
        std::vector<uint8_t> data((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());
        Array<uint8_t>* bytes = il2cpp_utils::vectorToArray(data);
        UnityEngine::Texture2D* texture = UnityEngine::Texture2D::New_ctor(0, 0, UnityEngine::TextureFormat::RGBA32, false, false);
        if (UnityEngine::ImageConversion::LoadImage(texture, bytes, false)) {
            texture->set_wrapMode(UnityEngine::TextureWrapMode::Clamp);
            return UnityEngine::Sprite::Create(texture, UnityEngine::Rect(0.0f, 0.0f, (float)texture->get_width(), (float)texture->get_height()), UnityEngine::Vector2(0.5f,0.5f), 100.0f, 1u, UnityEngine::SpriteMeshType::Tight, UnityEngine::Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
        }
        return nullptr;
    }

    UnityEngine::Sprite* LoadSprite(std::string_view image)
    {
        std::string path = string_format("/sdcard/ModData/com.beatgames.beatsaber/Mods/SongBrowser/Icons/%s", image.data());
        return FileToSprite(path);
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