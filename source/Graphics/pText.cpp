#include "pText.h"

pText::pText(const std::string& text, FontName font, OOInt x, OOInt y, SDL_Color clr) {
    this->X = x;
    this->Y = y;
    this->Z = -0.02f;
    this->Text = text;
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
    TextManager::PrintLocate(this->X, this->Y, this->Origin,"%s", this->Text.c_str());
}
