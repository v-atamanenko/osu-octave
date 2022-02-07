#include "Welcome.h"

#include "Graphics/pText.h"

void TapToStartHandler(pDrawable* self, uint16_t x, uint16_t y) {
    ChangeModeOnFrameEnd(MODE_SONGSELECT);
}

Welcome::Welcome() {
    GraphicsManager::Graphics().LoadTexturesForMode(MODE_WELCOME);

    pSprite* bg = new pSprite(TX_WELCOME_BG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255);
    pSprite* sprite = new pSprite(TX_TAP_TO_START, 396, 422, 168, 10, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 0);
    sprite->Show(GameClock::Clock().Time(), GameClock::Clock().Time()+1000);
    bg->OnClick = TapToStartHandler;
    bg->Clickable = true;

    mSpriteManager.Add(bg);
    mSpriteManager.Add(sprite);
}

void Welcome::HandleInput() {}

void Welcome::Update()
{
    mSpriteManager.Draw();
}
