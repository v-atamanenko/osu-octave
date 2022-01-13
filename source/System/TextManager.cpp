#include "TextManager.h"

TextManager TextManager::sTop;
TextManager TextManager::sBottom;

TextManager::TextManager()
{
}

void TextManager::Init()
{
	sBottom.mConsole = (gfxPrintConsole*)malloc(sizeof(gfxPrintConsole));
	sTop.mConsole = (gfxPrintConsole*)malloc(sizeof(gfxPrintConsole));
	
	gfxConsoleInit(sBottom.mConsole, BgType_Bmp16, BgSize_B16_256x256, true, true);
	gfxConsoleInit(sTop.mConsole, BgType_Bmp16, BgSize_B16_256x256, false, true);
	
	AddFont(FONT_CONSOLE, hoo06_bmf);
	AddFont(FONT_SCORE, gomics_bmf);
	AddFont(FONT_NUMBERING, _370_bmf);
	AddFont(FONT_VERDANA, ver08_bmf);
}

void TextManager::AddFont(FONT font, const u8* data)
{
	BMFont* bmf = (BMFont*)malloc(sizeof(BMFont));
	LoadBMFont(bmf, data);
	sBottom.mFonts[font] = gfxConsoleAddFont(sBottom.mConsole, bmf);
	sTop.mFonts[font] = gfxConsoleAddFont(sTop.mConsole, bmf);
}

void TextManager::Warn(char* format, ...)
{
	gfxConsoleSelect(sTop.mConsole);
	iprintf("\x1b[31;1;47;7;1m\n");
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	iprintf("\n\x1b[0m");
}

void TextManager::SetFont(FONT font)
{
	gfxConsoleSetFont(mConsole, mFonts[font]);
}

void TextManager::PrintFloat(char* format, ...)
{
	gfxConsoleSelect(mConsole);
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

void TextManager::Print(char* format, ...)
{
	gfxConsoleSelect(mConsole);
	va_list args;
	va_start(args, format);
	viprintf(format, args);
	va_end(args);
}

void TextManager::Clear()
{
	gfxConsoleSelect(mConsole);
	gfxConsoleClear();
}

void TextManager::PrintLocate(int x, int y, DrawOrigin origin, char* format, ...)
{	
	gfxConsoleSelect(mConsole);
	
	// convert from 640x480 (expected values) to 256x192 (internal values)
	x = (int)(x/2.5f);
	y = (int)(y/2.5f);
	
	va_list args;
	va_start(args, format);
	
	// if we're printing from top left, we don't need to know width and height
	if (origin == ORIGIN_TOPLEFT)
	{
		iprintf("\x1b[%i;%iX", y, x);
		viprintf(format, args);
		va_end(args);
		return;
	}
	
	// otherwise we need to know the width and height
	char* tmp;
	
	vasiprintf(&tmp, format, args);
	va_end(args);
	
	// for calculating width
	std::vector<char*> lines;
	lines.reserve(10);
	
	// height depends on the number of new lines
	char* pos = strchr(tmp, '\n');
	while (pos != NULL)
	{
		*pos = '\0';
		lines.push_back(tmp);
		
		tmp = pos+1;
		pos = strchr(tmp, '\n');
	}
	lines.push_back(tmp);
	
	// multiply by line height for pixel value
	int height = lines.size() * mConsole->font[mConsole->curFont]->Head->bLineHeight;
	
	//width depends on the characters themselves, we use the longest line
	int width = 0;
	for (u32 i=0; i<lines.size(); i++)
	{
		int current = 0;
		tmp = lines[i];
		for (int j=0; tmp[j] != '\0'; j++)
		{
			current += mConsole->font[mConsole->curFont]->Ltrs[tmp[j]]->bShift;
		}
		
		if (current > width)
			width = current;
	}
	
	// now that we have x,y,width,height we can calculate the actual location
	switch (origin)
	{
		case ORIGIN_TOPLEFT:
			break; // special case  - would not reach this point
		
		case ORIGIN_CENTER:
			x -= width >> 1;
			y -= height >> 1;
			break;
		
		case ORIGIN_BOTTOMLEFT:
			y -= height;
			break;
		
		case ORIGIN_TOPRIGHT:
			x -= width;
			break;
	}
	
	// move cursor y pos
	iprintf("\x1b[%iy", y);
	
	// dump information with x pos
	for (int i=0; i<lines.size(); i++)
		if (x > 0 && x + width < 640) 
			iprintf("\x1b[%ix%s\n", x, lines[i]);
}

