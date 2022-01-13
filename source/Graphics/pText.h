#include <string>
#include <nds.h>

#include "pDrawable.h"
#include "System/TextManager.h"

#ifndef __PTEXT_H__
#define __PTEXT_H__

class pText : public pDrawable {
public:
    std::string Text;
    FONT Font;

    pText(std::string text, FONT font, u16 x, u16 y);

    void Draw() override;
};

#endif