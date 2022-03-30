#pragma once

#include <cstring>
#include <string>
#include <algorithm>
#include <cstdio>
#include <map>
#include <shared_mutex>

#include <SDL.h>
#include "SDL_image.h"

#include "GfxInfo.h"

#include "defines.h"
#include "Modes/Mode.h"
#include "DataStorage/Settings.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define rmask 0xff000000
#define gmask 0x00ff0000
#define bmask 0x0000ff00
#define amask 0x000000ff
#else
#define rmask 0x000000ff
#define gmask 0x0000ff00
#define bmask 0x00ff0000
#define amask 0xff000000
#endif

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

		void Draw(TextureType tex, OOInt x, OOInt y, OOUInt width, OOUInt height, DrawOrigin origin, FieldType fieldtype, SDL_Color color, OOUInt alpha, OOInt angle, OOFloat z = 0, const SDL_Rect* uv = nullptr);

        void CreateTextureFromSurface(SDL_Surface* bg, TextureType texid);
        void LoadBeatmapPicTexture(TextureType texid, SDL_Surface *tex);
        void LoadBeatmapBackground(const std::string& path);
        void DrawBeatmapBackground();

        void LoadTexturesForMode(ModeType mod);
        void UnloadTextures();
        SDL_Texture * GetTexture(TextureType texid);

        void DrawFullScreenRectangle(SDL_Color c);

        static void Clear();
        static void Present();

        static bool ScaleSurface(SDL_Surface*& pSurface, OOInt limit);
        static bool CropSurfaceToSquare(SDL_Surface*& loadedSurface);

        static const OOUInt PlayXOffset = PLAYFIELD_X_OFFSET;
		static const OOUInt PlayYOffset = PLAYFIELD_Y_OFFSET;

    bool LoadTexture(TextureType texid, const std::string& path);

    protected:
        std::map<TextureType, SDL_Surface*> mapsurfaces;
		std::map<TextureType, SDL_Texture*> maptextures;
        std::shared_mutex mut_maptextures;

        void CreateRectangularTexture(TextureType texid, OOUInt width, OOUInt height, SDL_Color c);

        static OOInt ForceBounds(OOInt value);
		static GraphicsManager sGraphicsManager;
	
	private:
		GraphicsManager() = default;
		~GraphicsManager() = default;
};
