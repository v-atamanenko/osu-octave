#pragma once

#include <string>

#include "pDrawable.h"
#include "System/TextManager.h"
#include <utility>

class pText : public pDrawable {
public:
    std::string Text;
    FontName Font;
    SDL_Color Color{};

    pText(const std::string& text, FontName font, OOInt x, OOInt y, SDL_Color clr={255, 255, 255, 255});
    void Draw() override;
};
