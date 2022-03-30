#include "RulesetOsu.h"

RulesetOsu::RulesetOsu() {
	HitObject::SetScoreCallback(this);
	mSpecialScore = 2;
    mSpecialScorePrev = 2;
}

void RulesetOsu::IncreaseScore(ScoreType score, bool forceNoCombo, bool forceNoAnimation, HitObjectPoint point, bool comboEnd) {
    TextureType tex;
    float hpIncrease = 0;
    OOTime now = GameClock::Clock().Time();

    switch (score) {
        case SCORE_300:
            if (comboEnd) {
				tex = (TextureType)(TX_PLAY_HIT300 + mSpecialScore);
				hpIncrease = (float)Lifebar::HP_300;
                if (mSpecialScore == 2) hpIncrease += Lifebar::HP_GEKI;
                else if (mSpecialScore == 1) hpIncrease += Lifebar::HP_KATSU;
			} else {
				tex = TX_PLAY_HIT300;
				hpIncrease = Lifebar::HP_300;
			}
			break;

		case SCORE_100:
			mSpecialScore = (mSpecialScore >= 1 ? 1 : 0);
			if (comboEnd) {
				tex = (TextureType)(TX_PLAY_HIT100 + mSpecialScore);
				hpIncrease = (float)Lifebar::HP_100 + (mSpecialScore == 1 ? (float)Lifebar::HP_KATSU : 0);
			} else {
				tex = TX_PLAY_HIT100;
				hpIncrease = Lifebar::HP_100;
			}
			break;

		case SCORE_50:
            mSpecialScore = 0;
			tex = TX_PLAY_HIT50;
			hpIncrease = Lifebar::HP_50;
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

        case SCORE_GEKI:
        case SCORE_KATU:
            // Unreachable
            break;

        case SCORE_COMBOBREAK:
            if (mSpecialScore >= 1 && mSpecialScorePrev != 0 && mCurrentScore.CurrentCombo() >= 20) {
                AudioManager::Engine().PlayUISound(UISOUND_COMBOBREAK);
            }
            mSpecialScore = 0;
            break;

        case SCORE_MISS:
        default:
            if (mSpecialScore >= 1 && mSpecialScorePrev != 0 && mCurrentScore.CurrentCombo() >= 20) {
                AudioManager::Engine().PlayUISound(UISOUND_COMBOBREAK);
            }
            mSpecialScore = 0;
            tex = TX_PLAY_HIT0;
            hpIncrease = (float)DifficultyManager::GetMissHpDrain();
            break;
    }

    // TODO: Differentiate Geki/Katu at 300 score.
	mCurrentScore.Add(score, forceNoCombo, comboEnd && (mSpecialScore > 0));
	mLifebar.Increase(hpIncrease);

	if (!forceNoAnimation) {
		auto* spr = new pSprite(tex, point.x, point.y, 120, 120, ORIGIN_CENTER,
                                FIELD_PLAY, SDL_Color(), 0, ((float)now*(-1.f)-1001.f));
		spr->Show(now, now+100);
		spr->Hide(now+300, now+700);
		spr->Kill(now+1100);

		if (tex == TX_PLAY_HIT0) {
			spr->Scale(now, now+100, 1.8, 1);
		} else {
			spr->Scale(now, now+100, 0.8, 1);
		}

		mSpriteManager.Add(spr);
	}

    mSpecialScorePrev = mSpecialScore;

    if (comboEnd) {
        // Reset geki/katu modifier on combo end.
        mSpecialScore = 2;
    }
}

void RulesetOsu::OnSkip() {
	mLifebar.ClearTransforms();
	mLifebar.Increase(Lifebar::MAXHP);
}

void RulesetOsu::DoCallback(void** args) {
	IncreaseScore(
		ARGS_POP(args, 0, ScoreType),
		ARGS_POP(args, 1, bool),
		ARGS_POP(args, 2, bool),
		ARGS_POP(args, 3, HitObjectPoint),
		ARGS_POP(args, 4, bool)
	);
}
