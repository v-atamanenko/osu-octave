#pragma once

#include "Graphics/SpriteManager.h"
#include "UIElements/UIElementsManager.h"

typedef enum SettingsTab {
    SETTINGS_TAB_UNSET = 0,
    SETTINGS_TAB_GENERAL = 1,
    SETTINGS_TAB_GAMEPLAY = 2,
    SETTINGS_TAB_SOUND = 3,
    SETTINGS_TAB_MAX = SETTINGS_TAB_SOUND+1
} SettingsTab;

class ModeSettings : public Mode {
public:
    ModeSettings();
    ~ModeSettings() override;

    void Update() override;

    static SettingsTab SwitchTabTo;

protected:
    void InitCommonSprites(int use_bg=0);
    void InitTabs(SettingsTab active_tab);
    void Clear();

    void HandleInput() override;

    void TabGeneral();
    void TabGameplay();
    void TabSound();

    SpriteManager mSpriteManager;
    UIElementsManager mUIElementsManager = UIElementsManager(&mSpriteManager);
};
