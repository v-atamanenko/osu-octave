#include <cstdio>
#include <cstdarg>
#include <vector>
#include "SDL_ttf.h"
#include "SDL_FontCache.h"

#include "Graphics/GraphicsManager.h"

#ifndef __TEXTMANAGER_H__
#define __TEXTMANAGER_H__

#define NUMBER_OF_FONTS 9

typedef enum {
	FONT_CONSOLE,
	FONT_SCORE,
	FONT_NUMBERING,
	FONT_VERDANA,
    FONT_PIXEL,
    FONT_PIXEL_ACTIVE,
    FONT_CONSOLE_BIG,
    FONT_CONSOLE_BIG_BOLD,
    FONT_CONSOLE_BOLD
} FontName;

class TextManager
{
	public:
        static void Init();
        static void SetFont(FontName font);

        static void Print(float x, float y, const char *fmt, ...);
        static void PrintLocate(float x, float y, DrawOrigin origin, const char *fmt, ...);
	
	protected:
		static FC_Font* mFonts[NUMBER_OF_FONTS];
        static FontName currentFont;

		static void AddFont(FontName font, const std::string& path, int ptsize=18, SDL_Color c={0, 0, 0, 255});

	private:
		TextManager() = default;
};

#endif
