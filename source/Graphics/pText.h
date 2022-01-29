#include <string>

#include "pDrawable.h"
#include "System/TextManager.h"

#ifndef __PTEXT_H__
#define __PTEXT_H__

class pText : public pDrawable {
public:
    std::string Text;
    FONT Font;

    pText(std::string text, FONT font, uint16_t x, uint16_t y);

    void Draw() override;
};

#endif