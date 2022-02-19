#include "SDL.h"
#include "RulesetOsu.h"

RulesetOsu::RulesetOsu()
{
	HitObject::SetScoreCallback(this);
	mScoreZ = 0;
	mSpecialScore = 0;
}

void RulesetOsu::IncreaseScore(ScoreType score, bool forceNoCombo, bool forceNoAnimation, HitObjectPoint point, bool comboEnd)
{
	int now = GameClock::Clock().Time();
	
	TextureType tex;
	float hpIncrease;

    printf("comboEnd: %i\n",comboEnd);
	
	switch (score)
	{
		case SCORE_300:
			if (comboEnd)
			{
				tex = (TextureType)(TX_PLAY_HIT300 + mSpecialScore);
				mSpecialScore = 2;
				hpIncrease = Lifebar::HP_300 + (mSpecialScore == 2 ? Lifebar::HP_GEKI : (mSpecialScore == 1 ? Lifebar::HP_KATSU : 0));
			}
			else
			{
				tex = TX_PLAY_HIT300;
				hpIncrease = Lifebar::HP_300;
			}
			break;
		
		case SCORE_100:
			mSpecialScore = (mSpecialScore > 1 ? 1 : mSpecialScore);
			if (comboEnd)
			{
				tex = (TextureType)(TX_PLAY_HIT100 + mSpecialScore);
				mSpecialScore = 2;
				hpIncrease = Lifebar::HP_100 + (mSpecialScore == 1 ? Lifebar::HP_KATSU : 0);
			}
			else
			{
				tex = TX_PLAY_HIT100;
				hpIncrease = Lifebar::HP_100;
			}
			break;
		
		case SCORE_50:
			mSpecialScore = (comboEnd ? 2 : 0);
			tex = TX_PLAY_HIT50;
			hpIncrease = Lifebar::HP_50;
			break;
		
		case SCORE_MISS:
		default:
			mSpecialScore = (comboEnd ? 2 : 0);
			tex = TX_PLAY_HIT0;
			hpIncrease = DifficultyManager::GetMissHpDrain();
			break;
		
		case SCORE_TICK_30:
			hpIncrease = Lifebar::HP_SLIDER_REPEAT;
			break;
		
		case SCORE_TICK_10:
			hpIncrease = Lifebar::HP_SLIDER_TICK;
			break;
		
		case SCORE_SPIN_100:
			hpIncrease = Lifebar::HP_SPINNER_SPIN;
			break;
		
		case SCORE_SPIN_1000:
			hpIncrease = Lifebar::HP_SPINNER_BONUS;
			break;
	}
	
	mCurrentScore.Add(score, forceNoCombo, (mSpecialScore == 2));
	mLifebar.Increase(hpIncrease);
	
	if (!forceNoAnimation)
	{	
		pSprite* spr = new pSprite(tex, point.x, point.y, 160, 160, ORIGIN_CENTER, FIELD_PLAY, SDL_Color({0,0,0}), 0, mScoreZ);
		spr->Show(now, now+100);
		spr->Hide(now+500, now+1000);
		spr->Kill(now+1500);
		
		//NEGATIVE deltaz to bring to front
		mScoreZ -= 0.02f;
		
		if (tex == TX_PLAY_HIT0)
		{
			spr->Scale(now, now+100, 2, 1);
		}
		else
		{
			spr->Scale(now, now+100, 0.8, 1);
		}
		
		mSpriteManager.Add(spr);
	}
}

void RulesetOsu::OnSkip()
{
	mLifebar.ClearTransforms();
	mLifebar.Increase(Lifebar::MAXHP);
}

void RulesetOsu::OnGameOver()
{
	mCurrentScore.CalculateGrade();
}

void RulesetOsu::OnFailed()
{
    //
}

void RulesetOsu::DoCallback(void** args)
{
	IncreaseScore(
		ARGS_POP(args, 0, ScoreType),
		ARGS_POP(args, 1, bool),
		ARGS_POP(args, 2, bool),
		ARGS_POP(args, 3, HitObjectPoint),
		ARGS_POP(args, 4, bool)
	);
}

