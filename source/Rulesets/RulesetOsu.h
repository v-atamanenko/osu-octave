#pragma once

#include <cstdio>

#include "SDL.h"
#include "Ruleset.h"
#include "HitObjects/HitObject.h"
#include "System/ICallback.h"

class RulesetOsu : public Ruleset, public ICallback
{
	public:
		RulesetOsu();
		~RulesetOsu() override = default;
		
		void IncreaseScore(ScoreType score, bool forceNoCombo, bool forceNoAnimation, HitObjectPoint point, bool comboEnd);
		void OnSkip() override;
		
		void DoCallback(void** args) override;
	
	protected:
		int mSpecialScore;
        int mSpecialScorePrev;
};
