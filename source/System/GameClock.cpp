#include "GameClock.h"
#include "cstdio"
GameClock GameClock::sClock;

GameClock::GameClock()
{
	/*
	TIMER1_DATA = 32022;
	TIMER1_CR = TIMER_DIV_1;
	
	SkipTo(0);
	*/
	mTime = 0;
	mFraction = 0;
}

int GameClock::Time()
{
	return mTime;
}

void GameClock::Update()
{
	mTime += kTimeInterval;
	mFraction += kFractionInterval;
	
	if (mFraction > 1000000000)
	{
		++mTime;
        mFraction -= 1000000000;
	}
}


/*
void GameClock::CacheTime()
{
	//lock the values when reading
	TIMER2_CR &= ~TIMER_ENABLE;
	register u16 d2 = TIMER2_DATA;
	register u16 d3 = TIMER3_DATA;
	TIMER2_DATA = d2;
	TIMER2_CR |= TIMER_ENABLE;
	
	mTime = d2 + (d3 << 16);
}
*/

void GameClock::SkipTo(int time)
{
	mTime = time;
	mFraction = 0;
	/*
	TIMER2_DATA = (u16)(time & 0xFFFF);
	TIMER3_DATA = (u16)(time >> 16);
	
	TIMER1_CR &= ~TIMER_ENABLE;
	
	TIMER2_CR = 0;
	TIMER2_CR = TIMER_ENABLE | TIMER_CASCADE;
	
	TIMER3_CR = 0;
	TIMER3_CR = TIMER_ENABLE | TIMER_CASCADE;
	
	TIMER1_CR |= TIMER_ENABLE;
	*/
}

void GameClock::Reset()
{
	SkipTo(0);
}


