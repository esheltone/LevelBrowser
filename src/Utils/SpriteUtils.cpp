#include "Utils/SpriteUtils.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include <string_view>
#include <string>
#include <fstream>

#include "UnityEngine/Object.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/TextureWrapMode.hpp"
#include "UnityEngine/ImageConversion.hpp"

namespace SpriteUtils
{
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

    UnityEngine::Sprite* get_StarFullIcon()
    {
        return LoadSprite("StarFull.png");
    }

    UnityEngine::Sprite* get_SpeedIcon()
    {
        return LoadSprite("Speed.png");
    }

    UnityEngine::Sprite* get_GraphIcon()
    {
        return LoadSprite("Graph.png");
    }

    UnityEngine::Sprite* get_DeleteIcon()
    {
        return LoadSprite("DeleteIcon.png");
    }

    UnityEngine::Sprite* get_XIcon()
    {
        return LoadSprite("X.png");
    }

    UnityEngine::Sprite* get_DoubleArrow()
    {
        return LoadSprite("DoubleArrow.png");
    }

    UnityEngine::Sprite* get_RandomIcon()
    {
        return LoadSprite("RandomIcon.png");
    }

    UnityEngine::Sprite* get_NoteStartOffsetIcon()
    {
        return LoadSprite("NoteStartOffset.png");
    }

    UnityEngine::Sprite* get_PlaylistIcon()
    {
        return LoadSprite("PlaylistIcon.png");
    }
}