#include <nds.h>
#include <stdio.h>

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
		
		virtual void OnGameOver();
		virtual void Update();
		virtual void HandleInput();
	
	protected:
		Score mCurrentScore;
		HitObjectManager mHitObjectManager;
		SpriteManager mSpriteManager;
		Lifebar mLifebar;
		
		virtual void OnSkip() = 0;
};

#endif

