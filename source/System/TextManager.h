#pragma once

#include <cstdio>
#include <cstdarg>
#include <vector>

#include "SDL_ttf.h"
#include "SDL_FontCache.h"

#include "DataStorage/Settings.h"
#include "DataStorage/Skins.h"

#include "types.h"
#include "defines.h"

#define NUMBER_OF_FONTS 9

typedef struct InMemoryFont {
    void * buf;
    OOInt size;
} InMemoryFont;

class TextManager
{
	public:
        static void Init();
        static void InitDeferred();
        static void FullReload();
        static void SetFont(FontName font);

        static void PrintLocateColor(OOInt x, OOInt y, DrawOrigin origin, SDL_Color clr, const char *fmt, ...);

	protected:
		static FC_Font* mFonts[NUMBER_OF_FONTS];
        static std::map<std::string, InMemoryFont> mTTF;
        static FontName currentFont;

		static void AddFont(FontName font, const std::string& path, OOInt ptsize=18, SDL_Color c={0, 0, 0, 255});

	private:
		TextManager() = default;
};
