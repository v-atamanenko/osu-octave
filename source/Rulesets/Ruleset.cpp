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
}

void Ruleset::Update()
{
	list<HitObject*> hitObjectList;
	BeatmapManager::Current().Buffer(hitObjectList);
	
	for (hitObjectIterator it = hitObjectList.begin(); it != hitObjectList.end(); ++it)
	{
		HitObject* object = *it;
		mHitObjectManager.Add(object);
		object->AddToSpriteManager(mSpriteManager);
	}
	
	mLifebar.Update();
	mSpriteManager.Draw();
	
	// score/combo output
	
	TextManager::Bottom().SetFont(FONT_SCORE);
	
	TextManager::Bottom().PrintLocate(635, 5, ORIGIN_TOPRIGHT, "  %08i", mCurrentScore.CurrentScore());
	
	if (mCurrentScore.CurrentCombo() > 0)
		TextManager::Bottom().PrintLocate(5, 475, ORIGIN_BOTTOMLEFT, "%ix    ", mCurrentScore.CurrentCombo());
	else
		TextManager::Bottom().PrintLocate(5, 475, ORIGIN_BOTTOMLEFT, "       ");
}

void Ruleset::HandleInput()
{
	mHitObjectManager.HandleInput();
}

