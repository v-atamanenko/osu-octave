#include "TextManager.h"
#include "SDL_ttf.h"
#include "defines.h"

TextManager TextManager::sTM;

TextManager::TextManager()
{
}

void TextManager::initTex()
{
    int w_w, w_h;
    uint32_t pf;

    SDL_GetWindowSize(window, &w_w, &w_h);
    pf = SDL_GetWindowPixelFormat(window);
    sTM.mConsole = SDL_CreateTexture(renderer, pf, SDL_TEXTUREACCESS_TARGET, w_w, w_h);
    SDL_SetTextureBlendMode(sTM.mConsole, SDL_BLENDMODE_BLEND);
    sTM.Clear();
}

void TextManager::drawTex()
{
    if (sTM.mConsole == nullptr) {
        sTM.initTex();
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, sTM.mConsole, nullptr, nullptr);
}

void TextManager::updateTex(const std::string &text, SDL_Color color, int x, int y)
{
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(sTM.mFonts[sTM.currentFont], text.c_str(), color);

    if (surfaceMessage == nullptr) {
        fprintf(stderr, "Failed to create surfaceMessage: %s\n", SDL_GetError());
    }

    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    if (Message == nullptr) {
        fprintf(stderr, "Failed to create Message texture: %s\n", SDL_GetError());
    }

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(Message, NULL, NULL, &texW, &texH);

    SDL_Rect Message_rect = {x,y,texW,texH};

    if (sTM.mConsole == nullptr) {
        sTM.initTex();
    }

    SDL_SetRenderTarget( renderer, sTM.mConsole );
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_RenderCopy(renderer, Message, nullptr, &Message_rect);
    SDL_SetRenderTarget( renderer, nullptr );

    // Don't forget to free your surface and texture
    SDL_DestroyTexture(Message);
    SDL_FreeSurface(surfaceMessage);
}

void TextManager::Init()
{
	//TODO: Use different fonts or remove rendundant font types
	AddFont(FONT_CONSOLE, "fonts/verdana.ttf");
	AddFont(FONT_SCORE, "fonts/verdana.ttf");
	AddFont(FONT_NUMBERING, "fonts/verdana.ttf");
	AddFont(FONT_VERDANA, "fonts/verdana.ttf");

    sTM.currentFont = FONT_VERDANA;
}

void TextManager::AddFont(FONT font, std::string path, int ptsize)
{
    sTM.mFonts[font] = TTF_OpenFont(path.c_str(), ptsize);
}

void TextManager::Warn(char* format, ...)
{
    char* message;
    va_list args;
    va_start(args, format);
    SDL_asprintf(&message, format, args);
    va_end(args);

    updateTex(message, SDL_Color({255, 25, 25, 255}), 0, 0);
}

void TextManager::SetFont(FONT font)
{
    currentFont = font;
}

void TextManager::Print(char* format, ...)
{
	char* message;
	va_list args;
	va_start(args, format);
	SDL_asprintf(&message, format, args);
	va_end(args);

    updateTex(message, SDL_Color(), 0, 0);
}

void TextManager::Clear()
{
    SDL_SetRenderTarget( renderer, mConsole );
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(renderer, NULL);
    SDL_SetRenderTarget( renderer, nullptr );
}

void TextManager::PrintLocate(int x, int y, DrawOrigin origin, char* format, ...)
{
    char* message;
	// convert from 640x480 (expected values) to 256x192 (internal values)
	x = (int)(x/2.5f);
	y = (int)(y/2.5f);
	
	va_list args;
	va_start(args, format);
	
	// if we're printing from top left, we don't need to know width and height
	if (origin == ORIGIN_TOPLEFT)
	{
        SDL_asprintf(&message, format, args);
		va_end(args);
		return;
	}
	
	// otherwise we need to know the width and height
	char* tmp;

    SDL_asprintf(&tmp, format, args);
    SDL_asprintf(&message, format, args);
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
	int height = lines.size() * 16; // FIXME: assuming font height here
	
	//width depends on the characters themselves, we use the longest line
	int width = 0;
	for (uint32_t i=0; i<lines.size(); i++)
	{
		int current = 0;
		tmp = lines[i];
		for (int j=0; tmp[j] != '\0'; j++)
		{
			current += 10; //FIXME: assuming font height here
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

    updateTex(message, SDL_Color({255, 255, 255, 255}), x, y);
}

