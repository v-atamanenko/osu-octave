#include <cstdio>
#include "System/GameClock.h"

#ifndef __TRANSFORMATION_H__
#define __TRANSFORMATION_H__

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
		Transformation(TransformType type, int32_t starttime, int32_t endtime, int32_t startvalue, int32_t endvalue);
		void Update();
		int32_t Value() const { return currentvalue; }
		bool Active();
		TransformType Type() const { return type; }
		
		bool Finished() const { return GameClock::Clock().Time() > endtime; }
	
	protected:
		TransformType type;
		
		bool active, lastactive;
		
		int32_t starttime, endtime, totaltime;
		
		int32_t startvalue, endvalue, currentvalue;
		int32_t totalvalue;
};


#endif

