#ifndef _MODEABOUT_H
#define _MODEABOUT_H

#include "DataStorage/Settings.h"
#include "Graphics/pText.h"
#include "Graphics/SpriteManager.h"
#include "System/TextManager.h"
#include "Graphics/SpriteContainer.h"

class ModeAbout : public Mode {
public:
    ModeAbout();
    void Update() override;

protected:
    void HandleInput() override {};
    SpriteManager mSpriteManager;
};

#endif //_MODEABOUT_H
