#include "pText.h"

#include <utility>

pText::pText(std::string text, FontName font, uint16_t x, uint16_t y, SDL_Color clr) {
    this->X = x;
    this->Y = y;
    this->Z = -0.02f;
    this->Text = std::move(text);
    this->Font = font;
    this->Clickable = false;
    this->mAlive = true;
    this->Alpha = 255;
    this->Width = 1;
    this->Height = 1;
    this->Color = clr;
    this->Origin = ORIGIN_TOPLEFT;
    UV = nullptr;
}

void pText::Draw() {
    TextManager::SetFont(this->Font);

    TextManager::PrintLocate(
            (float)this->X, (float)this->Y, this->Origin,
            "%s", this->Text.c_str()
	);
}