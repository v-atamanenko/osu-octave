#include "Ruleset.h"

Ruleset::Ruleset()
{
	mLifebar.AddToSpriteManager(mSpriteManager);
}

void Ruleset::Initialize()
{
	mLifebar.Initialize();
}

void Ruleset::Skip()
{
    fprintf(stderr, "SkipTime: %i\n",BeatmapManager::Current().SkipTime());
	if (GameClock::Clock().Time() < BeatmapManager::Current().SkipTime())
	{
		AudioManager::Engine().MusicSkipTo(BeatmapManager::Current().SkipTime());
		GameClock::Clock().SkipTo(BeatmapManager::Current().SkipTime());
		
		// template hook
		OnSkip();
	}
}

void Ruleset::OnGameOver()
{
    // Defined in RulesetOsu
}

void Ruleset::OnFailed()
{
    // Defined in RulesetOsu
}

void Ruleset::UpdateGameOver()
{
    GraphicsManager::Graphics().DrawFullScreenRectangle({0,0,0,100});

    TextManager::Bottom().SetFont(FONT_SCORE);

    TextManager::Bottom().PrintScore(450,75, ORIGIN_TOPLEFT, "Grade: %s", mCurrentScore.GetGrade());

    TextManager::Bottom().PrintScore(0,0, ORIGIN_TOPLEFT, "Score: %i", mCurrentScore.CurrentScore());

    TextManager::Bottom().PrintScore(25, 25, ORIGIN_TOPLEFT, "300: %ix", mCurrentScore.Count300());
    TextManager::Bottom().PrintScore(225, 25, ORIGIN_TOPLEFT, "Geki: %ix", mCurrentScore.CountGeki());
    TextManager::Bottom().PrintScore(25, 75, ORIGIN_TOPLEFT, "100: %ix", mCurrentScore.Count100());
    TextManager::Bottom().PrintScore(225, 75, ORIGIN_TOPLEFT, "Katu: %ix", mCurrentScore.CountKatu());
    TextManager::Bottom().PrintScore(25, 125, ORIGIN_TOPLEFT, "50: %ix", mCurrentScore.Count50());
    TextManager::Bottom().PrintScore(225, 125, ORIGIN_TOPLEFT, "Miss: %ix", mCurrentScore.CountMiss());

    TextManager::Bottom().PrintScore(25,175, ORIGIN_TOPLEFT, "Combo: %ix", mCurrentScore.CurrentCombo());
    TextManager::Bottom().PrintScore(225,175, ORIGIN_TOPLEFT, "Accuracy: %.2f%%", mCurrentScore.CountAccuracy());
}

void Ruleset::UpdateFailed()
{
    GraphicsManager::Graphics().DrawFullScreenRectangle({0,0,0,100});

    TextManager::Bottom().SetFont(FONT_SCORE);

    TextManager::Bottom().PrintScore(0,0, ORIGIN_CENTER, "WASTED%s", nullptr);
    TextManager::Bottom().PrintScore(450,75, ORIGIN_TOPLEFT, "Grade: %s", mCurrentScore.GetGrade());

    TextManager::Bottom().PrintScore(0,0, ORIGIN_TOPLEFT, "Score: %i", mCurrentScore.CurrentScore());

    TextManager::Bottom().PrintScore(25, 25, ORIGIN_TOPLEFT, "300: %ix", mCurrentScore.Count300());
    TextManager::Bottom().PrintScore(225, 25, ORIGIN_TOPLEFT, "Geki: %ix", mCurrentScore.CountGeki());
    TextManager::Bottom().PrintScore(25, 75, ORIGIN_TOPLEFT, "100: %ix", mCurrentScore.Count100());
    TextManager::Bottom().PrintScore(225, 75, ORIGIN_TOPLEFT, "Katu: %ix", mCurrentScore.CountKatu());
    TextManager::Bottom().PrintScore(25, 125, ORIGIN_TOPLEFT, "50: %ix", mCurrentScore.Count50());
    TextManager::Bottom().PrintScore(225, 125, ORIGIN_TOPLEFT, "Miss: %ix", mCurrentScore.CountMiss());

    TextManager::Bottom().PrintScore(25,175, ORIGIN_TOPLEFT, "Combo: %ix", mCurrentScore.CurrentCombo());
    TextManager::Bottom().PrintScore(225,175, ORIGIN_TOPLEFT, "Accuracy: %.2f%%", mCurrentScore.CountAccuracy());
}

bool Ruleset::Update()
{
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
	
	TextManager::Bottom().SetFont(FONT_SCORE);
    TextManager::Bottom().PrintScore(osuPixelsXtoScreenX(635), 25, ORIGIN_TOPRIGHT, "  %08u", mCurrentScore.CurrentScore());
	TextManager::Bottom().PrintScore(25, 25, ORIGIN_BOTTOMLEFT, "%ix   ", mCurrentScore.CurrentCombo());

    if(mLifebar.GetCurrentHP() == 0.f && GameClock::Clock().Time() > BeatmapManager::Current().SkipTime())
        return false; // Premature game over, HP is 0

    return true;
}

void Ruleset::HandleInput()
{
	mHitObjectManager.HandleInput();
}

