#pragma once

#include "cstdint"
#include <SDL_timer.h>
#include "types.h"

class GameClock
{
	public:
        [[nodiscard]] OOTime Time() const;
		
		void Update();
		void SkipTo(OOTime time);
		void Reset();
        void Reset(OOTime to);
        void Pause();
        void Resume();
		
		static GameClock& Clock() { return sClock; }

	protected:
        OOTime mTime;
        OOTime mLastUpdateAt;

        bool mPaused = false;
		
		static GameClock sClock;

	private:
		GameClock();
		~GameClock() = default;
};
