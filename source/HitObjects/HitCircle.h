#include <SDL.h>
#include "HitObject.h"

#ifndef __HITCIRCLE_H__
#define __HITCIRCLE_H__

class HitCircle : public HitObject
{
	public:
		HitCircle(int32_t x, int32_t y, int32_t time, HitObjectType type, HitObjectSound sound, bool combo);
		
		bool InBounds(int32_t x, int32_t y);
		
		void OnTouchDown(const touchPosition& touch);
		
		void Hit();
};

#endif


