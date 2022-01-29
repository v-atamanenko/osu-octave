#include <cstdio>
#include <cstdarg>
#include <vector>
#include "SDL_ttf.h"

#include "Graphics/GraphicsManager.h"

//#include "hoo06_bmf.h"
//#include "gomics_bmf.h"
//#include "370_bmf.h"
//#include "ver08_bmf.h"

#ifndef __TEXTMANAGER_H__
#define __TEXTMANAGER_H__

#define NUMBER_OF_FONTS 4

typedef enum {
	FONT_CONSOLE,
	FONT_SCORE,
	FONT_NUMBERING,
	FONT_VERDANA
} FONT;

class TextManager
{
	public:
        //TODO: No top/bottom anymore
		static TextManager& Top() { return sTM; }
		static TextManager& Bottom() { return sTM; }
		
		static void Init();
		
		static void Warn(char* format, ...);
		
		void SetFont(FONT font);

        void drawTex();
		void Print(char* format, ...);
		void Clear();
		
		void PrintLocate(int x, int y, DrawOrigin origin, char* format, ...);
	
	protected:
		//gfxPrintConsole* mConsole;
        TTF_Font* mFonts[NUMBER_OF_FONTS];
        FONT currentFont;
        SDL_Texture* mConsole;

		static TextManager sTM;

		static void AddFont(FONT font, std::string path, int ptsize=18);
        static void updateTex(const std::string &text, SDL_Color color, int x, int y);
        static void initTex();
	private:
		TextManager();

};

#endif

