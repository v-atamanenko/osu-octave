#include "GameClock.h"

GameClock GameClock::sClock;

GameClock::GameClock() {
    mLastUpdateAt = (OOTime)SDL_GetTicks();
	mTime = 0;
}

OOTime GameClock::Time() const {
	return mTime;
}

void GameClock::Update() {
    if (!mPaused)
        mTime += ((OOTime)SDL_GetTicks() - mLastUpdateAt);
    mLastUpdateAt = (OOTime)SDL_GetTicks();
}

void GameClock::SkipTo(OOTime time) {
    mLastUpdateAt = (OOTime)SDL_GetTicks();
	mTime = time;
}

void GameClock::Reset() {
	SkipTo(0);
    mPaused = false;
}

void GameClock::Reset(OOTime to) {
    SkipTo(to);
    mPaused = false;
}

void GameClock::Pause() {
    mPaused = true;
}

void GameClock::Resume() {
    mPaused = false;
}
