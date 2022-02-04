#include "pText.h"

pText::pText(std::string text, FONT font, uint16_t x, uint16_t y, SDL_Color clr) {
    this->X = x;
    this->Y = y;
    this->Z = 2.f;
    this->Text = text;
    this->Font = font;
    this->Clickable = false;
    this->mAlive = true;
    this->Alpha = 100;
    this->Width = 1;
    this->Height = 1;
    this->Color = clr;
    UV = NULL;
}

void pText::Draw() {
    TextManager::Bottom().SetFont(this->Font);

	TextManager::Bottom().PrintLocate(
		this->X + 2, this->Y, this->Origin,
        this->Color, (char*) this->Text.c_str()
	);
}