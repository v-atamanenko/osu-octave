#include "Welcome.h"

#include "Graphics/pText.h"

void TapToStartHandler(pDrawable* self, uint16_t x, uint16_t y) {
    ChangeMode(MODE_SONGSELECT);
}

Welcome::Welcome() {
    GraphicsManager::Graphics().setBgByMode(MODE_WELCOME);
    GraphicsManager::Graphics().loadTexturesForMode(MODE_WELCOME);

    pSprite* bg = new pSprite(TX_WELCOME_BG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255);
    pSprite* sprite = new pSprite(TX_TAP_TO_START, 396, 422, 168, 10, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255);

    bg->OnClick = TapToStartHandler;
    bg->Clickable = true;

    mSpriteManager.Add(bg);
    mSpriteManager.Add(sprite);
}

Welcome::~Welcome() {}

void Welcome::HandleInput() {}

void Welcome::Update()
{
    mSpriteManager.Draw();
}
