#include <cstdint>
#include <cmath>
#include "MathHelper.h"

#define PI 3.1415926535

uint16_t MathHelper::mSeed = 3246;

uint16_t MathHelper::Random(uint16_t min, uint16_t max)
{
	mSeed= 1664525*mSeed+1013904223;
	return (mSeed % (max-min) + min);
}

double MathHelper::CosineInterpolate(double y1, double y2, double mu) {
    double mu2;

    mu2 = (1-cos(mu*PI))/2;
    return(y1*(1-mu2)+y2*mu2);
}

