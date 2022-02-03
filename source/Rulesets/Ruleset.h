#include <cstdio>

#include "Beatmaps/BeatmapManager.h"
#include "GameplayElements/Score.h"
#include "Graphics/SpriteManager.h"
#include "GameplayElements/Lifebar.h"
#include "GameplayElements/HitObjectManager.h"
#include "System/TextManager.h"

#ifndef __RULESET_H__
#define __RULESET_H__

class Ruleset
{
	public:
		Ruleset();
		virtual ~Ruleset() {}
		
		void Initialize();
		void Skip();

        virtual void HandleInput();
		virtual void OnGameOver();
        virtual void OnFailed();
		virtual bool Update();
        virtual void UpdateGameOver();
        virtual void UpdateFailed();
	
	protected:
		Score mCurrentScore;
		HitObjectManager mHitObjectManager;
		SpriteManager mSpriteManager;
		Lifebar mLifebar;
		
		virtual void OnSkip() = 0;


};

#endif

