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

#ifndef __PLAYER_H__
#define __PLAYER_H__

typedef enum {
	PLAYSTATE_PLAY,
	PLAYSTATE_GAMEOVER,
    PLAYSTATE_FAILED
} PlayState;

class Player : public Mode
{
	public:
		Player();
		~Player();
		
		void Update();
		void HandleInput();
	
	protected:
		RulesetOsu mRuleset;
		PlayState mPlayState;
        bool mNoFail = false;
};

#endif
