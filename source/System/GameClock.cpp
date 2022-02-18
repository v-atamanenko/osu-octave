#include <SDL_timer.h>
#include "GameClock.h"
GameClock GameClock::sClock;

GameClock::GameClock() {
    mStartedAt = SDL_GetTicks();
    mLastUpdateAt = SDL_GetTicks();
	mTime = 0;
}

long GameClock::Time() const {
	return mTime;
}

void GameClock::Update() {
	mTime += (SDL_GetTicks() - mLastUpdateAt);
    mLastUpdateAt = SDL_GetTicks();
}

void GameClock::SkipTo(int time) {
    mLastUpdateAt = SDL_GetTicks();
	mTime = time;
}

void GameClock::Reset() {
	SkipTo(0);
}


