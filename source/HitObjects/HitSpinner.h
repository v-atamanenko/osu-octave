#include <cmath>
#include "HitObject.h"

#ifndef __HITSPINNER_H__
#define __HITSPINNER_H__

class HitSpinner : public HitObject
{
	public:
		HitSpinner(int32_t time, int32_t endtime, HitObjectSound sound);
		~HitSpinner();
		
		void Update();
		
		void OnTouchDown(const touchPosition& touch);
		void OnTouch(const touchPosition& touch);
		
		void Hit();
	
	protected:
		int32_t GetAngle(int32_t x, int32_t y);
		
		int32_t mLastAngle;
		bool fSpinning;
		int32_t mDirection, mZeroPoint;
		float mTotalRotation;
		uint32_t mCurrentRotation;
		uint32_t mTotalSpins;
		uint32_t mRequiredSpins;
		
		int mChannel;
        int mChannelBonus;
};

#endif


