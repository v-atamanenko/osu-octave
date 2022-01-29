//#include <math.h>

#ifndef __MATHHELPER_H__
#define __MATHHELPER_H__

class MathHelper
{
	public:
		static uint32_t		Abs(int32_t value) { return (value > 0 ? value : -value); }
		static int32_t		Max(int32_t value1, int32_t value2) { return (value1 > value2 ? value1 : value2); }
		static int32_t		Min(int32_t value1, int32_t value2) { return (value1 < value2 ? value1 : value2); }
		static int32_t		Sgn(int32_t value) { return (value == (int32_t)Abs(value) ? 1 : -1); }
		static float	Frc(float value) { return value - (int)value; }
		
		static uint16_t Random(uint16_t min, uint16_t max);
	
	protected:
		static uint16_t mSeed;
};

#endif



