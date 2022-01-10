#include "SongSelect.h"

SongSelect::SongSelect()
{
	for (int i=0; i<8; ++i)
	{
		pSprite* spr = new pSprite(TX_WHITE, 0, 0, 160, 120, ORIGIN_TOPLEFT, FIELD_SCREEN, RGB15(4,4,4), 20 + i);
		mSpriteManager.Add(spr);
	}
	
	mCurrentX = 0;
	mDeltaX = 0;
	mIndex = -1;
	
	mColTarget = 0;
	mColCurrent = 0;
	mColMax = BeatmapManager::SongCount() / 2;
	mColSpill = (BeatmapManager::SongCount() % 6) % 2;
	
	MoveSongList(mCurrentX);
}

SongSelect::~SongSelect()
{
}

void SongSelect::HandleInput()
{
	if (InputHelper::KeyDown(KEY_LEFT))
	{
		MoveSongListLeft();
	}
	else if (InputHelper::KeyDown(KEY_RIGHT))
	{
		MoveSongListRight();
	}
	else if (InputHelper::KeyDown(KEY_TOUCH))
	{
		touchPosition pos = InputHelper::TouchRead();
		
		// scrolling by touching the edge of the screen
		if (pos.py > 120 && pos.py < 360)
		{
			if (pos.px < 30)
			{
				MoveSongListLeft();
			}
			else if (pos.px > 610)
			{
				MoveSongListRight();
			}
		}
		
		// calculate vector index (determined by lower right bounding point)
		u32 x = (pos.px - mCurrentX) / kSongListSpacing;
		u32 y = pos.py / kSongListSpacing;
		u32 index = x*2+y;
		
		// calculate upper left bounding point
		u32 left = x * kSongListSpacing + kSongListXOffset + mCurrentX;
		u32 top = y * kSongListSpacing + kSongListYOffset;
		
		// check bounds
		if (pos.px >= left && pos.py >= top)
		{
			// check validity of click
			if (mSpriteManager.Sprites()[index]->Alpha == 31)
			{
				// load beatmap
				//BeatmapManager::Load(index + mColCurrent*2); // take scrolling offset into account
				//ChangeMode(MODE_PLAYER);
				return;
			}
		}
	}
}

void SongSelect::Update()
{
	if (mDeltaX != 0)
	{
		u32 delta = mDeltaX / 6;
		
		if (delta == 0 && mDeltaX != 0)
			delta = MathHelper::Sgn(mDeltaX);
		
		mDeltaX -= delta;
		MoveSongList(delta);
	}
	
	mSpriteManager.Draw();

	// beatmap names
	
	TextManager::Bottom().Clear();
	
	for (int i=0; i<4; ++i)
	{
		for (int j=0; j<2; ++j)
		{
			int index = i*2 + j;
			pSprite* spr = mSpriteManager.Sprites()[index];
			
			index += mColCurrent*2;
			if (index >= BeatmapManager::SongCount())
				break;
			Beatmap* map = BeatmapManager::Beatmaps()[index];

			spr->tag = map->Title();
			spr->Z = j + i;

			TextManager::Bottom().SetFont(FONT_CONSOLE);
			
			TextManager::Bottom().PrintLocate(
				spr->X + 2, spr->Y + spr->Height, ORIGIN_BOTTOMLEFT,
				"%s\n%s\n%s", map->Artist().c_str(), map->Title().c_str(), map->Version().c_str()
			);
		}
	}
}

void SongSelect::MoveSongList(s32 dx)
{	
	mCurrentX += dx;
	
	if (mCurrentX > 0) // scrolled 1 column left
	{
		mCurrentX -= 200;
		mColCurrent -= 1;
	}
	else if (mCurrentX <= -200) // scrolled 1 column right
	{
		mCurrentX += 200;
		mColCurrent += 1;
	}
	
	// reset visibility
	for (int i=0; i<8; i++)
	{
		mSpriteManager.Sprites()[i]->Alpha = 31;
	}
	
	// if we're on the last page, hide the final icons
	if (mColMax - mColCurrent <= 3)
	{
		for (int i=(mColMax-mColCurrent)*2+mColSpill; i<8; i++)
		{
			mSpriteManager.Sprites()[i]->Alpha = 0;
		}
	}
	
	// calculate positions for each box
	for (int i=0; i<4; ++i)
	{
		for (int j=0; j<2; ++j)
		{
			mSpriteManager.Sprites()[i*2+j]->Move(
				kSongListXOffset + mCurrentX + (i * kSongListSpacing),
				kSongListYOffset + (j * kSongListSpacing)
			);
		}
	}
}

void SongSelect::MoveSongListLeft()
{
	if (mColTarget > 0)
	{
		mDeltaX += 600;
		mColTarget -= 3;
	}
}

void SongSelect::MoveSongListRight()
{
	if (mColTarget + 3 < mColMax + mColSpill)
	{
		mDeltaX -= 600;
		mColTarget += 3;
	}
}

