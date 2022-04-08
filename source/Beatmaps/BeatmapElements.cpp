#include "BeatmapElements.h"
#include "DataStorage/Skins.h"

BeatmapElements BeatmapElements::sInstance;

const TimingPoint& BeatmapElements::GetTimingPoint(OOTime time) {
	OOInt i;
	for (
		i = (OOInt)mTimingPoints.size() - 1;
		i > 0 && mTimingPoints[i].Time > time;
		--i
	);
	return mTimingPoints[i];
}

const TimingPoint& BeatmapElements::GetTimingPoint() {
	return GetTimingPoint(GameClock::Clock().Time());
}

bool BeatmapElements::IsBreak() {
	for (auto & mBreakPoint : mBreakPoints) {
		if (GameClock::Clock().Time() > mBreakPoint.StartTime && GameClock::Clock().Time() < mBreakPoint.EndTime)
			return true;
	}
	return false;
}

SDL_Color BeatmapElements::GetNextColour() {
	//backwards compatibility - increase counter BEFORE returning
	if (++mCurrentColour >= mColours.size())
		mCurrentColour = 0;
	
	return mColours[mCurrentColour];
}

void BeatmapElements::AddTimingPoint(OOTime time, OOFloat beattime, OOUShort samplesetid) {
	TimingPoint tPoint = {
		time,
		beattime,
		samplesetid
	};
	
	mTimingPoints.push_back(tPoint);
}

void BeatmapElements::AddBreakPoint(OOTime start, OOTime end) {
	BreakPoint bPoint = {
		start,
		end
	};
	
	mBreakPoints.push_back(bPoint);
}

void BeatmapElements::ResetColours(bool fill) {
	mCurrentColour = 0;

    if (!fill) {
        mColours.clear();
    }

    if (mColours.empty() && fill) {
        int skin_colors_added = 0;
        if (Skins::get_options().TintCustomColor1.r != 0 ||
            Skins::get_options().TintCustomColor1.g != 0 ||
            Skins::get_options().TintCustomColor1.b != 0 ||
            Skins::get_options().TintCustomColor1.a != 0) {
            mColours.push_back(Skins::get_options().TintCustomColor1);
            skin_colors_added++;
        }
        if (Skins::get_options().TintCustomColor2.r != 0 ||
            Skins::get_options().TintCustomColor2.g != 0 ||
            Skins::get_options().TintCustomColor2.b != 0 ||
            Skins::get_options().TintCustomColor2.a != 0) {
            mColours.push_back(Skins::get_options().TintCustomColor2);
            skin_colors_added++;
        }
        if (Skins::get_options().TintCustomColor3.r != 0 ||
            Skins::get_options().TintCustomColor3.g != 0 ||
            Skins::get_options().TintCustomColor3.b != 0 ||
            Skins::get_options().TintCustomColor3.a != 0) {
            mColours.push_back(Skins::get_options().TintCustomColor3);
            skin_colors_added++;
        }
        if (Skins::get_options().TintCustomColor4.r != 0 ||
            Skins::get_options().TintCustomColor4.g != 0 ||
            Skins::get_options().TintCustomColor4.b != 0 ||
            Skins::get_options().TintCustomColor4.a != 0) {
            mColours.push_back(Skins::get_options().TintCustomColor4);
            skin_colors_added++;
        }

        if (skin_colors_added == 0) {
            mColours.push_back(SDL_Color({229, 198, 254}));
            mColours.push_back(SDL_Color({176, 225, 254}));
            mColours.push_back(SDL_Color({254, 218, 244}));
            mColours.push_back(SDL_Color({223, 255, 250}));
        }
    }
}

// Somehow SDL color modulations doesn't like 0/255 values.
inline SDL_Color AdjustColor(SDL_Color c) {
    SDL_Color y;
    y.r = std::min(254, c.r + 1);
    y.g = std::min(254, c.g + 1);
    y.b = std::min(254, c.b + 1);
    return y;
}

void BeatmapElements::AddColor(SDL_Color c) {
    mColours.push_back(AdjustColor(c));
}
