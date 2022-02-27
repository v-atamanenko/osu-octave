#include "Ruleset.h"
#include "Graphics/pText.h"
#include "DataStorage/Scores.h"

void OnBtnRetryClick(pDrawable* self, uint16_t x, uint16_t y) {
    ChangeModeOnFrameEnd(MODE_PLAYER);
}

void OnBtnBackClick(pDrawable* self, uint16_t x, uint16_t y) {
    ChangeModeOnFrameEnd(MODE_SONGSELECT);
}


Ruleset::Ruleset()
{
    mDarkenOverlay.AddToSpriteManager(mSpriteManager);
	mLifebar.AddToSpriteManager(mSpriteManager);
}

void Ruleset::Initialize()
{
    mDarkenOverlay.Initialize();
	mLifebar.Initialize();
}

void Ruleset::Skip()
{
    fprintf(stderr, "SkipTime: %i\n",BeatmapManager::Current().SkipTime());
	if (GameClock::Clock().Time() < BeatmapManager::Current().SkipTime())
	{
        if (!mMusicStarted) {
            fprintf(stderr, "1mMusicStarted: %i", mMusicStarted);
            StartMusic();
        }
        fprintf(stderr, "2mMusicStarted: %i", mMusicStarted);
		AudioManager::Engine().MusicSkipTo(BeatmapManager::Current().SkipTime());
		GameClock::Clock().SkipTo(BeatmapManager::Current().SkipTime());
		
		// template hook
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
        rankingtex = TX_RANKING_SS_LG;
    } else if (strcmp(mCurrentScore.GetGrade(), "S") == 0) {
        rankingtex = TX_RANKING_S_LG;
    } else if (strcmp(mCurrentScore.GetGrade(), "A") == 0) {
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

    auto scoreLbl = new pText(mCurrentScore.CurrentScoreFormatted(), FONT_NUMBERING, 480, 48, SDL_Color());
    scoreLbl->Origin = ORIGIN_TOPRIGHT;
    scoreScreenSprites.push_back(scoreLbl);

    scoreLbl = new pText(mCurrentScore.Count300Formatted(), FONT_NUMBERING, 152, 162, SDL_Color());
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.Count100Formatted(), FONT_NUMBERING, 152, 237, SDL_Color());
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.Count50Formatted(), FONT_NUMBERING, 152, 311, SDL_Color());
    scoreScreenSprites.push_back(scoreLbl);

    scoreLbl = new pText(mCurrentScore.CountGekiFormatted(), FONT_NUMBERING, 369, 162, SDL_Color());
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.CountKatuFormatted(), FONT_NUMBERING, 369, 237, SDL_Color());
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.CountMissFormatted(), FONT_NUMBERING, 369, 311, SDL_Color());
    scoreScreenSprites.push_back(scoreLbl);

    scoreLbl = new pText(mCurrentScore.CountAccuracyFormatted(), FONT_NUMBERING, 77, 412, SDL_Color());
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.CurrentComboFormatted(), FONT_NUMBERING, 297, 412, SDL_Color());
    scoreScreenSprites.push_back(scoreLbl);

    for (auto s : scoreScreenSprites) {
        s->Alpha = 0;
        s->Show(now, now+700);
    }

    mSpriteManager.Add(scoreScreenSprites);

    mLifebar.Kill();

    Highscore h;
    h.accuracy = mCurrentScore.CountAccuracy();
    h.combo = mCurrentScore.CurrentCombo();
    h.score = mCurrentScore.CurrentScore();
    h.checksum = BeatmapManager::Current().BeatmapChecksum();
    strcpy(h.grade, mCurrentScore.GetGrade());
    Scores::set_highscore(h);
    Scores::save();
}

void Ruleset::OnFailed()
{
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

    auto scoreLbl = new pText(mCurrentScore.CurrentScoreFormatted(), FONT_NUMBERING, 480, 48, SDL_Color());
    scoreLbl->Origin = ORIGIN_TOPRIGHT;
    scoreScreenSprites.push_back(scoreLbl);

    scoreLbl = new pText(mCurrentScore.Count300Formatted(), FONT_NUMBERING, 152, 162, SDL_Color());
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.Count100Formatted(), FONT_NUMBERING, 152, 237, SDL_Color());
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.Count50Formatted(), FONT_NUMBERING, 152, 311, SDL_Color());
    scoreScreenSprites.push_back(scoreLbl);

    scoreLbl = new pText(mCurrentScore.CountGekiFormatted(), FONT_NUMBERING, 369, 162, SDL_Color());
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.CountKatuFormatted(), FONT_NUMBERING, 369, 237, SDL_Color());
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.CountMissFormatted(), FONT_NUMBERING, 369, 311, SDL_Color());
    scoreScreenSprites.push_back(scoreLbl);

    scoreLbl = new pText(mCurrentScore.CountAccuracyFormatted(), FONT_NUMBERING, 77, 412, SDL_Color());
    scoreScreenSprites.push_back(scoreLbl);
    scoreLbl = new pText(mCurrentScore.CurrentComboFormatted(), FONT_NUMBERING, 297, 412, SDL_Color());
    scoreScreenSprites.push_back(scoreLbl);

    for (auto s : scoreScreenSprites) {
        s->Alpha = 0;
        s->Show(now, now+700);
    }

    mSpriteManager.Add(scoreScreenSprites);

    mLifebar.Kill();
}

void Ruleset::UpdateGameOver()
{
    GraphicsManager::Graphics().DrawFullScreenRectangle({0,0,0,100});
    mSpriteManager.Draw();
}

void Ruleset::UpdateFailed()
{
    GraphicsManager::Graphics().DrawFullScreenRectangle({0,0,0,100});
    mSpriteManager.Draw();
}

bool Ruleset::Update()
{
    if (!mMusicStarted && GameClock::Clock().Time() >= 0) {
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
    TextManager::PrintLocate(20, 534, ORIGIN_BOTTOMLEFT, "%ix   ", mCurrentScore.CurrentCombo());

    if(mLifebar.GetCurrentHP() == 0.f && GameClock::Clock().Time() > BeatmapManager::Current().SkipTime()) {
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
    AudioManager::Engine().MusicPlay();
    GameClock::Clock().Reset();
}