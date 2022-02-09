#include <cstring>
#include <string>
#include <cstdio>
#include <map>

#include <SDL.h>
#include "SDL_image.h"

#include "GfxInfo.h"

#include "defines.h"
#include "Modes/Mode.h"

#ifndef __GRAPHICSMANAGER_H__
#define __GRAPHICSMANAGER_H__

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

		void Draw(TextureType tex, int32_t x, int32_t y, uint32_t width, uint32_t height, DrawOrigin origin, FieldType fieldtype, SDL_Color color, uint32_t alpha, int32_t angle, float z = 0, const SDL_Rect* uv = nullptr);

        void CreateTextureFromSurface(SDL_Surface* bg, TextureType texid);
        void LoadBeatmapBackground(const std::string& path);
        void DrawBeatmapBackground();

        void LoadTexturesForMode(ModeType mod);
        void UnloadTextures();

        void DrawFullScreenRectangle(SDL_Color c);

        static void Clear();
        static void Present();

        static const uint32_t PlayXOffset = ((SCREEN_WIDTH-640)/2);
		static const uint32_t PlayYOffset = 73;
	
	protected:
		SDL_Texture* textures[NUMBER_OF_TEXTURES];
        std::map<TextureType, SDL_Texture*> maptextures;

        bool LoadTexture(TextureType texid, const std::string& path);

        static int32_t ForceBounds(int32_t value);
		static GraphicsManager sGraphicsManager;
	
	private:
		GraphicsManager() = default;
		~GraphicsManager() = default;
};

#endif // __GRAPHICSMANAGER_H__
