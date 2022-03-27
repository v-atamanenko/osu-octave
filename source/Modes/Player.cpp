#include "Player.h"
#include "DataStorage/Settings.h"
//#include "menuBG.h"

Player::Player()
{
    GraphicsManager::Graphics().LoadTexturesForMode(MODE_PLAYER);
    BeatmapManager::Current().InitBG();

    GameClock::Clock().Reset(BeatmapManager::Current().AudioLeadIn());

    mRuleset.Initialize();

    mPlayState = PLAYSTATE_PLAY;

    mNoFail = Settings::get_bool("noFail");
}

Player::~Player()
{
    //delete mBaseDir;
    //GraphicsManager::Graphics().UnloadTextures();
    //AudioManager::Engine().MusicStop();
    mRuleset.StopMusic();
}

void Player::Update()
{
    GraphicsManager::Graphics().DrawBeatmapBackground();

    switch (mPlayState) {
        case PLAYSTATE_PLAY: {
            bool failed = !mRuleset.Update();

            if (BeatmapManager::Current().GameOver()) {
                mPlayState = PLAYSTATE_GAMEOVER;
                mRuleset.OnGameOver();
            }

            if (failed && !mNoFail) {
                mPlayState = PLAYSTATE_FAILED;
                mRuleset.OnFailed();
            }

            break;
        }

        case PLAYSTATE_PAUSE: {
            mRuleset.UpdatePause();
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
    if (mPlayState == PLAYSTATE_PLAY)
        mRuleset.HandleInput();

    //handle play mode input
    if (InputHelper::KeyDown(Control::IH_CONTROL_SKIP))
    {
        mRuleset.Skip();
    }

    if (InputHelper::KeyDown(Control::IH_CONTROL_QUIT))
    {
        if (mPlayState == PLAYSTATE_PLAY) {
            mRuleset.OnPause();
            mPlayState = PLAYSTATE_PAUSE;
        } else if (mPlayState == PLAYSTATE_PAUSE) {
            mRuleset.OnPauseEnd();
            mPlayState = PLAYSTATE_PLAY;
        }
    }
}
