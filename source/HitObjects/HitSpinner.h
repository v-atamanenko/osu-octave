#pragma once

#include <cmath>
#include "HitObject.h"
#include "Graphics/pText.h"

class HitSpinner : public HitObject
{
	public:
		HitSpinner(OOTime time, OOTime endtime, HitObjectSound sound, bool combo, OOInt number_in_combo);
		~HitSpinner();
		
		void Update();
		
		void OnTouchDown(const touchPosition& touch);
		void OnTouch(const touchPosition& touch);
		
		void Hit();
	
	protected:
        OOInt GetAngle(OOInt x, OOInt y);

        OOInt mLastAngle;
		bool fSpinning;
        OOInt mDirection, mZeroPoint;
		OOFloat mTotalRotation;
        OOFloat mReallyTotalRotation = 0; // yeah,sorry about that
		OOUInt mCurrentRotation;
        OOUInt mTotalSpins;
        OOUInt mRequiredSpins;
        pText* mRPMIndicator;
        OOFloat mRPM = 0;
        OOTime mLastUpdateAt = -1;
		
		OOInt mChannel;
        OOInt mChannelBonus;
};
