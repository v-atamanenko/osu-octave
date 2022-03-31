#include "HitSpinner.h"

HitSpinner::HitSpinner(OOTime time, OOTime endtime, HitObjectSound sound, bool combo, OOInt number_in_combo) : HitObject(256, 192, time, HIT_SPINNER, sound, combo, number_in_combo) {
	mEndTime = endtime;
	fSpinning = false;
	
	mDirection = 0; //0 = not set, values -1 and 1 used
	mZeroPoint = 0; //point to start counting rotations from
	mTotalRotation = 0; //counts rotations in current direction (resets)
	mCurrentRotation = 0; //keeps track of which rotation we are at to count
	mTotalSpins = 0; //counts total number of spins
    mLastAngle = 0;

    // auto spinner_length_seconds = (OOUInt)floor(((OOFloat)mEndTime - (OOFloat)mTime) / 1000.0); // Why would anyone floor that? Ask peppy :P
	// mRequiredSpins = (OOUInt)round((OOFloat)spinner_length_seconds * DifficultyManager::RequiredRPS); //total spins required

    /*
     * A bit of a story. These two variables above (spinner_length_seconds, mRequiredSpins)
     * are produced exactly as McOsu does and apparently as osu! did before 2016 according
     * to my friend who might have seen the source code.
     *
     * But the mRequiredSpins are always very off from current osu! values. I was unable
     * to figure oth the actual algorithm behind the current osu! spinner calculations.
     * But I found an algorithm opsu! uses. It's still off, but not that bad.
     *
     * A few measurements:
     * osu! algo | McOsu algo | opsu algo
     * 40        | 78         | 44
     * 18        | 18         | 14
     * 16        | 24         | 15
     *
     * Should come back to this once >.>
     */

    OOFloat spinner_length_minutes = ((OOFloat)mEndTime - (OOFloat)mTime) / 60000;
    OOFloat spinsPerMinute = 100 + (DifficultyManager::OD * 15);
    mRequiredSpins = ceil(spinsPerMinute * spinner_length_minutes);

    pSprite* spr;
	
	spr = new pSprite(TX_PLAY_CIRCLEAPPROACH, 480, 302, 460, 460, ORIGIN_CENTER, FIELD_SCREEN, SDL_Color(), 0, (OOFloat)time+5000.0-0.06);
	spr->Show(time-300, time);
	spr->Hide(endtime, endtime+300);
	spr->Scale(time-300, endtime, 1, 0);
	spr->Kill(endtime+300);
	mSprites.push_back(spr);

    // Using FIELD_PLAY and SCREEN_X_TO_PLAYFIELD_X/SCREEN_Y_TO_PLAYFIELD_Y here, because mSprites[1] is used as a point
    // for score sprite on spinner hit. And for displaying that sprite, FIELD_PLAY is used always.
	spr = new pSprite(TX_PLAY_SPINNER, SCREEN_X_TO_PLAYFIELD_X(480), SCREEN_Y_TO_PLAYFIELD_Y(302), 440, 440, ORIGIN_CENTER, FIELD_PLAY, SDL_Color(), 0, (OOFloat)time+5000.0-0.05);
	spr->Show(time-300, time);
	spr->Hide(endtime, endtime+300);
	spr->Kill(endtime+300);
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_SPINNERBARS, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_BOTTOMLEFT, FIELD_SCREEN, SDL_Color(), 0, (OOFloat)time+5000.0-0.05);
	spr->Show(time-300, time);
	spr->Hide(endtime, endtime+300);
	spr->Kill(endtime+300);
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_SPINNERBG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 0, (OOFloat)time+5000.0-0.03);
	spr->Show(time-300, time+300);
	spr->Hide(endtime, endtime+300);
	spr->Kill(endtime+300);
	mSprites.push_back(spr);

    spr = new pSprite(TX_PLAY_SPINNER_RPM_BG, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 25, 180, 50, ORIGIN_CENTER, FIELD_SCREEN, SDL_Color(), 0, (OOFloat)time+5000.0-0.06);
    spr->Show(time-300, time+300);
    spr->Hide(endtime, endtime+300);
    spr->Kill(endtime+300);
    mSprites.push_back(spr);

    mRPMIndicator = new pText("0", FONT_SCORE, SCREEN_WIDTH / 2 + 42, SCREEN_HEIGHT - 24, SDL_Color({255,255,255}));
    mRPMIndicator->Show(time-300, time+300);
    mRPMIndicator->Hide(endtime, endtime+300);
    mRPMIndicator->Kill(endtime+300);
    mRPMIndicator->Origin = ORIGIN_CENTER;
    mRPMIndicator->Alpha = 0;
    mSprites.push_back(mRPMIndicator);

	mScoreSpriteId = 1;

    delete mSprites[2]->UV;
    mSprites[2]->UV = new SDL_Rect({0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});

    mChannel = -1;
    mChannelBonus = -1;
}

HitSpinner::~HitSpinner() {
	if (mChannel != -1)
        AudioManager::Engine().StopChannel(mChannel);
    if (mChannelBonus != -1)
        AudioManager::Engine().StopChannel(mChannelBonus);
}

void HitSpinner::Update()
{

	OOFloat ratio = ((OOFloat)mTotalSpins + MathHelper::Frc(mTotalRotation)) / (OOFloat)mRequiredSpins;
    //set spinner bars
	OOUInt height = MathHelper::Max(0, MathHelper::Min(SCREEN_HEIGHT, (OOInt)floor(ratio*(OOFloat)(SCREEN_HEIGHT))) - (OOInt)MathHelper::Random(0, 20));

    delete mSprites[2]->UV;
    mSprites[2]->UV = new SDL_Rect({0, SCREEN_HEIGHT-height, SCREEN_WIDTH, (OOInt)height});

    //set spinner sound
	if (mChannel == -1 && GameClock::Clock().Time() >= mTime)
		mChannel = AudioManager::Engine().PlaySpinnerSound(SND_NORMAL);

    OOTime deltaTime = GameClock::Clock().Time() - mTime;

    if (deltaTime != 0) {
        OOFloat multiplier = 1 / ((OOFloat)deltaTime / 60000); // 60000 ms in m
        mRPM = (mReallyTotalRotation * multiplier) / (((OOFloat)deltaTime / 60000) * multiplier);
        mRPMIndicator->Text = std::to_string((OOInt)(round(mRPM)));
    }
	//if (mChannel != -1)
        //TODO: changing freq
		//AudioManager::Engine().SetChannelFreq(mChannel, MathHelper::Min((uint32_t)(10000 + (ratio * 30000)), 65535));
	
	//trigger score calculation once slider is finished
	if (GameClock::Clock().Time() >= mEndTime)
		Hit();
}

void HitSpinner::OnTouchDown(const touchPosition& touch)
{
	if (GameClock::Clock().Time() >= mTime && GameClock::Clock().Time() <= mEndTime) {
		mLastAngle = GetAngle(touch.px, touch.py);
		fSpinning = true;
	}
}

void HitSpinner::OnTouch(const touchPosition& touch) {
    OOTime now = GameClock::Clock().Time();

	if (now >= mTime && now <= mEndTime) {
		if (MathHelper::Abs((OOInt)touch.px - (SCREEN_WIDTH / 2)) < 10 && MathHelper::Abs((OOInt)touch.py - (SCREEN_HEIGHT/2)) < 10) {
			fSpinning = false;
			return;
		}
		
		//work out the new angle
		OOInt newAngle = GetAngle(touch.px, touch.py);

        // TODO: Right now you can spin a spinner like a DJ's plate moving the cursor back and forth.
        // Might actually be considered a feature 🤔
		
		if (!fSpinning) {
			mLastAngle = 0;
			fSpinning = true;
			return;
		}
		
		if (newAngle == mLastAngle)
			return;
		
		OOInt deltaAngle = newAngle - mLastAngle;
		
		mSprites[1]->Angle += deltaAngle;
		
		mReallyTotalRotation += ((OOFloat)MathHelper::Abs(deltaAngle) / 720);
        mTotalRotation += ((OOFloat)MathHelper::Abs(deltaAngle) / 720);

		//if we have made an extra circle (or more) add to total

		if (mCurrentRotation < (OOUInt)floor(mTotalRotation)) {
            mTotalSpins += (OOUInt)floor(mTotalRotation) - mCurrentRotation;
            mCurrentRotation = (OOUInt)floor(mTotalRotation);
			
			IncreaseScore(SCORE_SPIN_100, true, true);
			if (mTotalSpins > mRequiredSpins) {
				IncreaseScore(SCORE_SPIN_1000, true, true);
				mChannelBonus = AudioManager::Engine().PlaySpinnerSound(SND_BONUS);
			}
		}
		
		mLastAngle = newAngle;
	}
}

void HitSpinner::Hit()
{
	if (mRequiredSpins > 0 && (mTotalSpins == 0 || mTotalSpins < MathHelper::Max(0, (OOInt)(mRequiredSpins - 2)))) {
		IncreaseScore(SCORE_MISS);
	} else {
		//compatibility with osu!pc
		if (mRequiredSpins == 0) {
			IncreaseScore(SCORE_50, false, true); //score sprite doesn't show if you score 50 on spinners
		} else if (mTotalSpins >= mRequiredSpins) {
			IncreaseScore(SCORE_300);
		} else if (mTotalSpins == MathHelper::Max(0, (OOInt)(mRequiredSpins - 1))) {
			IncreaseScore(SCORE_100);
		} else if (mTotalSpins == MathHelper::Max(0, (OOInt)(mRequiredSpins - 2))) {
			IncreaseScore(SCORE_50, false, true); //score sprite doesn't show if you score 50 on spinners
		}
		
		AudioManager::Engine().PlayHitSound(mSound);
	}
	
	if (mChannel != -1)
		AudioManager::Engine().StopChannel(mChannel);

    if (mChannelBonus != -1)
        AudioManager::Engine().StopChannel(mChannelBonus);
	
	mHit = true;
}

OOInt HitSpinner::GetAngle(OOInt x, OOInt y) {
    OOFloat screenmidx = ((OOFloat)SCREEN_WIDTH/2.0);
    OOFloat screenmidy = ((OOFloat)SCREEN_HEIGHT/2.0);
    return ((OOInt) rad2deg(atan2(screenmidx-(OOFloat)x, screenmidy-(OOFloat)y))) * (-1);
}
