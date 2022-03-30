#pragma once

#include <SDL.h>
#include "HitObject.h"

class HitCircle : public HitObject
{
	public:
		HitCircle(OOInt x, OOInt y, OOTime time, HitObjectType type, HitObjectSound sound, bool combo, OOInt number_in_combo);
		
		bool InBounds(OOInt x, OOInt y);
		
		void OnTouchDown(const touchPosition& touch);
		
		void Hit();
};
