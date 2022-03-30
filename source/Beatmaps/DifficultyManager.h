#pragma once

#include "types.h"

class DifficultyManager
{
	public:
		static OOFloat HP;
		static OOFloat CS;
		static OOFloat OD;
		static OOFloat SliderMultiplier;
		static OOFloat SliderTickRate;

        // Diameter of a circle based on CS in osu! pixels
        static OOFloat CircleDiameterPx;

        static OOTime PreemptMs;
        static OOTime FadeInMs;

        static OOTime HitWindow300;
        static OOTime HitWindow100;
        static OOTime HitWindow50;
        static OOTime HitWindow;

        static OOFloat RequiredRPS;

        static OOFloat GetMissHpDrain() { return (-10) * HP; }
		static OOFloat GetHpDrainRate() { return 1.f / (110.f + ((-10) * HP)); }
};
