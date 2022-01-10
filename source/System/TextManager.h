#include <nds.h>
#include <stdio.h>
#include <stdarg.h>
#include <vector>

#include "Graphics/GraphicsManager.h"
#include "Libraries/gfxconsole.h"

#include "hoo06_bmf.h"
#include "gomics_bmf.h"
#include "370_bmf.h"
#include "ver08_bmf.h"

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
		static TextManager& Top() { return sTop; }
		static TextManager& Bottom() { return sBottom; }
		
		static void Init();
		
		static void Warn(char* format, ...);
		
		void SetFont(FONT font);
		
		void PrintFloat(char* format, ...);
		void Print(char* format, ...);
		void Clear();
		
		void PrintLocate(int x, int y, DrawOrigin origin, char* format, ...);
	
	protected:
		gfxPrintConsole* mConsole;
		int mFonts[NUMBER_OF_FONTS];
		
		static TextManager sTop, sBottom;
		
		static void AddFont(FONT font, const u8* data);
	
	private:
		TextManager();

};

#endif

