#include "Ruleset.h"
#include "Graphics/pText.h"
#include "DataStorage/Scores.h"

void OnBtnResumeClick(pDrawable* self, OOInt x, OOInt y) {
    InputHelper::SimulateKeyDown(Control::IH_CONTROL_QUIT);
}

void OnBtnRetryClick(pDrawable* self, OOInt x, OOInt y) {
    ChangeModeOnFrameEnd(MODE_PLAYER);
}

void OnBtnBackClick(pDrawable* self, OOInt x, OOInt y) {
    ChangeModeOnFrameEnd(MODE_SONGSELECT);
}


Ruleset::Ruleset()
{
    mDarkenOverlay.AddToSpriteManager(mSpriteManager);
	mLifebar.AddToSpriteManager(mSpriteManager);
#ifdef __vita__
    // The setting is always false unless running on the Vita.
    mCursor = Settings::get_bool("vitaUseBackTouch");
#endif
}

void Ruleset::Initialize()
{
    mDarkenOverlay.Initialize();
	mLifebar.Initialize();

    if (mCursor) {
        mCursorSize = (int)round(DifficultyManager::CircleDiameterPx * 0.83);
        mCursorSprite = new pSprite(TX_PLAY_CURSOR, 100, 100, mCursorSize, mCursorSize, ORIGIN_CENTER, FIELD_SCREEN, SDL_Color(), 255, -1000090.f);
        mSpriteManager.Add(mCursorSprite);
    }
}

void Ruleset::Skip()
{
    if (GameClock::Clock().Time() < BeatmapManager::Current().SkipTime())
	{
        if (!mMusicStarted) {
            StartMusic();
        }

        AudioManager::Engine().MusicSkipTo(BeatmapManager::Current().SkipTime());
		GameClock::Clock().SkipTo(BeatmapManager::Current().SkipTime());

		OnSkip();
	}
}


void Ruleset::OnGameOver()
{
    int now = GameClock::Clock().Time();
    mCurrentScore.CalculateGrade();
    std::vector<pDrawable *> scoreScreenSprites;

    auto* spr = new pSprite(TX_GAMEOVER_BG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 0, 0);
    scoreScreenSprites.push_back(spr);

    pSprite* btn_retry = new pSprite(TX_BUTTON_BIG, 602, 337, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_retry->OnClick = OnBtnRetryClick;
    btn_retry->Clickable = true;
    scoreScreenSprites.push_back(btn_retry);
    pText* btn_retry_label = new pText("Retry", FONT_PIXEL, 740, 364, SDL_Color({67,19,115}));
    btn_retry_label->Origin = ORIGIN_CENTER;
    scoreScreenSprites.push_back(btn_retry_label);

    pSprite* btn_back = new pSprite(TX_BUTTON_BIG, 602, 405, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_back->OnClick = OnBtnBackClick;
    btn_back->Clickable = true;
    scoreScreenSprites.push_back(btn_back);
    pText* btn_back_label = new pText("Back", FONT_PIXEL, 740, 432, SDL_Color({67,19,115}));
    btn_back_label->Origin = ORIGIN_CENTER;
    scoreScreenSprites.push_back(btn_back_label);

    TextureType rankingtex;
    if (strcmp(mCurrentScore.GetGrade(), "SS") == 0) {
        AudioManager::Engine().PlayUISound(UISOUND_APPLAUSE);
        rankingtex = TX_RANKING_SS_LG;
    } else if (strcmp(mCurrentScore.GetGrade(), "S") == 0) {
        AudioManager::Engine().PlayUISound(UISOUND_APPLAUSE);
        rankingtex = TX_RANKING_S_LG;
    } else if (strcmp(mCurrentScore.GetGrade(), "A") == 0) {
        AudioManager::Engine().PlayUISound(UISOUND_APPLAUSE);
        rankingtex = TX_RANKING_A_LG;
    } else if (strcmp(mCurrentScore.GetGrade(), "B") == 0) {
        rankingtex = TX_RANKING_B_LG;
    } else if (strcmp(mCurrentScore.GetGrade(), "C") == 0) {
        rankingtex = TX_RANKING_C_LG;
    } else {
        rankingtex = TX_RANKING_D_LG;
    }

    auto* rankspr = new pSprite(rankingtex, 741, 163, 212, 243, ORIGIN_CENTER, FIELD_SCREEN, SDL_Color(), 0, 0);
    scoreScreenSprites.push_back(rankspr);

    auto scoreLbl = new pText(mCurrentScore.CurrentScoreFormatted(), FONT_NUMBERING, 480, 48, SDL_Color({67,19,115}));
    scoreLbl->Origin = ORIGIN_TOPRIGHT;
    scoreScreenSprites.push_back(scoreLbl);

    scoreLbl = new pText(mCurrentScore.Count300Formatted(), FONT_NUMBERING, 152, 162, SDL_Color({67,19,115}));
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.Count100Formatted(), FONT_NUMBERING, 152, 237, SDL_Color({67,19,115}));
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.Count50Formatted(), FONT_NUMBERING, 152, 311, SDL_Color({67,19,115}));
    scoreScreenSprites.push_back(scoreLbl);

    scoreLbl = new pText(mCurrentScore.CountGekiFormatted(), FONT_NUMBERING, 369, 162, SDL_Color({67,19,115}));
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.CountKatuFormatted(), FONT_NUMBERING, 369, 237, SDL_Color({67,19,115}));
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.CountMissFormatted(), FONT_NUMBERING, 369, 311, SDL_Color({67,19,115}));
    scoreScreenSprites.push_back(scoreLbl);

    scoreLbl = new pText(mCurrentScore.CountAccuracyFormatted(), FONT_NUMBERING, 77, 412, SDL_Color({67,19,115}));
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.MaxComboFormatted(), FONT_NUMBERING, 297, 412, SDL_Color({67,19,115}));
    scoreScreenSprites.push_back(scoreLbl);

    for (auto s : scoreScreenSprites) {
        s->Alpha = 0;
        s->Show(now, now+700);
    }

    mSpriteManager.Add(scoreScreenSprites);

    mLifebar.Kill();

    Highscore h_old;
    Scores::get_highscore(BeatmapManager::Current().BeatmapChecksum(), h_old);
    if (h_old.score < mCurrentScore.CurrentScore()) {
        Highscore h;
        h.accuracy = mCurrentScore.CountAccuracy();
        h.combo = mCurrentScore.MaxCombo();
        h.score = mCurrentScore.CurrentScore();
        h.checksum = BeatmapManager::Current().BeatmapChecksum();
        strcpy(h.grade, mCurrentScore.GetGrade());
        Scores::set_highscore(h);
        Scores::save();
    }
}

void Ruleset::OnFailed()
{
    AudioManager::Engine().PlayUISound(UISOUND_FAILSOUND);

    int now = GameClock::Clock().Time();
    std::vector<pDrawable *> scoreScreenSprites;

    auto* spr = new pSprite(TX_GAMEOVER_BG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 0, 0);
    scoreScreenSprites.push_back(spr);

    pSprite* btn_retry = new pSprite(TX_BUTTON_BIG, 602, 337, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_retry->OnClick = OnBtnRetryClick;
    btn_retry->Clickable = true;
    scoreScreenSprites.push_back(btn_retry);
    pText* btn_retry_label = new pText("Retry", FONT_PIXEL, 740, 364, SDL_Color({67,19,115}));
    btn_retry_label->Origin = ORIGIN_CENTER;
    scoreScreenSprites.push_back(btn_retry_label);

    pSprite* btn_back = new pSprite(TX_BUTTON_BIG, 602, 405, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    btn_back->OnClick = OnBtnBackClick;
    btn_back->Clickable = true;
    scoreScreenSprites.push_back(btn_back);
    pText* btn_back_label = new pText("Back", FONT_PIXEL, 740, 432, SDL_Color({67,19,115}));
    btn_back_label->Origin = ORIGIN_CENTER;
    scoreScreenSprites.push_back(btn_back_label);

    auto* rankspr = new pSprite(TX_RANKING_F_LG, 741, 163, 212, 243, ORIGIN_CENTER, FIELD_SCREEN, SDL_Color(), 0, 0);
    scoreScreenSprites.push_back(rankspr);

    auto scoreLbl = new pText(mCurrentScore.CurrentScoreFormatted(), FONT_NUMBERING, 480, 48, SDL_Color({67,19,115}));
    scoreLbl->Origin = ORIGIN_TOPRIGHT;
    scoreScreenSprites.push_back(scoreLbl);

    scoreLbl = new pText(mCurrentScore.Count300Formatted(), FONT_NUMBERING, 152, 162, SDL_Color({67,19,115}));
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.Count100Formatted(), FONT_NUMBERING, 152, 237, SDL_Color({67,19,115}));
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.Count50Formatted(), FONT_NUMBERING, 152, 311, SDL_Color({67,19,115}));
    scoreScreenSprites.push_back(scoreLbl);

    scoreLbl = new pText(mCurrentScore.CountGekiFormatted(), FONT_NUMBERING, 369, 162, SDL_Color({67,19,115}));
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.CountKatuFormatted(), FONT_NUMBERING, 369, 237, SDL_Color({67,19,115}));
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.CountMissFormatted(), FONT_NUMBERING, 369, 311, SDL_Color({67,19,115}));
    scoreScreenSprites.push_back(scoreLbl);

    scoreLbl = new pText(mCurrentScore.CountAccuracyFormatted(), FONT_NUMBERING, 77, 412, SDL_Color({67,19,115}));
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.MaxComboFormatted(), FONT_NUMBERING, 297, 412, SDL_Color({67,19,115}));
    scoreScreenSprites.push_back(scoreLbl);

    for (auto s : scoreScreenSprites) {
        s->Alpha = 0;
        s->Show(now, now+700);
    }

    mSpriteManager.Add(scoreScreenSprites);

    mLifebar.Kill();
}

void Ruleset::OnPause() {
    AudioManager::Engine().MusicPause();
    GameClock::Clock().Pause();

    auto* spr = new pSprite(TX_PAUSE_BG, 299, 168, 361, 275, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -1000000.f);
    pauseMenuSprites.push_back(spr);

    auto* btn_resume = new pSprite(TX_BUTTON_BIG, 341, 210, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -1000001.f);
    btn_resume->OnClick = OnBtnResumeClick;
    btn_resume->Clickable = true;
    pauseMenuSprites.push_back(btn_resume);
    auto* btn_resume_label = new pText("Resume", FONT_PIXEL, 479, 237, SDL_Color({67,19,115}));
    btn_resume_label->Origin = ORIGIN_CENTER;
    btn_resume_label->Z = -1000002.f;
    pauseMenuSprites.push_back(btn_resume_label);

    auto* btn_retry = new pSprite(TX_BUTTON_BIG, 341, 278, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -1000001.f);
    btn_retry->OnClick = OnBtnRetryClick;
    btn_retry->Clickable = true;
    pauseMenuSprites.push_back(btn_retry);
    pText* btn_retry_label = new pText("Retry", FONT_PIXEL, 479, 305, SDL_Color({67,19,115}));
    btn_retry_label->Origin = ORIGIN_CENTER;
    btn_retry_label->Z = -1000002.f;
    pauseMenuSprites.push_back(btn_retry_label);

    pSprite* btn_back = new pSprite(TX_BUTTON_BIG, 341, 346, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -1000001.f);
    btn_back->OnClick = OnBtnBackClick;
    btn_back->Clickable = true;
    pauseMenuSprites.push_back(btn_back);
    pText* btn_back_label = new pText("Back", FONT_PIXEL, 479, 373, SDL_Color({67,19,115}));
    btn_back_label->Origin = ORIGIN_CENTER;
    btn_back_label->Z = -1000002.f;
    pauseMenuSprites.push_back(btn_back_label);

    mSpriteManager.Add(pauseMenuSprites);
}

void Ruleset::OnPauseEnd() {
    for (pDrawable* s : pauseMenuSprites) {
        s->Kill(GameClock::Clock().Time());
    }
    pauseMenuSprites.clear();

    AudioManager::Engine().MusicResume();
    GameClock::Clock().Resume();
}

void Ruleset::UpdatePause() {
    if (mCursor) {
        mCursorSprite->X = (int32_t)InputHelper::TouchRead().px;
        mCursorSprite->Y = (int32_t)InputHelper::TouchRead().py;
    }

    GraphicsManager::Graphics().DrawFullScreenRectangle({0,0,0,100});
    mSpriteManager.Draw();
}

void Ruleset::UpdateGameOver()
{
    OOTime now = GameClock::Clock().Time();
    if (mCursor) {
        mCursorSprite->X = (int32_t)InputHelper::TouchRead().px;
        mCursorSprite->Y = (int32_t)InputHelper::TouchRead().py;
        if (InputHelper::KeyDown(Control::IH_CONTROL_ACTION)) {
            mCursorSprite->Scale(now, now+100, 1.f, 0.8f);
        }
        if (InputHelper::KeyUp(Control::IH_CONTROL_ACTION)) {
            mCursorSprite->Scale(now, now+100, 0.8f, 1.f);
        }
    }

    GraphicsManager::Graphics().DrawFullScreenRectangle({0,0,0,100});
    mSpriteManager.Draw();
}

void Ruleset::UpdateFailed()
{
    OOTime now = GameClock::Clock().Time();
    if (mCursor) {
        mCursorSprite->X = (int32_t)InputHelper::TouchRead().px;
        mCursorSprite->Y = (int32_t)InputHelper::TouchRead().py;
        if (InputHelper::KeyDown(Control::IH_CONTROL_ACTION)) {
            mCursorSprite->Scale(now, now+100, 1.f, 0.8f);
        }
        if (InputHelper::KeyUp(Control::IH_CONTROL_ACTION)) {
            mCursorSprite->Scale(now, now+100, 0.8f, 1.f);
        }
    }

    GraphicsManager::Graphics().DrawFullScreenRectangle({0,0,0,100});
    mSpriteManager.Draw();
}

bool Ruleset::Update()
{
    OOTime now = GameClock::Clock().Time();

    if (mCursor) {
        mCursorSprite->X = (int32_t)InputHelper::TouchRead().px;
        mCursorSprite->Y = (int32_t)InputHelper::TouchRead().py;
        if (InputHelper::KeyDown(Control::IH_CONTROL_ACTION)) {
            mCursorSprite->Scale(now, now+100, 1.f, 0.8f);
        }
        if (InputHelper::KeyUp(Control::IH_CONTROL_ACTION)) {
            mCursorSprite->Scale(now, now+100, 0.8f, 1.f);
        }
    }

    if (!mMusicStarted && now >= 0) {
        StartMusic();
    }

	std::list<HitObject*> hitObjectList;
	BeatmapManager::Current().Buffer(hitObjectList);
	
	for (auto object : hitObjectList)
	{
        mHitObjectManager.Add(object);
		object->AddToSpriteManager(mSpriteManager);
	}
	
	mLifebar.Update();
	mSpriteManager.Draw();

    // score/combo output

    TextManager::SetFont(FONT_SCORE);
    TextManager::PrintLocate(940, 10, ORIGIN_TOPRIGHT, "  %08u", mCurrentScore.CurrentScore());
    TextManager::PrintLocate(910, 44, ORIGIN_TOPRIGHT, "  %.2f%%", mCurrentScore.CountAccuracy());
    TextManager::PrintLocate(20, 528, ORIGIN_BOTTOMLEFT, "%ix   ", mCurrentScore.CurrentCombo());

    if(mLifebar.GetCurrentHP() == 0.f && now > BeatmapManager::Current().SkipTime()) {
        return false; // Premature game over, HP is 0
    }
    return true;
}

void Ruleset::HandleInput()
{
	mHitObjectManager.HandleInput();
}

void Ruleset::StopMusic() {
    mMusicStarted = false;
    AudioManager::Engine().MusicStop();
}

void Ruleset::StartMusic() {
    mMusicStarted = true;
    AudioManager::Engine().MusicPlay(Settings::get_float("volume_music"));
    GameClock::Clock().Reset();
}