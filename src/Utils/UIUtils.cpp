#include "Utils/UIUtils.hpp"
#include "Utils/ArrayUtil.hpp" 
namespace UIUtils
{
    UnityEngine::UI::Button* CreateBaseButton(std::string_view name, UnityEngine::Transform* parent, std::string_view buttonTemplate)
    {
        Il2CppString* templCS = il2cpp_utils::newcsstr(buttonTemplate.data());
        auto btn = UnityEngine::Object::Instantiate(ArrayUtil::Last(UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::UI::Button*>(),  [&](UnityEngine::UI::Button* x) {
            return x->get_name()->Equals(templCS);
        }), parent, false);

        btn->set_name(il2cpp_utils::newcsstr(name.data()));
        btn->set_interactable(true);
        return btn;
    }

    UnityEngine::UI::Button* CreateIconButton(std::string_view name, UnityEngine::Transform* parent, std::string_view buttonTemplate, UnityEngine::Sprite* icon, std::string_view hint)
    {
        #warning not implemented
        return nullptr;
    }

    UnityEngine::UI::Button* CreateIconButton(std::string_view name, UnityEngine::Transform* parent, std::string_view buttonTemplate, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(void)> onClick, UnityEngine::Sprite* icon, std::string_view hint)
    {
        #warning not implemented
        return nullptr;
    }

    UnityEngine::UI::Button* CreatePageButton(std::string_view name, UnityEngine::Transform* parent, std::string_view buttonTemplate, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(void)> onClick, UnityEngine::Sprite* icon)
    {
        #warning not implemented
        return nullptr;
    }

    UnityEngine::UI::Button* CreateUIButton(std::string_view name, UnityEngine::Transform* parent, std::string_view buttonTemplate, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void(void)> onClick, std::string_view buttonText)
    {
        #warning not implemented
        return nullptr;
    }

    UnityEngine::RectTransform* CreateStatIcon(std::string_view name, UnityEngine::Transform* templ, UnityEngine::Transform* parent, UnityEngine::Sprite* icon, std::string_view hoverHintText)
    {
        #warning not implemented
        return nullptr;
    }

    TMPro::TextMeshProUGUI* CreateText(UnityEngine::Transform* parent, std::string_view text, float fontSize, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta)
    {
        #warning not implemented
        return nullptr;
    }

    void SetHoverHint(UnityEngine::RectTransform* button, std::string_view name, std::string_view text)
    {
        #warning not implemented
    }

    void DestroyHoverHint(UnityEngine::RectTransform* button)
    {
        #warning not implemented
    }

    void SetButtonTextColor(UnityEngine::UI::Button* button, UnityEngine::Color color)
    {
        #warning not implemented
    }

    void SetStatButtonText(UnityEngine::RectTransform* rect, std::string_view text)
    {
        #warning not implemented
    }

    void SetStatButtonIcon(UnityEngine::RectTransform* rect, UnityEngine::Sprite* icon)
    {
        #warning not implemented
    }

}