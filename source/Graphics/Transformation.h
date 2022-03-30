#pragma once

#include <cstdio>
#include "System/GameClock.h"

typedef enum {
	TR_FADE,
	TR_MOVEX,
	TR_MOVEY,
	TR_SCALEX,
	TR_SCALEY,
	TR_ROTATE,
	TR_KILL
} TransformType;

class Transformation
{
	public:
		Transformation(TransformType type, OOTime starttime, OOTime endtime, int32_t startvalue, int32_t endvalue);
		void Update();

		[[nodiscard]] OOInt Value() const { return currentvalue; }
		[[nodiscard]] TransformType Type() const { return type; }

        bool Active();
		[[nodiscard]] bool Finished() const { return GameClock::Clock().Time() > endtime; }
	
	protected:
		TransformType type;
		
		bool active, lastactive;

        OOTime starttime, endtime, totaltime;

        OOInt startvalue, endvalue, currentvalue;
        OOInt totalvalue;
};
