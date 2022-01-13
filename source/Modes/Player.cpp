#include "Player.h"
#include "menuBG.h"

Player::Player()
{
	//initialisation
	mRuleset.Initialize();

	//load audio
	BeatmapManager::Current().InitBG();
	ChangeToSongDir();
	AudioManager::Engine().MusicPlay(BeatmapManager::Current().AudioFilename());

	//set player to process gameplay
	mPlayState = PLAYSTATE_PLAY;
}

Player::~Player()
{
	//delete mBaseDir;
	AudioManager::Engine().MusicStop();
	memcpy(BG_BMP_RAM(8), menuBGBitmap, menuBGBitmapLen);
}

void Player::Update()
{
	switch (mPlayState)
	{
		case PLAYSTATE_PLAY:
		{
			mRuleset.Update();

			if (BeatmapManager::Current().GameOver())
			{
				mPlayState = PLAYSTATE_GAMEOVER;
				mRuleset.OnGameOver();
			}

			break;
		}

		case PLAYSTATE_GAMEOVER:
		{
			iprintf("\x1b[0;0HGame over");
			break;
		}

		default:
			break;
	}

	AudioManager::Engine().MusicUpdate();
}

void Player::HandleInput()
{
	mRuleset.HandleInput();

	//handle play mode input
	if (InputHelper::KeyDown(KEY_A))
	{
		mRuleset.Skip();
	}

	if (InputHelper::KeyDown(KEY_SELECT))
	{
		ChangeMode(MODE_SONGSELECT);
		return;
	}
}

void Player::ChangeToSongDir()
{
	ChangeToOsuDir();
	chdir(BeatmapManager::Current().BaseDir().c_str());
}

