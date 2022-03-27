#ifndef _MODEABOUT_H
#define _MODEABOUT_H

#include "DataStorage/Settings.h"
#include "Graphics/pText.h"
#include "Graphics/SpriteManager.h"
#include "System/TextManager.h"
#include "Graphics/SpriteContainer.h"
#include "UIElements/Logo.h"

class ModeAbout : public Mode {
public:
    ModeAbout();
    ~ModeAbout() override { delete mLogo; };
    void Update() override;

protected:
    void HandleInput() override {};
    Logo* mLogo;
    SpriteManager mSpriteManager;
};

#endif //_MODEABOUT_H
