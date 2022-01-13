#include "pText.h"

pText::pText(std::string text, FONT font, u16 x, u16 y) {
    this->X = x;
    this->Y = y;
    this->Text = text;
    this->Font = font;
    this->Clickable = false;
    this->mAlive = true;
    this->Alpha = 100;
    this->Width = 1;
    this->Height = 1;
    UV = NULL;
}

void pText::Draw() {
    TextManager::Bottom().SetFont(this->Font);
			
	TextManager::Bottom().PrintLocate(
		this->X + 2, this->Y, this->Origin,
		(char*) this->Text.c_str()
	);

    nocashMessage(this->Text.c_str());
}