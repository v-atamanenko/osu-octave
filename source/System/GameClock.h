#ifndef __GAMECLOCK_H__
#define __GAMECLOCK_H__

#include "cstdint"

class GameClock
{
	public:
		long Time() const;
		
		void Update(); //calls should be synced with vblank
		void SkipTo(int time);
		void Reset();
        void Reset(int32_t to);
		
		static GameClock& Clock() { return sClock; }
	
	protected:
        long mStartedAt;
        long mLastUpdateAt;
		long mTime;
		
		static GameClock sClock;
	
	private:
		GameClock();
		~GameClock() = default;
};

#endif

