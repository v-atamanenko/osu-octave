#pragma once

#include <cstring>
#include "Mode.h"

#include "System/GameClock.h"

#include "GameplayElements/Score.h"
#include "Graphics/SpriteManager.h"
#include "Helpers/AudioManager.h"
#include "GameplayElements/Lifebar.h"
#include "GameplayElements/HitObjectManager.h"
#include "Rulesets/Ruleset.h"
#include "Rulesets/RulesetOsu.h"
#include "DataStorage/Settings.h"

typedef enum {
	PLAYSTATE_PLAY,
    PLAYSTATE_PAUSE,
	PLAYSTATE_GAMEOVER,
    PLAYSTATE_FAILED
} PlayState;

class Player : public Mode
{
	public:
		Player();
		~Player() override;
		
		void Update() override;
		void HandleInput() override;
	
	protected:
		RulesetOsu mRuleset;
		PlayState mPlayState;
        bool mNoFail = false;
};
