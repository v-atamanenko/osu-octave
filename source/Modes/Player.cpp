#include "Player.h"
//#include "menuBG.h"

Player::Player()
{
    GraphicsManager::Graphics().LoadTexturesForMode(MODE_PLAYER);
    BeatmapManager::Current().InitBG();

    GameClock::Clock().Reset(BeatmapManager::Current().AudioLeadIn());

    mRuleset.Initialize();

    mPlayState = PLAYSTATE_PLAY;
}

Player::~Player()
{
    //delete mBaseDir;
    //GraphicsManager::Graphics().UnloadTextures();
    AudioManager::Engine().MusicStop();
}

void Player::Update()
{
    if (!mMusicStarted && GameClock::Clock().Time() >= 0) {
        mMusicStarted = true;
        AudioManager::Engine().MusicPlay();
        GameClock::Clock().Reset();
    }

    GraphicsManager::Graphics().DrawBeatmapBackground();

    switch (mPlayState) {
        case PLAYSTATE_PLAY: {
            bool failed = !mRuleset.Update();

            if (BeatmapManager::Current().GameOver()) {
                mPlayState = PLAYSTATE_GAMEOVER;
                mRuleset.OnGameOver();
            }

            if (failed) {
                //mPlayState = PLAYSTATE_FAILED;
                //mRuleset.OnFailed();
            }

            break;
        }

        case PLAYSTATE_GAMEOVER: {
            mRuleset.UpdateGameOver();
            break;
        }

        case PLAYSTATE_FAILED: {
            mRuleset.UpdateFailed();
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
        ChangeModeOnFrameEnd(MODE_SONGSELECT);
        return;
    }
}
