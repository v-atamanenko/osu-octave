#include "SongSelect.h"

#include "Graphics/pText.h"

void OnMapSpriteClick(pDrawable* self, uint16_t x, uint16_t y) {
	int index = (int)(size_t) self->Tag;

	BeatmapManager::Load(index); 
	ChangeMode(MODE_PLAYER);
}

SongSelect::SongSelect()
{
    GraphicsManager::Graphics().setBgByMode(MODE_SONGSELECT);
    GraphicsManager::Graphics().loadTexturesForMode(MODE_SONGSELECT);

    pSprite* btn_import = new pSprite(TX_BUTTON_BIG, 37, 281, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, 1);
    mSpriteManager.Add(btn_import);
    pText* btn_import_label = new pText("import osu! map", FONT_PIXEL, 70, 302, SDL_Color({67,19,115}));
    mSpriteManager.Add(btn_import_label);

    pSprite* btn_settings = new pSprite(TX_BUTTON_BIG, 37, 349, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, 1);
    mSpriteManager.Add(btn_settings);
    pText* btn_settings_label = new pText("settings", FONT_PIXEL, 119, 370, SDL_Color({67,19,115}));
    mSpriteManager.Add(btn_settings_label);

    pSprite* btn_quit = new pSprite(TX_BUTTON_BIG, 37, 418, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, 1);
    mSpriteManager.Add(btn_quit);
    pText* btn_quit_label = new pText("quit", FONT_PIXEL, 140, 438, SDL_Color({67,19,115}));
    mSpriteManager.Add(btn_quit_label);

	for(int i = 0; i != BeatmapManager::SongCount(); i++) {
		pSprite* sprite = new pSprite(TX_WHITE, 16, 16 + (i * 75), 60, 60, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color({4, 4, 4}), 80);

		Beatmap* map = BeatmapManager::Beatmaps()[i];
		
		sprite->OnClick = OnMapSpriteClick;
		sprite->Clickable = true;
		sprite->Tag = (void *) (size_t) i;
		sprite->Z = 5;

		mSpriteManager.Add(sprite);

		std::string mapString = map->Artist() + " - " + map->Title() + " [" + map->Version() + "]";

		pText* mapText = new pText(mapString, FONT_CONSOLE, 240, 75 + (i * 75));
		mapText->Z = 0;


		mSpriteManager.Add(mapText);
	}
}

SongSelect::~SongSelect()
{
}

void SongSelect::HandleInput()
{

}

void SongSelect::Update()
{
	TextManager::Bottom().Clear();

	if (mDeltaX != 0)
	{
		uint32_t delta = mDeltaX / 6;
		
		if (delta == 0 && mDeltaX != 0)
			delta = MathHelper::Sgn(mDeltaX);
		
		mDeltaX -= delta;
		//MoveSongList(delta);
	}
	
	mSpriteManager.Draw();

	// beatmap names
}

void SongSelect::MoveSongList(int32_t dx)
{	
	
}

void SongSelect::MoveSongListLeft()
{

}

void SongSelect::MoveSongListRight()
{

}

