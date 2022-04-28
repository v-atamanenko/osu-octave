#pragma once

#include "UIElements/UIElement.h"
#include "Graphics/SpriteManager.h"

class UIElementsManager {
public:
    UIElementsManager(SpriteManager* sprite_manager);
    ~UIElementsManager();

    void Update();
    void Clear();
    void HandleInput();

    void CreateLogo(OOInt x, OOInt y, bool start_animation = true);
    void CreateValueSlider(OOInt x, OOInt y, const std::string& label, const std::string &setting_name,
                           bool multiplier_mode=false);
    void CreateStringSelector(OOInt x, OOInt y, const std::string& label, const std::string &setting_name);
    void CreateRadioButton(OOInt x, OOInt y, const std::string& label, const std::string &setting_name,
                           const std::string &false_value_label, const std::string &true_value_label);
    void CreateTernaryButton(OOInt x, OOInt y, const std::string& label, const std::string &setting_name,
                             TextureType bg1, TextureType bg2, TextureType bg3);

protected:
    SpriteManager* mSpriteManager;
    std::vector<UIElement *> mUIElements;
};

