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
    if (!mPaused)
        mTime += (SDL_GetTicks() - mLastUpdateAt);
    mLastUpdateAt = SDL_GetTicks();
}

void GameClock::SkipTo(int time) {
    mLastUpdateAt = SDL_GetTicks();
	mTime = time;
}

void GameClock::Reset() {
	SkipTo(0);
    mPaused = false;
}

void GameClock::Reset(int32_t to) {
    SkipTo(to);
    mPaused = false;
}

void GameClock::Pause() {
    mPaused = true;
}

void GameClock::Resume() {
    mPaused = false;
}

