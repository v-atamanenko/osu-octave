#include "SongSelect.h"

#include "Graphics/pText.h"

void OnMapSpriteClick(pDrawable* self, u16 x, u16 y) {
	int index = (int) self->Tag;

	BeatmapManager::Load(index); 
	ChangeMode(MODE_PLAYER);
}

SongSelect::SongSelect()
{

	for(int i = 0; i != BeatmapManager::SongCount(); i++) {
		pSprite* sprite = new pSprite(TX_WHITE, 16, 16 + (i * 75), 608, 60, ORIGIN_TOPLEFT, FIELD_SCREEN, RGB15(4,4,4), 80);

		Beatmap* map = BeatmapManager::Beatmaps()[i];
		
		sprite->OnClick = OnMapSpriteClick;
		sprite->Clickable = true;
		sprite->Tag = (void*) i;
		sprite->Z = 5;

		mSpriteManager.Add(sprite);

		std::string mapString = map->Artist() + " - " + map->Title() + " [" + map->Version() + "]";

		pText* mapText = new pText(mapString, FONT_CONSOLE, 16, 32 + (i * 75));
		mapText->Z = 0;

		std::string debugString = mapString + ":::: Y:" + std::to_string(mapText->Y);
		nocashMessage(debugString.c_str());

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
		u32 delta = mDeltaX / 6;
		
		if (delta == 0 && mDeltaX != 0)
			delta = MathHelper::Sgn(mDeltaX);
		
		mDeltaX -= delta;
		//MoveSongList(delta);
	}
	
	mSpriteManager.Draw();

	// beatmap names
}

void SongSelect::MoveSongList(s32 dx)
{	
	
}

void SongSelect::MoveSongListLeft()
{

}

void SongSelect::MoveSongListRight()
{

}

