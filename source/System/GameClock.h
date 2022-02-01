#ifndef __GAMECLOCK_H__
#define __GAMECLOCK_H__

class GameClock
{
	public:
		int Time();
		
		void Update(); //calls should be synced with vblank
		void SkipTo(int time);
		void Reset();
		
		static GameClock& Clock() { return sClock; }
	
	protected:
		long mTime;
		long mFraction;
		
		static const int kTimeInterval = 16;
		static const int kFractionInterval = 666666667;
		
		static GameClock sClock;
	
	private:
		GameClock();
		~GameClock() {}
};

#endif

