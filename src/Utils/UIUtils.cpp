#include "Utils/UIUtils.hpp"
#include "Utils/ArrayUtil.hpp" 
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"

#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/UI/LayoutGroup.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/Events/UnityAction.hpp"

#include "HMUI/ImageView.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/HoverHintController.hpp"

#include "Polyglot/LocalizedTextMeshProUGUI.hpp"

#include "Components/ButtonIconImage.hpp"

#include <map>

#include "logging.hpp"

#include "custom-types/shared/delegate.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace TMPro;
using namespace HMUI;

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

#define MakeDelegate(DelegateType, varName) (custom_types::MakeDelegate<DelegateType>(classof(DelegateType), varName))

namespace UIUtils
{
    Button* CreateBaseButton(std::string_view name, Transform* parent, std::string_view buttonTemplate)
    {
        Il2CppString* templCS = il2cpp_utils::newcsstr(buttonTemplate.data());
        auto btn = Object::Instantiate(Resources::FindObjectsOfTypeAll<Button*>().Last([&](Button* x) {
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
        btn->get_gameObject()->AddComponent<QuestUI::ExternalComponents*>()->Add( btn->GetComponentsInChildren<LayoutGroup*>().First([&](auto x) -> bool {
            return x->get_gameObject()->get_name()->Equals(Content_cs);
        }));

        GET_STRING(Text);
        GET_STRING(Icon);
        GET_STRING(Underline);

        Transform* contentTransform = btn->get_transform()->Find(Content_cs);
        Object::Destroy(contentTransform->Find(Text_cs)->get_gameObject());
        Image* iconImage = GameObject::New_ctor(Icon_cs)->AddComponent<HMUI::ImageView*>();
        // idk what mat that is

        Il2CppString* templCS = il2cpp_utils::newcsstr(buttonTemplate.data());
        auto orig = Resources::FindObjectsOfTypeAll<Button*>().Last([&](Button* x) {
            return x->get_name()->Equals(templCS);
        });

        iconImage->set_material(orig->get_gameObject()->GetComponentInChildren<HMUI::ImageView*>()->get_material());
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
        btn->GetComponentsInChildren<RectTransform*>().First([&](auto x) -> bool {
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
        btnIcon->image = btn->get_gameObject()->GetComponentsInChildren<Image*>(true).First([](auto x) -> bool {
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
        auto btn = CreateBaseButton(name, parent, buttonTemplate);
        btn->get_gameObject()->SetActive(true);

        auto localizer = btn->get_gameObject()->GetComponentInChildren<Polyglot::LocalizedTextMeshProUGUI*>();
        if (localizer)
            Object::Destroy(localizer);
        
        auto externalComponents = btn->get_gameObject()->AddComponent<QuestUI::ExternalComponents*>();
        auto textMesh = btn->get_gameObject()->GetComponentInChildren<TextMeshProUGUI*>();
        textMesh->set_richText(true);
        externalComponents->Add(textMesh);

        GET_STRING(Content);

        auto contentTransform = btn->get_transform()->Find(Content_cs)->GetComponent<LayoutElement*>();
        if (contentTransform)
            Object::Destroy(contentTransform);

        auto buttonSizeFitter = btn->get_gameObject()->AddComponent<ContentSizeFitter*>();
        buttonSizeFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
        buttonSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);

        auto stackLayoutGroup = btn->get_gameObject()->GetComponentInChildren<LayoutGroup*>();
        if (stackLayoutGroup)
            externalComponents->Add(stackLayoutGroup);

        if (onClick)
            btn->get_onClick()->AddListener(MakeDelegate(UnityAction*, onClick));

        auto rect = reinterpret_cast<RectTransform*>(btn->get_transform());
        rect->set_anchorMin(Vector2(0.5f, 0.5f));
        rect->set_anchorMax(Vector2(0.5f, 0.5f));
        rect->set_anchoredPosition(anchoredPosition);
        rect->set_sizeDelta(sizeDelta);

        SetButtonText(btn, buttonText);

        return btn;
    }

    Transform* CreateStatIcon(std::string_view name, Transform* templ, Transform* parent, Sprite* icon, std::string_view hoverHintText)
    {
        auto statIcon = Object::Instantiate(templ, parent, false);
        statIcon->set_name(il2cpp_utils::newcsstr(name));
        reinterpret_cast<RectTransform*>(statIcon->get_transform())->Translate(0.0f, -0.1f, 0.0f);
        SetStatButtonIcon(statIcon, icon);
        DestroyHoverHint(statIcon);

        if (hoverHintText.size() > 1)
        {
            SetHoverHint(statIcon, string_format("%s_hoverHintText", hoverHintText.data()), hoverHintText);
        }

        return statIcon;
    }

    TMPro::TextMeshProUGUI* CreateText(Transform* parent, std::string_view text, float fontSize, Vector2 anchoredPosition, Vector2 sizeDelta)
    {
        GET_STRING(CustomUIText);
        auto gameObj = GameObject::New_ctor(CustomUIText_cs);
        gameObj->SetActive(false);

        auto textMesh = gameObj->AddComponent<CurvedTextMeshPro*>();
        static auto font = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Teko-Medium SDF");
        auto orig = Resources::FindObjectsOfTypeAll<TMP_FontAsset*>().First([&](auto t) {
            if (!t || !t->get_name()) return false;
            return t->get_name()->Equals(font);
        });

        textMesh->set_font(orig);

        auto rect = textMesh->get_rectTransform();
        rect->SetParent(parent, false);
        textMesh->set_text(il2cpp_utils::newcsstr(text));
        textMesh->set_fontSize(fontSize);
        textMesh->set_color({1.0f, 1.0f, 1.0f, 1.0f});

        rect->set_anchorMin(Vector2(0.5f, 0.5f));
        rect->set_anchorMax(Vector2(0.5f, 0.5f));
        rect->set_sizeDelta(sizeDelta);
        rect->set_anchoredPosition(anchoredPosition);

        gameObj->SetActive(true);
        return textMesh;
    }

    void SetHoverHint(Transform* button, std::string_view name, std::string_view text)
    {
        auto hover = button->get_gameObject()->AddComponent<HoverHint*>();
        hover->set_text(il2cpp_utils::newcsstr(text.data()));
        hover->set_name(il2cpp_utils::newcsstr(name.data()));
        hover->dyn__hoverHintController() = Resources::FindObjectsOfTypeAll<HoverHintController*>().First();
    }

    void DestroyHoverHint(Transform* button)
    {
        auto hint = button->get_gameObject()->GetComponentInChildren<HoverHint*>();
        if (hint) Object::DestroyImmediate(hint);
    }

    void SetButtonTextColor(Button* button, Color color)
    {
        auto txt = button->get_gameObject()->GetComponentsInChildren<TextMeshProUGUI*>().First([](auto x) {
            GET_STRING(Text); 
            return x->get_gameObject()->get_name()->Equals(Text_cs); 
        });
        if (txt) txt->set_color(color);
    }

    void SetStatButtonText(Transform* transform, std::string_view text)
    {
        auto txt = transform->get_gameObject()->GetComponentsInChildren<TextMeshProUGUI*>().First([](auto x) {
            GET_STRING(ValueText); 
            return x->get_gameObject()->get_name()->Equals(ValueText_cs); 
        });
        if (txt) txt->set_text(il2cpp_utils::newcsstr(text));
    }

    void SetStatButtonIcon(Transform* transform, Sprite* icon)
    {
        auto img = transform->get_gameObject()->GetComponentsInChildren<Image*>().First([](auto x) {
            GET_STRING(Icon); 
            return x->get_gameObject()->get_name()->Equals(Icon_cs); 
        });
        if (img)
        {
            img->set_sprite(icon);
            img->set_color({1.0f, 1.0f, 1.0f, 1.0f});
        }
    }

    void SetButtonText(UnityEngine::UI::Button* button, std::string_view text)
    {
        auto textMesh = button->GetComponentInChildren<CurvedTextMeshPro*>();
        if (textMesh) textMesh->SetText(il2cpp_utils::newcsstr(text));
    }

    void SetButtonTextSize(UnityEngine::UI::Button* button, float fontSize)
    {
        auto textMesh = button->GetComponentInChildren<CurvedTextMeshPro*>();
        if (textMesh) textMesh->set_fontSize(fontSize);
    }

    void ToggleWordWrapping(UnityEngine::UI::Button* button, bool enableWordWrapping)
    {
        auto textMesh = button->GetComponentInChildren<CurvedTextMeshPro*>();
        if (textMesh) textMesh->set_enableWordWrapping(enableWordWrapping);
    }

    void SetButtonBackgroundActive(UnityEngine::UI::Button* button, bool active)
    {
        auto img = button->get_gameObject()->GetComponentsInChildren<ImageView*>(true).Last([](auto x) {
            GET_STRING(BG);
            return x->get_gameObject()->get_name()->Equals(BG_cs);
        });
        if (img) img->get_gameObject()->SetActive(active);
    }

    void SetButtonUnderlineColor(UnityEngine::UI::Button* button, UnityEngine::Color color)
    {
        auto img = button->get_gameObject()->GetComponentsInChildren<ImageView*>().First([](auto x) {
            GET_STRING(Underline);
            return x->get_gameObject()->get_name()->Equals(Underline_cs);
        });
        if (img) img->set_color(color);
    }

    void SetButtonBorder(UnityEngine::UI::Button* button, UnityEngine::Color color)
    {
        auto img = button->get_gameObject()->GetComponentsInChildren<ImageView*>().First([](auto x) {
            GET_STRING(Border);
            return x->get_gameObject()->get_name()->Equals(Border_cs);
        });
        if (img)
        {
            img->set_color0(color);
            img->set_color1(color);
            img->set_color(color);
            img->set_fillMethod(Image::FillMethod::Horizontal);
            img->SetAllDirty();
        }
    }
}