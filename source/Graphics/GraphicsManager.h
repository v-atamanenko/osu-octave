#include <cstring>
#include <string>
#include <cstdio>

#include <SDL.h>
#include "SDL_image.h"

#include "GfxInfo.h"

#include "defines.h"
#include "Modes/Mode.h"

#ifndef __GRAPHICSMANAGER_H__
#define __GRAPHICSMANAGER_H__

typedef struct {
	int palette;
	uint8_t format;
	const uint32_t* uv;
} TextureInfo;

typedef enum {
	ORIGIN_CENTER,
	ORIGIN_TOPLEFT,
	ORIGIN_BOTTOMLEFT,
	ORIGIN_TOPRIGHT
} DrawOrigin;

typedef enum {
	FIELD_SCREEN,
	FIELD_PLAY
} FieldType;

class GraphicsManager
{
	public:
		static GraphicsManager& Graphics() { return sGraphicsManager; }

		void Draw(TextureType tex, int32_t x, int32_t y, uint32_t width, uint32_t height, DrawOrigin origin, FieldType fieldtype, SDL_Color color, uint32_t alpha, int32_t angle, float z = 0, const uint32_t* uv = nullptr);

        void loadBgFromSurface(SDL_Surface* bg );
        void bgDraw();

        void loadTexturesForMode(ModeType mod);
        void setBgByMode(ModeType mod);

        void DrawFullScreenRectangle(SDL_Color c);

        void clear();
        void present();

        static const uint32_t PlayXOffset = 64;
		static const uint32_t PlayYOffset = 73;
	
	protected:
		int32_t ForceBounds(int32_t value);
		
		TextureInfo textureInfo[NUMBER_OF_TEXTURES];
		SDL_Texture* textures[NUMBER_OF_TEXTURES];

        SDL_Texture * loadTexture(int texid, const std::string& path);
		
		static GraphicsManager sGraphicsManager;
	
	private:
		GraphicsManager();
		~GraphicsManager() {}
};

#endif

