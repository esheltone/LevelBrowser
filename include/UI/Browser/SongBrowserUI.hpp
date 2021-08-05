#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "HMUI/ViewController.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"

#include "DataAccess/SongBrowserModel.hpp"

DECLARE_CLASS_CODEGEN(SongBrowser::UI, SongBrowserUI, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(SongBrowser::SongBrowserModel*, model);

    void Show();
    void Hide();
    void UpdateLevelDataModel();
    void UpdateLevelCollectionSelection();
    void RefreshSongList();
    void CreateUI(GlobalNamespace::MainMenuViewController::MenuButton mode);
)

DECLARE_CLASS_CODEGEN(SongBrowser::UI, SongBrowserViewController, HMUI::ViewController,

    /*named instance (no specific impl)*/
    
)
