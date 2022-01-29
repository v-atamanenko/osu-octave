#include "Player.h"
//#include "menuBG.h"

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

    GraphicsManager::Graphics().resetBg();
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
			printf("\x1b[0;0HGame over");
			break;
		}

		default:
			break;
	}

	//AudioManager::Engine().MusicUpdate();
}

void Player::HandleInput()
{
	mRuleset.HandleInput();

	//handle play mode input
	if (InputHelper::KeyDown(SDLK_SPACE, IH_KEY_KEYBOARD) ||
        InputHelper::KeyDown(SDLK_ESCAPE, IH_KEY_KEYBOARD) ||
        InputHelper::KeyDown(SDL_CONTROLLER_BUTTON_A, IH_KEY_CONTROLLER)
        )
	{
		mRuleset.Skip();
	}

	if (InputHelper::KeyDown(SDLK_ESCAPE, IH_KEY_KEYBOARD) ||
        InputHelper::KeyDown(SDL_CONTROLLER_BUTTON_START, IH_KEY_CONTROLLER))
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

