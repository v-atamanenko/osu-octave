#include <cstdint>
#include <cmath>
#include "MathHelper.h"

uint32_t MathHelper::Random(uint32_t min, uint32_t max) {
    return min + (rand() % static_cast<int>(max - min + 1));
}

double MathHelper::CosineInterpolate(double y1, double y2, double mu) {
    double mu2;

    mu2 = (1-cos(mu*PI))/2;
    return(y1*(1-mu2)+y2*mu2);
}
