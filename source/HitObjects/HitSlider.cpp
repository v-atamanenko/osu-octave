#include "HitSlider.h"

HitSlider::HitSlider(OOInt x, OOInt y, OOTime time, OOTime lengthtime, std::vector<HitObjectPoint*>& points, std::vector<HitObjectPoint*>& ticks, OOUInt repeats, HitObjectType type, HitObjectSound sound, bool combo, OOInt number_in_combo)
	: HitObject(x, y, time, type, sound, combo, number_in_combo) {
	fTouching = false;
	fStarted = false;
	fFinished = false;
	mTimeLast = time;
	mChannel = -1;
	
	OOUInt pointCount = points.size();
	OOUInt tickCount = ticks.size();
	
	mTickCount = tickCount;
    mTicksHit = 0;
	mRepeats = repeats;
	mRepeatCurrent = 0;
	mTicksTarget = (tickCount + 1) * repeats + 1;
	
	mTicks = new TickSprites[tickCount];
	mTickTimes = new int32_t[mTicksTarget-1];
	
	auto beatTime = (OOTime)floor(BeatmapElements::Element().GetTimingPoint(time).BeatTime);
	
	//populate tick times excluding first hit
    if (DifficultyManager::SliderTickRate > 0) {
        mTickTime = (OOTime) floor((OOFloat)beatTime / DifficultyManager::SliderTickRate);
    } else {
        mTickTime = 0;
    }

	OOTime specialTickTime = lengthtime - (((OOTime)mTickCount)*mTickTime);
	
	//easy if we don't have any ticks, just keep adding lengthtime
	if (tickCount == 0) {
		for (OOUInt i=0; i<mTicksTarget-1; ++i) {
			mTickTimes[i] = time + (lengthtime * (OOTime)(i+1));
		}
	} else {
		//the reason for the lengthiness is the time between the final
		//tick and the slider end is not necessarily the same as mTickTime
		//then there is the reversal of ticks when going backwards
		
		//long story short - it just works :D
		
		OOUInt iterator = 0;
		while (iterator < repeats)
		{
			OOUInt j;
			for (OOUInt i=0; i<tickCount; ++i)
			{
				j = iterator*(tickCount+1)+i;
				if (j==0)
					mTickTimes[0] = time + mTickTime;
				else
					mTickTimes[j] = mTickTimes[j-1] + mTickTime;
			}
			mTickTimes[iterator*(tickCount+1)+tickCount] = mTickTimes[j] + specialTickTime;
			
			++iterator;
			
			if (iterator < repeats)
			{
				mTickTimes[iterator*(tickCount+1)] = mTickTimes[iterator*(tickCount+1)-1] + specialTickTime;
				for (OOUInt i=1; i<tickCount+1; ++i)
				{
					j = iterator*(tickCount+1)+i;
					mTickTimes[j] = mTickTimes[j-1] + mTickTime;
				}
			}
			++iterator;
		}
	}
	
	auto circleSize = (OOUInt)round(DifficultyManager::CircleDiameterPx);
	OOUInt ballSize = (circleSize >> 3) * 7; // 7/8 size
	OOUInt tickSize = circleSize >> 2; // 1/4 size

    OOTime preempt = DifficultyManager::PreemptMs;
    OOTime fadeInStart = time - preempt;
    OOTime fadeInEnd = fadeInStart + DifficultyManager::FadeInMs;
	mEndTime = time + (lengthtime * (OOTime)repeats);
	mLengthTime = lengthtime;
	
	pSprite* spr;
	
	OOUInt fps = 60;
	
	//slider ball
	spr = new pAnimation(TX_PLAY_SLIDERB0, 10, fps, x, y, ballSize, ballSize, ORIGIN_CENTER, FIELD_PLAY, SDL_Color(), 0);
	spr->Show(time);
	MapSliderPath(spr, points, time, lengthtime, repeats);
	spr->Kill(mEndTime);
    if (Skins::get_options().TintSliderBall) spr->Color = mColour;
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_SLIDERFOLLOW, x, y, ballSize, ballSize, ORIGIN_CENTER, FIELD_PLAY, SDL_Color(), 0);
	spr->Scale(time, time+50, 1, 2);
	spr->Scale(mEndTime, mEndTime+50, 2, 1.5);
	MapSliderPath(spr, points, time, lengthtime, repeats);
	spr->Kill(mEndTime+100);
    if (Skins::get_options().TintSliderBall) spr->Color = mColour;
	mSprites.push_back(spr);
	
	if ((repeats&0x1)==1) { //if ODD score is at end
		mScoreSpriteId = 6;
		mSecondaryScoreSpriteId = 4;
	} else {
		mScoreSpriteId = 4;
		mSecondaryScoreSpriteId = 6;
	}
	
	//slider start
	spr = new pSprite(TX_PLAY_CIRCLEAPPROACH, x, y, circleSize, circleSize, ORIGIN_CENTER, FIELD_PLAY, SDL_Color(), 0);
	spr->Show(fadeInStart, fadeInEnd);
	spr->Hide(time, time+100);
	spr->Scale(fadeInStart, time, 4, 1);
	spr->Kill(mEndTime);
    spr->Z = (OOFloat)time-0.6;
    if (Skins::get_options().TintSliderCircles) spr->Color = mColour;
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_CIRCLEOVERLAY, x, y, circleSize, circleSize, ORIGIN_CENTER, FIELD_PLAY, SDL_Color(), 0);
	spr->Show(fadeInStart, fadeInEnd);
	spr->Kill(mEndTime+1000);
    spr->Z = (OOFloat)time - 0.51;
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_CIRCLE, x, y, circleSize, circleSize, ORIGIN_CENTER, FIELD_PLAY, SDL_Color(), 0);
	spr->Show(fadeInStart, fadeInEnd);
	spr->Kill(mEndTime+1000);
    spr->Z = (OOFloat)time - 0.5;
    if (Skins::get_options().TintSliderCircles) spr->Color = mColour;
	mSprites.push_back(spr);
	
	//slider end
    mEndPoint = {points[pointCount-1]->x, points[pointCount-1]->y};

	spr = new pSprite(TX_PLAY_CIRCLEOVERLAY, points[pointCount-1]->x, points[pointCount-1]->y, circleSize, circleSize, ORIGIN_CENTER, FIELD_PLAY, SDL_Color(), 0);

    if (Skins::get_options().ShowSliderEndCircle) {
        spr->Show(fadeInStart, fadeInEnd);
    }

	spr->Kill(mEndTime+1000);
    spr->Z = (OOFloat)time - 0.45;
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_CIRCLE, points[pointCount-1]->x, points[pointCount-1]->y, circleSize, circleSize, ORIGIN_CENTER, FIELD_PLAY, SDL_Color(), 0);

    if (Skins::get_options().ShowSliderEndCircle) {
        spr->Show(fadeInStart, fadeInEnd);
    }

	spr->Kill(mEndTime+1000);
    spr->Z = (OOFloat)time - 0.4;
    if (Skins::get_options().TintSliderCircles) spr->Color = mColour;
	mSprites.push_back(spr);
	
	//slider30 sprites
	spr = new pSprite(TX_PLAY_SLIDER30, points[0]->x, points[0]->y, 40, 40, ORIGIN_CENTER, FIELD_PLAY, SDL_Color(), 0);
	spr->Kill(mEndTime+1000);
    spr->Z = (OOFloat)time-0.7;
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_SLIDER30, points[pointCount-1]->x, points[pointCount-1]->y, 40, 40, ORIGIN_CENTER, FIELD_PLAY, SDL_Color(), 0);
	spr->Kill(mEndTime+1000);
    spr->Z = (OOFloat)time-0.7;
	mSprites.push_back(spr);
	
	//slider repeats
	for (OOUInt i=1; i<repeats; ++i) { //start at 1 because no repeats if repeats==1
		HitObjectPoint* p;
        OOTime tAnimStart, tFadeInEnd;
        OOInt tAngle;
		
		if ((i&0x1)==0) { //if even - slider start
			p = points[0];
			tAngle = p->angle;
			//animations shifted until number disappears
			tAnimStart = time;
			tFadeInEnd = tAnimStart + (preempt >> 3);
		} else { //if odd - slider end
			p = points[pointCount-1];
			tAngle = p->angle + 180;
			tAnimStart = fadeInStart;
			tFadeInEnd = fadeInEnd;
		}

        auto wh = (OOUInt) round((OOFloat)ballSize * 0.8);
		spr = new pSprite(TX_PLAY_SLIDERREVERSE, p->x, p->y, wh, wh, ORIGIN_CENTER, FIELD_PLAY, SDL_Color(), 0);
		spr->Show(tAnimStart, tFadeInEnd);
        spr->Z = (OOFloat)time-0.8;
        spr->Angle = tAngle;
		//todo: slider repeat explosion?
		spr->Hide(time + (lengthtime*(OOTime)i));
		
		//animate arrow
        OOTime tAnimPeriod = floor(BeatmapElements::Element().GetTimingPoint(mEndTime).BeatTime);
		do {
            OOTime tAnimEnd = tAnimStart + 160;
			spr->Scale(tAnimStart, tAnimEnd-80, 1, 0.9);
            spr->Scale(tAnimStart+80, tAnimEnd, 0.9, 1);
			tAnimStart = tAnimEnd + (tAnimPeriod-160);
		} while (tAnimStart < time+(lengthtime*(OOTime)i));
		
		spr->Kill(mEndTime);
		mSprites.push_back(spr);
	}
	
	//slider ticks & score sprites
	for (OOUInt i=0; i<tickCount; ++i) {
		spr = new pSprite(TX_PLAY_SLIDERTICK, ticks[i]->x, ticks[i]->y, tickSize, tickSize, ORIGIN_CENTER, FIELD_PLAY, SDL_Color(), 0);
		spr->Show(fadeInStart, fadeInEnd);
		spr->Hide(mEndTime, mEndTime+120);
		spr->Kill(mEndTime+120);
        spr->Z = (OOFloat)time;
        mSprites.push_back(spr);
		
		//add to list for tracking ticks
		mTicks[i].Tick = spr;
		
		spr = new pSprite(TX_PLAY_SLIDER10, ticks[i]->x, ticks[i]->y, 40, 40, ORIGIN_CENTER, FIELD_PLAY, SDL_Color(), 0);
		spr->Kill(mEndTime+1000);
		mSprites.push_back(spr);
		
		//add to list as well
		mTicks[i].Score = spr;
	}
	
	//slider path
	for (OOUInt i=0; i<pointCount; ++i)
	{
        //slider border
        spr = new pSprite(TX_PLAY_SLIDERBORDER, points[i]->x, points[i]->y, (OOUInt)(ballSize*1.10), (OOUInt)(ballSize*1.10), ORIGIN_CENTER, FIELD_PLAY, SDL_Color(), 0);
        spr->Show(fadeInStart+29, fadeInEnd);
        spr->Hide(mEndTime-31, mEndTime+71);
        spr->Kill(mEndTime+71);
        spr->Z = (OOFloat)time+4.0-((OOFloat)i / 1000);
        spr->Angle = points[i]->angle;
        if (Skins::get_options().TintSliderBorder) spr->Color = mColour;
        mSprites.push_back(spr);

        // slider body
		spr = new pSprite(TX_PLAY_SLIDERBODY, points[i]->x, points[i]->y, ballSize, ballSize, ORIGIN_CENTER, FIELD_PLAY, mColour, 0);
		spr->Show(fadeInStart+30, fadeInEnd);
		spr->Hide(mEndTime-30, mEndTime+70);
		spr->Kill(mEndTime+70);
        spr->Z = (OOFloat)time+0.2-((OOFloat)i / 1000);
        spr->Angle = points[i]->angle;
        if (Skins::get_options().TintSliderPath) spr->Color = mColour;
		mSprites.push_back(spr);
	}

    TextureType numbertex;
    switch (mNextObjectNumberInCombo) {
        case 1: numbertex = TX_PLAY_NUMBER_1; break;
        case 2: numbertex = TX_PLAY_NUMBER_2; break;
        case 3: numbertex = TX_PLAY_NUMBER_3; break;
        case 4: numbertex = TX_PLAY_NUMBER_4; break;
        case 5: numbertex = TX_PLAY_NUMBER_5; break;
        case 6: numbertex = TX_PLAY_NUMBER_6; break;
        case 7: numbertex = TX_PLAY_NUMBER_7; break;
        case 8: numbertex = TX_PLAY_NUMBER_8; break;
        case 9: numbertex = TX_PLAY_NUMBER_9; break;
        default: numbertex = TX_PLAY_NUMBER_0; break;
    }
    auto h = (OOUInt)round((OOFloat)circleSize*0.44);
    auto w = (OOUInt)round((OOFloat)h*(35.0/52.0));
    spr = new pSprite(numbertex, x, y, w, h, ORIGIN_CENTER, FIELD_PLAY, SDL_Color(), 0);
    spr->Show(fadeInStart, fadeInEnd);
    //spr->Hide(time, mEndTime);
    spr->Kill(mEndTime+1000);
    spr->Z = (OOFloat)time-0.6;
    mSprites.push_back(spr);
}

HitSlider::~HitSlider() {
	delete mTicks;
	delete mTickTimes;
	
	if (mChannel != -1)
		AudioManager::Engine().StopChannel(mChannel);
}

bool HitSlider::InBounds(OOInt x, OOInt y) {
	//hit is based on sliderfollowcircle
	return mSprites[1]->InBounds(x, y);
}

void HitSlider::Update() {
	OOTime now = GameClock::Clock().Time();
	
	//trigger score calculation once slider is finished
	if (now >= mEndTime)
		Hit();
	
	//process slider behaviour
	{
		//find the next tick
		OOUInt i = 0;
		for (; i < mTicksTarget; ++i)
		{
			if (mTickTimes[i] > mTimeLast)
				break;
		}
		
		//mTickTimes[i] now refers to the time for the next tick
		
		//any normal tick/slider repeat
		if (i < mTicksTarget-2) //why? try it on a piece of paper >.>
		{
			//if we passed over a tick or repeat
			if (mTickTimes[i] <= now)
			{
				//if it's a tick we hide it and bounce the followcircle
				if ((i+1)%(mTickCount+1) != 0)
				{
					if (fTouching)
					{
						OOUInt real;
						
						//fairly straightfoward if this is the first (or only) repeat
						if (mRepeatCurrent == 0)
						{
							real = i;
						}
						//when we have repeats, we have to calculate the "real" tick
						else
						{
							real = i - (mRepeatCurrent * (mTickCount+1));
							//however, if repeat is ODD we have to count BACKWARDS >.>
							if ((mRepeatCurrent & 0x1) == 1)
							{
								real = (mTickCount-1) - real;
							}
						}
						
						++mTicksHit;
						mTicks[real].Tick->Hide();
						mSprites[1]->Scale(now, now+60, 2.5, 2);
						
						//slider10 animation
						mTicks[real].Score->Show();
						mTicks[real].Score->Hide(now+300, now+350);
						mTicks[real].Score->Move(now, now+250,
												mTicks[real].Tick->X, mTicks[real].Tick->Y,
												mTicks[real].Tick->X, mTicks[real].Tick->Y - 20);
						mTicks[real].Score->Move(now+250, now+320, 
												mTicks[real].Tick->X, mTicks[real].Tick->Y - 20,
												mTicks[real].Tick->X, mTicks[real].Tick->Y - 30);
						
						//normal ticks give 10 points
						IncreaseScore(SCORE_TICK_10, false, true);
						AudioManager::Engine().PlaySliderTick();
					}
					else //if we missed a tick
					{
						IncreaseScore(SCORE_MISS, true, true);
					}
				}
				//otherwise, it's a repeat, and we'll need to reset the ticks
				else
				{
					if (fTouching)
					{
						++mTicksHit;
						mSprites[1]->Scale(now, now+60, 2.5, 2);
						
						OOUInt tSpriteId;
						
						if ((mRepeatCurrent&1)==1) //if ODD
							tSpriteId = 7;
						else
							tSpriteId = 8;
						
						mSprites[tSpriteId]->Show();
						mSprites[tSpriteId]->Hide(now+300, now+350);
						mSprites[tSpriteId]->Move(now, now+250, mSprites[tSpriteId]->X, mSprites[tSpriteId]->Y - 20);
						mSprites[tSpriteId]->Move(now+250, now+320, mSprites[tSpriteId]->X, mSprites[tSpriteId]->Y - 20,
											mSprites[tSpriteId]->X, mSprites[tSpriteId]->Y - 30);
						mSprites[tSpriteId]->Move(now+350, now+350, mSprites[tSpriteId]->X, mSprites[tSpriteId]->Y);
						
						//repeats give 30 points
						IncreaseScore(SCORE_TICK_30, false, true);
						AudioManager::Engine().PlayHitSound(mSound);
					}
					else
					{
						IncreaseScore(SCORE_MISS, true, true);
					}
				}
			}
		}
		//slider end
		else if (fTouching && !fFinished && i == mTicksTarget-2) //again, try it on a piece of paper if you want to know why >.>
		{
			if (now >= mEndTime - DifficultyManager::HitWindow300) //hack - seems like we're missing a frame somewhere >.>
			{
				++mTicksHit;
				
				OOUInt tSpriteId;
				
				if ((mRepeats&1)==0) //if EVEN - mRepeats is always greater than the maximum value of mRepeatCurrent by 1
					tSpriteId = 7;
				else
					tSpriteId = 8;
				
				mSprites[tSpriteId]->Show();
				mSprites[tSpriteId]->Hide(now+300, now+350);
				mSprites[tSpriteId]->Move(now, now+250, mSprites[tSpriteId]->X, mSprites[tSpriteId]->Y - 20);
				mSprites[tSpriteId]->Move(now+250, now+320, mSprites[tSpriteId]->X, mSprites[tSpriteId]->Y - 20,
									mSprites[tSpriteId]->X, mSprites[tSpriteId]->Y - 30);
				mSprites[tSpriteId]->Move(now+350, now+350, mSprites[tSpriteId]->X, mSprites[tSpriteId]->Y);
				
				IncreaseScore(SCORE_TICK_30, true, true);
				fFinished = true;
			}
		}
	}
	
	//reset ticks if ball goes to a repeat regardless of fTouching
	if (now >= mTime + (mLengthTime * OOTime(mRepeatCurrent + 1))) {
		for (OOUInt j=0; j<mTickCount; ++j) {
			//no need to reshow visible sprites
			if (mTicks[j].Tick->Alpha != 255)
				mTicks[j].Tick->Show(now, now+50);
		}
        printf("mRepeatCurrent+\n");
		++mRepeatCurrent;
	}
	
	mTimeLast = now;
}

void HitSlider::OnTouchDown(const touchPosition& touch) {
	if (InBounds(touch.px, touch.py)) {
		OOTime now = GameClock::Clock().Time();
		auto delta = (OOTime)MathHelper::Abs(mTime - now);
		
		//slider start
		if (delta < DifficultyManager::HitWindow && !fStarted) {
			//fTouching = true; //set in OnTouch() which will always be called if OnTouchDown() is called
			fStarted = true;
			mSprites[1]->Transform(TR_FADE, now, mEndTime, 255, 255);
			mSprites[2]->Kill(now);

            mSprites[7]->Show();
            mSprites[7]->Hide(now+300, now+350);
            mSprites[7]->Move(now, now+250, mSprites[7]->X, mSprites[7]->Y - 20);
            mSprites[7]->Move(now+250, now+320, mSprites[7]->X, mSprites[7]->Y - 20,
                                mSprites[7]->X, mSprites[7]->Y - 30);
            mSprites[7]->Move(now+350, now+350, mSprites[7]->X, mSprites[7]->Y);

            //if it was hit within the window for any amount of points
            //if (delta < DifficultyManager::HitWindow) {
                ++mTicksHit;

                IncreaseScore(SCORE_TICK_30, false, true);
                AudioManager::Engine().PlayHitSound(mSound);
            //} else {
            //    IncreaseScore(SCORE_COMBOBREAK, true, true);
            //}
        }
		
		if (mChannel == -1)
			mChannel = AudioManager::Engine().PlaySliderSound(mSound);

        OOUInt comboNumberSpriteId = mSprites.size()-1;
        mSprites[comboNumberSpriteId]->Transform(TR_FADE, now, now+200, 150, 35);
        mSprites[comboNumberSpriteId]->Transform(TR_FADE, now+200, now+270, 35, 0);
        mSprites[comboNumberSpriteId]->Scale(now, now+150, 1, 1.3);
        mSprites[comboNumberSpriteId]->Scale(now+150, now+270, 1.3, 1.7);
    }
}

void HitSlider::OnTouch(const touchPosition& touch) {
	OOTime now = GameClock::Clock().Time();
	auto delta = (OOTime) MathHelper::Abs(mTime - now);
	
	if (InBounds(touch.px, touch.py) && now >= mTime && now <= mEndTime + DifficultyManager::HitWindow) {
		//if we're touching it without tapping it, then we missed the slider start
		if (delta < DifficultyManager::HitWindow && !fStarted) {
			fStarted = true;
			//IncreaseScore(SCORE_MISS, true, true);

            OOUInt comboNumberSpriteId = mSprites.size() - 1;
            mSprites[comboNumberSpriteId]->Transform(TR_FADE, now, now+200, 150, 35);
            mSprites[comboNumberSpriteId]->Transform(TR_FADE, now+200, now+270, 35, 0);
            mSprites[comboNumberSpriteId]->Scale(now, now+150, 1, 1.3);
            mSprites[comboNumberSpriteId]->Scale(now+150, now+270, 1.3, 1.7);
		}
		
		mSprites[1]->Transform(TR_FADE, now, mEndTime, 255, 255);
		fTouching = true;
		//mTimeLast = now; //set in Update()
		
		if (mChannel == -1)
			mChannel = AudioManager::Engine().PlaySliderSound(mSound);
	} else {
		//touching screen but not sprite is the same as not touching at all
		OnTouchUp(touch);
	}
}

void HitSlider::OnTouchUp(const touchPosition& touch) {
	OOTime now = GameClock::Clock().Time();

    if (!mCreatedFadeoutTransform) {
        mSprites[1]->Transform(TR_FADE, now, mEndTime, 0, 0);
        mCreatedFadeoutTransform = true;
    }

	if (mChannel != -1) {
		AudioManager::Engine().StopChannel(mChannel);
		mChannel = -1;
	}
	
	fTouching = false;
}

void HitSlider::Hit() {
	if (mTicksHit == 0) {
		IncreaseScore(SCORE_MISS);
		IncreaseScore(SCORE_MISS, true, false, mSecondaryScoreSpriteId); //miss shows on both slider start and end
		
		//fade hitcircles
		for (OOUInt i=3; i<=6; ++i) {
			mSprites[i]->Transform(TR_FADE, mEndTime, mEndTime+100, 255, 0);
		}

        OOUInt comboNumberSpriteId = mSprites.size() - 1;
        mSprites[comboNumberSpriteId]->Transform(TR_FADE, mEndTime, mEndTime+100, 255, 0);
	}
	else
	{
		if (mTicksHit == mTicksTarget) {
			IncreaseScore(SCORE_300);
		} else if (mTicksHit >= mTicksTarget>>1) { // at least half were hit
			IncreaseScore(SCORE_100);
		} else if (mTicksHit > 0) {
			IncreaseScore(SCORE_50);
		}
		
		//animate hitcircles
		for (OOUInt i=3; i<=6; ++i) {
			mSprites[i]->Transform(TR_FADE, mEndTime, mEndTime+200, 150, 35);
			mSprites[i]->Transform(TR_FADE, mEndTime+200, mEndTime+270, 35, 0);
			mSprites[i]->Scale(mEndTime, mEndTime+150, 1, 1.3);
			mSprites[i]->Scale(mEndTime+150, mEndTime+270, 1.3, 1.7);
		}

		AudioManager::Engine().PlayHitSound(mSound);
		
		if (mChannel != -1) {
			AudioManager::Engine().StopChannel(mChannel);
			mChannel = -1;
		}
	}
	
	mHit = true;
}

void HitSlider::MapSliderPath(pSprite* spr, std::vector<HitObjectPoint*>& points, OOTime time, OOTime lengthtime, OOUInt repeats) {
	OOTime timeperpoint = lengthtime / (OOTime)points.size();
	
	OOTime loops = 0;
	while (loops < repeats) {
		for (OOTime i=0; i<points.size()-1; ++i) {
			spr->Move(time+(loops*lengthtime)+(timeperpoint*i), time+(loops*lengthtime)+(timeperpoint*(i+1)), points[i]->x, points[i]->y, points[i+1]->x, points[i+1]->y);
			spr->Rotate(time+(loops*lengthtime)+(timeperpoint*i), time+(loops*lengthtime)+(timeperpoint*(i+1)), points[i]->angle, points[i+1]->angle);
		}
		++loops;
		
		if (loops < repeats) {
			for (OOTime i=0; i<points.size()-1; ++i) {
				spr->Move(time+(loops*lengthtime)+(timeperpoint*i), time+(loops*lengthtime)+(timeperpoint*(i+1)), points[points.size()-i-1]->x, points[points.size()-i-1]->y, points[points.size()-i-2]->x, points[points.size()-i-2]->y);
				spr->Rotate(time+(loops*lengthtime)+(timeperpoint*i), time+(loops*lengthtime)+(timeperpoint*(i+1)), points[points.size()-i-1]->angle+180, points[points.size()-i-2]->angle+180);
			}
			++loops;
		}
	}
}
