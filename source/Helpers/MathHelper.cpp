#include <cstdint>
#include <cmath>
#include "MathHelper.h"

uint64_t MathHelper::mSeed = 0;

// fast_rand() & fast_srand()
// Authors: Alex Dzyoba, Hang Yan

inline void fast_srand( uint64_t seed ) {
    MathHelper::mSeed = seed;
}

//fastrand routine returns one integer, similar output value range as C lib.
inline uint32_t fastrand() {
    MathHelper::mSeed = (214*MathHelper::mSeed+2531011);
    return (MathHelper::mSeed>>16)&0x7FFF;
}

uint32_t MathHelper::Random(uint32_t min, uint32_t max) {
    fast_srand((uint64_t)time(nullptr));
    return (fastrand() % (max-min) + min);
}

double MathHelper::CosineInterpolate(double y1, double y2, double mu) {
    double mu2;

    mu2 = (1-cos(mu*PI))/2;
    return(y1*(1-mu2)+y2*mu2);
}
