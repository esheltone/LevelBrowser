#include "Utils/UIUtils.hpp"
#include "Utils/ArrayUtil.hpp" 
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"

#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/UI/LayoutGroup.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/Events/UnityAction.hpp"

#include "HMUI/ImageView.hpp"

#include "Components/ButtonIconImage.hpp"

#include <map>

using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;

#include <array>
#include <algorithm>

std::map<std::string, Il2CppString*> stringToIl2CppString;

#define GET_STRING(string) \
Il2CppString* string## _cs; \
do \
{ \
    auto string## _itr = stringToIl2CppString.find(#string); \
    if (string## _itr != stringToIl2CppString.end()) { \
        string## _cs = string## _itr->second; \
    } else { \
        string## _cs = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>(#string); \
        stringToIl2CppString[#string] = string## _cs; \
    } \
} \
while (0)

#define MakeDelegate(DelegateType, varName) (il2cpp_utils::MakeDelegate<DelegateType>(classof(DelegateType), varName))

namespace UIUtils
{
    Button* CreateBaseButton(std::string_view name, Transform* parent, std::string_view buttonTemplate)
    {
        Il2CppString* templCS = il2cpp_utils::newcsstr(buttonTemplate.data());
        auto btn = Object::Instantiate(ArrayUtil::Last(Resources::FindObjectsOfTypeAll<Button*>(),  [&](Button* x) {
            return x->get_name()->Equals(templCS);
        }), parent, false);

        btn->set_name(il2cpp_utils::newcsstr(name.data()));
        btn->set_interactable(true);
        return btn;
    }

    Button* CreateIconButton(std::string_view name, Transform* parent, std::string_view buttonTemplate, Sprite* icon, std::string_view hint)
    {
        auto btn = CreateBaseButton(name, parent, buttonTemplate);

        SetHoverHint(btn->get_transform(), string_format("%s_hoverHintText", name.data()), hint);
        GET_STRING(Content);
        btn->get_gameObject()->AddComponent<QuestUI::ExternalComponents*>()->Add( ArrayUtil::First(btn->GetComponentsInChildren<LayoutGroup*>(), [&](auto x) -> bool { 
            return x->get_gameObject()->get_name()->Equals(Content_cs);
        }));

        GET_STRING(Text);
        GET_STRING(Icon);
        GET_STRING(Underline);

        Transform* contentTransform = btn->get_transform()->Find(Content_cs);
        Object::Destroy(contentTransform->Find(Text_cs)->get_gameObject());
        Image* iconImage = GameObject::New_ctor(Icon_cs)->AddComponent<HMUI::ImageView*>();
        // idk what mat that is
        //iconImage->set_material(BeatSaberMarkupLanguage.Utilities.ImageResources.NoGlowMat);
        iconImage->get_rectTransform()->SetParent(contentTransform, false);
        iconImage->get_rectTransform()->set_sizeDelta(Vector2(10.0f, 10.0f));
        iconImage->set_sprite(icon);
        iconImage->set_preserveAspect(true);

        if (iconImage)
        {
            auto btnIcon = btn->get_gameObject()->AddComponent<SongBrowser::Components::ButtonIconImage*>();
            btnIcon->image = iconImage;
        }

        Object::Destroy(btn->get_transform()->Find(Content_cs)->GetComponent<LayoutElement*>());
        ArrayUtil::First(btn->GetComponentsInChildren<RectTransform*>(), [&](auto x) -> bool {
            return x->get_name()->Equals(Underline_cs);
        })->get_gameObject()->SetActive(false);

        auto buttonSizeFitter = btn->get_gameObject()->AddComponent<ContentSizeFitter*>();
        buttonSizeFitter->set_verticalFit(ContentSizeFitter::FitMode::Unconstrained);
        buttonSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);

        btn->set_onClick(Button::ButtonClickedEvent::New_ctor());
        return btn;
    }

    Button* CreateIconButton(std::string_view name, Transform* parent, std::string_view buttonTemplate, Vector2 anchoredPosition, Vector2 sizeDelta, std::function<void(void)> onClick, Sprite* icon, std::string_view hint)
    {
        auto btn = CreateIconButton(name, parent, buttonTemplate, icon, hint);
        auto rect = reinterpret_cast<RectTransform*>(btn->get_transform());
        rect->set_anchorMin(Vector2(0.5f, 0.5f));
        rect->set_anchorMax(Vector2(0.5f, 0.5f));
        rect->set_anchoredPosition(anchoredPosition);
        rect->set_sizeDelta(sizeDelta);
        
        if (onClick)
            btn->get_onClick()->AddListener(MakeDelegate(UnityAction*, onClick));

        return btn;
    }

    Button* CreatePageButton(std::string_view name, Transform* parent, std::string_view buttonTemplate, Vector2 anchoredPosition, Vector2 sizeDelta, std::function<void(void)> onClick, Sprite* icon)
    {
        auto btn = CreateBaseButton(name, parent, buttonTemplate);

        auto rect = reinterpret_cast<RectTransform*>(btn->get_transform());
        rect->set_anchorMin(Vector2(0.5f, 0.5f));
        rect->set_anchorMax(Vector2(0.5f, 0.5f));
        rect->set_anchoredPosition(anchoredPosition);
        rect->set_sizeDelta(sizeDelta);
        rect->set_pivot(Vector2(0.5f, 0.5f));

        auto btnIcon = btn->get_gameObject()->AddComponent<SongBrowser::Components::ButtonIconImage*>();
        btnIcon->image = ArrayUtil::First(btn->get_gameObject()->GetComponentsInChildren<Image*>(true), [](auto x) -> bool { 
            GET_STRING(Icon); 
            return x->get_gameObject()->get_name()->Equals(Icon_cs); 
        });

        if (btnIcon->image) btnIcon->image->set_sprite(icon);

        btn->set_onClick(Button::ButtonClickedEvent::New_ctor());

        if (onClick)
            btn->get_onClick()->AddListener(MakeDelegate(UnityAction*, onClick));

        return btn;
    }

    Button* CreateUIButton(std::string_view name, Transform* parent, std::string_view buttonTemplate, Vector2 anchoredPosition, Vector2 sizeDelta, std::function<void(void)> onClick, std::string_view buttonText)
    {
        #warning not implemented
        return nullptr;
    }

    Transform* CreateStatIcon(std::string_view name, Transform* templ, Transform* parent, Sprite* icon, std::string_view hoverHintText)
    {
        #warning not implemented
        return nullptr;
    }

    TMPro::TextMeshProUGUI* CreateText(Transform* parent, std::string_view text, float fontSize, Vector2 anchoredPosition, Vector2 sizeDelta)
    {
        #warning not implemented
        return nullptr;
    }

    void SetHoverHint(Transform* button, std::string_view name, std::string_view text)
    {
        auto hover = button->get_gameObject()->AddComponent<HoverHint*>();
        hover->set_text(il2cpp_utils::newcsstr(text.data()));
        hover->set_name(il2cpp_utils::newcsstr(name.data()));
        hover->hoverHintController = ArrayUtil::First(Resources::FindObjectsOfTypeAll<HoverHintController*>());
    }

    void DestroyHoverHint(Transform* button)
    {
        auto hint = button->get_gameObject()->GetComponentInChildren<HoverHint*>();
        if (hint) Object::DestroyImmediate(hint);
    }

    void SetButtonTextColor(Button* button, Color color)
    {
        #warning not implemented
    }

    void SetStatButtonText(Transform* rect, std::string_view text)
    {
        #warning not implemented
    }

    void SetStatButtonIcon(Transform* rect, Sprite* icon)
    {
        #warning not implemented
    }

    void SetButtonText(UnityEngine::UI::Button* button, std::string_view _text)
    {
        #warning not implemented
    }

    void SetButtonTextSize(UnityEngine::UI::Button* button, float _fontSize)
    {
        #warning not implemented
    }

    void ToggleWordWrapping(UnityEngine::UI::Button* button, bool enableWordWrapping)
    {
        #warning not implemented
    }

    void SetButtonBackgroundActive(UnityEngine::UI::Button* button, bool active)
    {
        #warning not implemented
    }

    void SetButtonUnderlineColor(UnityEngine::UI::Button* button, UnityEngine::Color color)
    {
        #warning not implemented
    }

    void SetButtonBorder(UnityEngine::UI::Button* button, UnityEngine::Color color)
    {
        #warning not implemented
    }

}