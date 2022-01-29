#include <cstdint>
#include "MathHelper.h"

uint16_t MathHelper::mSeed = 3246;

uint16_t MathHelper::Random(uint16_t min, uint16_t max)
{
	mSeed= 1664525*mSeed+1013904223;
	return (mSeed % (max-min) + min);
}


