#include <cstring>
#include <string>
#include <cstdio>
#include <map>
#include <shared_mutex>

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
        void LoadBeatmapPicTexture(TextureType texid, SDL_Surface *tex);
        void LoadBeatmapBackground(const std::string& path);
        void DrawBeatmapBackground();

        static SDL_Texture* LoadSquareTexture(const std::string& path);
        void LoadTexturesForMode(ModeType mod);
        void UnloadTextures();
        SDL_Texture * GetTexture(TextureType texid);

        void DrawFullScreenRectangle(SDL_Color c);

        static void Clear();
        static void Present();

        static const uint32_t PlayXOffset = ((SCREEN_WIDTH-640)/2);
		static const uint32_t PlayYOffset = 73;

    bool LoadTexture(TextureType texid, const std::string& path);

protected:
        std::map<TextureType, SDL_Surface*> mapsurfaces;
		std::map<TextureType, SDL_Texture*> maptextures;
        std::shared_mutex mut_maptextures;

    void CreateRectangularTexture(TextureType texid, uint32_t width, uint32_t height, SDL_Color c);


        static int32_t ForceBounds(int32_t value);
		static GraphicsManager sGraphicsManager;
	
	private:
		GraphicsManager();
		~GraphicsManager() = default;
};

#endif // __GRAPHICSMANAGER_H__
