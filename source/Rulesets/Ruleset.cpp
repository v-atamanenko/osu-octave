#include "Ruleset.h"

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
            mMusicStarted = true;
            AudioManager::Engine().MusicPlay();
        }
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

    TextManager::SetFont(FONT_SCORE);

    TextManager::Print(450, 75, "Grade: %s", mCurrentScore.GetGrade());

    TextManager::Print(0, 0, "Score: %i", mCurrentScore.CurrentScore());

    TextManager::Print(25, 25, "300: %ix", mCurrentScore.Count300());
    TextManager::Print(225, 25, "Geki: %ix", mCurrentScore.CountGeki());
    TextManager::Print(25, 75, "100: %ix", mCurrentScore.Count100());
    TextManager::Print(225, 75, "Katu: %ix", mCurrentScore.CountKatu());
    TextManager::Print(25, 125, "50: %ix", mCurrentScore.Count50());
    TextManager::Print(225, 125, "Miss: %ix", mCurrentScore.CountMiss());

    TextManager::Print(25, 175, "Combo: %ix", mCurrentScore.CurrentCombo());
    TextManager::Print(225, 175, "Accuracy: %.2f%%", mCurrentScore.CountAccuracy());
}

void Ruleset::UpdateFailed()
{
    GraphicsManager::Graphics().DrawFullScreenRectangle({0,0,0,100});

    TextManager::SetFont(FONT_SCORE);

    TextManager::PrintLocate(480, 272, ORIGIN_CENTER, "WASTED%s", nullptr);
    TextManager::Print(450, 75, "Grade: %s", mCurrentScore.GetGrade());

    TextManager::Print(0, 0, "Score: %i", mCurrentScore.CurrentScore());

    TextManager::Print(25, 25, "300: %ix", mCurrentScore.Count300());
    TextManager::Print(225, 25, "Geki: %ix", mCurrentScore.CountGeki());
    TextManager::Print(25, 75, "100: %ix", mCurrentScore.Count100());
    TextManager::Print(225, 75, "Katu: %ix", mCurrentScore.CountKatu());
    TextManager::Print(25, 125, "50: %ix", mCurrentScore.Count50());
    TextManager::Print(225, 125, "Miss: %ix", mCurrentScore.CountMiss());

    TextManager::Print(25, 175, "Combo: %ix", mCurrentScore.CurrentCombo());
    TextManager::Print(225, 175, "Accuracy: %.2f%%", mCurrentScore.CountAccuracy());
}

bool Ruleset::Update()
{
    if (!mMusicStarted && GameClock::Clock().Time() >= 0) {
        mMusicStarted = true;
        AudioManager::Engine().MusicPlay();
        GameClock::Clock().Reset();
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

    if(mLifebar.GetCurrentHP() == 0.f && GameClock::Clock().Time() > BeatmapManager::Current().SkipTime())
        return false; // Premature game over, HP is 0

    return true;
}

void Ruleset::HandleInput()
{
	mHitObjectManager.HandleInput();
}

