#include "pText.h"

#include <utility>

pText::pText(std::string text, FONT font, uint16_t x, uint16_t y, SDL_Color clr) {
    this->X = x;
    this->Y = y;
    this->Text = std::move(text);
    this->Font = font;
    this->Clickable = false;
    this->mAlive = true;
    this->Alpha = 255;
    this->Width = 1;
    this->Height = 1;
    this->Color = clr;
    UV = nullptr;
}

void pText::Draw() {
    TextManager::Bottom().SetFont(this->Font);

	TextManager::Bottom().PrintLocate(
		this->X, this->Y, this->Origin,
        this->Color, this->Text.c_str()
	);
}