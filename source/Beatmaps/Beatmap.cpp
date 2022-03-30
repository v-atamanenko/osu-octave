#include "Beatmap.h"

Beatmap::Beatmap(const std::string &filename, const std::string &basedir) {
	fLoadable = false;
	
	mFilename = filename;
	mBaseDir = basedir;
    mParser = nullptr;

    std::ifstream file(mBaseDir  + "/" + mFilename);
    if (!file) {
        fprintf(stderr, "Couldn't read .osu file %s", mFilename.c_str());
        fLoadable = false;
        mValid = false;
        return;
    }

    osuParser::OsuParser p(&file);
    p.Parse();

    if (p.mode != osuParser::gmStandard) {
        // Not an osu! map
        mValid = false;
        return;
    }

    mTitle = p.title;
    mArtist = p.artist;
    mCreator = p.creator;
    mVersion = p.version;
    mAudioFilename = mBaseDir + "/" + p.audioFilename;
    mAudioLeadIn = p.audioLeadIn;
    mBreakDurationTotal = 0;

    for (const osuParser::Event& e : p.events) {
        if (e.type == osuParser::eBackground) {
            mBackgroundFilepath = mBaseDir + "/" + e.file;
            mBackgroundFilename = e.file;
        }
        if (e.type == osuParser::eBreak) {
            mBreakPoints.push_back({e.begin, e.end});
            mBreakDurationTotal += (e.end - e.begin);
        }
    }

    mBeatmapChecksum = md5(p.title + p.artist + p.version + std::to_string(p.beatmapID));

    fLoadable = true;
	fReady = false;
}

void Beatmap::Initialize() {
    if (fReady) {
        return;
    }

    std::ifstream file(mBaseDir + "/" + mFilename);
    if (!file) {
        fprintf(stderr, "Couldn't read .osu file %s", mFilename.c_str());
        mValid = false;
        return;
    }

    mParser = new osuParser::OsuParser(&file);
    mParser->Parse();

    mTitle = mParser->title;
    mArtist = mParser->artist;
    mCreator = mParser->creator;
    mVersion = mParser->version;
    mAudioFilename = mBaseDir + "/" + mParser->audioFilename;
    mAudioLeadIn = mParser->audioLeadIn;

    AudioManager::Engine().MusicLoad(mAudioFilename);

    DifficultyManager::HP = mParser->hpDrainRate;
    DifficultyManager::CS = mParser->circleSize;
    DifficultyManager::OD = mParser->overallDifficulty;
    DifficultyManager::SliderMultiplier = mParser->sliderMultiplier;
    DifficultyManager::SliderTickRate = mParser->sliderTickRate;

    DifficultyManager::PreemptMs = mParser->preemptMs;

    OOFloat hoscale = (Settings::get_float("hoscale") + 100) / 100;
    DifficultyManager::CircleDiameterPx = (mParser->circleRadiusPx * 2) * hoscale;
    DifficultyManager::HitWindow300 = mParser->hitWindow300;
    DifficultyManager::HitWindow100 = mParser->hitWindow100;
    DifficultyManager::HitWindow50 = mParser->hitWindow50;
    DifficultyManager::HitWindow = mParser->hitWindow50 * 2;
    DifficultyManager::RequiredRPS = mParser->requiredRPS;
    DifficultyManager::FadeInMs = mParser->fadeInMs;

    mStackingEnabled = Settings::get_bool("enableStacking");

    for (osuParser::TimingPoint tp : mParser->timingPoints) {
        OOTime time = tp.offset;
        OOFloat beattime = tp.adjustedMsPerBeat;
        OOUShort samplesetid = tp.sampleSet;
        if (samplesetid < 1 || samplesetid > 2) // TODO: Learn what are samplesets 0 and 3
            samplesetid = 1;
        BeatmapElements::Element().AddTimingPoint(time, beattime, samplesetid);
    }

    for (const osuParser::Event& e : mParser->events) {
        if (e.type == osuParser::eBreak) {
            BeatmapElements::Element().AddBreakPoint(e.begin, e.end);
        }
    }

    BeatmapElements::Element().ResetColours(false);
    for (const osuParser::RGBAColor& c : mParser->colors) {
        BeatmapElements::Element().AddColor({c.r, c.g, c.b});
    }
}

void Beatmap::CleanUp() {
	//set object back to uninitialised state
	fReady = false;
	delete mParser;
    mParser = nullptr;
}

Beatmap::~Beatmap() {
	delete mParser;
}

void Beatmap::InitBG() {
    assert(mValid);

	GraphicsManager::Graphics().LoadBeatmapBackground(mBackgroundFilepath);

	mHitObjectCount = mParser->hitObjects.size();
	mHitObjectRead = 0;
	mLastObjectEndTime = mParser->hitObjects.at(mHitObjectCount-1).time;

    //read ahead
	ReadNextObject();
	mFirstObjectTime = mNextObjectTime;

    //the time to skip to is the first object - 8 beats
    mSkipTime = MathHelper::Max(0, mNextObjectTime - (int32_t)(BeatmapElements::Element().GetTimingPoint(mNextObjectTime).BeatTime*8));

    BeatmapElements::Element().ResetColours(true);

	//now we can play this map
	fReady = true;
}

void Beatmap::Buffer(std::list<HitObject*>& hitObjectList)
{
    // The method must never be called if the beatmap failed to initialize.
    assert(fReady);
    assert(mParser != nullptr);

    //we buffer objects to 10 seconds ahead
	while (mHitObjectRead < mHitObjectCount && mNextObjectTime < GameClock::Clock().Time() + 4000) {
		HitObject* object;

		HitObjectType type = mNextObjectType;
        OOInt x = mNextObjectX;
        OOInt y = mNextObjectY;
		HitObjectSound sound = mNextObjectSound;
        mForceNewCombo = false;
		
		switch (type) { //ignore HIT_COMBO
			case HIT_NORMAL:
			{
				object = new HitCircle(x, y, mNextObjectTime, type, sound, mNextObjectCombo, mNextObjectNumberInCombo);
				break;
			}
			
			case HIT_SLIDER:
			{
                osuParser::HitObject ho = mParser->hitObjects.at(mHitObjectRead);

				OOUShort repeats = ho.slider.nRepeats;
                OOTime lengthtime = ho.slider.durationPerRepeat;

                std::vector<HitObjectPoint*> points;

                char curveType;
                switch (ho.slider.type) {
                    case osuParser::sLinear: {
                        curveType = 'L';
                        break;
                    }
                    case osuParser::sPerfect: {
                        curveType = 'P';
                        break;
                    }
                    case osuParser::sBezier: {
                        curveType = 'B';
                        break;
                    }
                    case osuParser::sCatmull: {
                        curveType = 'C';
                        break;
                    }
                }

                std::vector<Vector2> pts;
                for (osuParser::CurvePoint cp : ho.slider.curvePoints) {
                    pts.emplace_back(cp.x, cp.y);
                }
                OsuSliderCurve *m_curve = OsuSliderCurve::createCurve(curveType, pts, (float)ho.slider.length);

                std::vector<Vector2> screenPoints = m_curve->getPoints();
                for (int i = 0; i < screenPoints.size(); i++) {
                    auto* tPoint = new HitObjectPoint();
                    tPoint->x = (OOInt)round(screenPoints[i].x);
                    tPoint->y = (OOInt)round(screenPoints[i].y);

                    OOFloat ballAngle;
                    if (i+1 < screenPoints.size()) {
                        ballAngle = rad2deg( atan2(screenPoints[i+1].y - screenPoints[i].y, screenPoints[i+1].x - screenPoints[i].x) );
                    } else {
                        ballAngle = rad2deg( atan2(screenPoints[i].y - screenPoints[i-1].y, screenPoints[i].x - screenPoints[i-1].x) );
                    }

                    tPoint->angle = (OOInt)round(ballAngle);
                    points.push_back(tPoint);
                }

                std::vector<HitObjectPoint*> ticks;
                OOFloat msPerBeat = BeatmapElements::Element().GetTimingPoint(mNextObjectTime).BeatTime;
                OOFloat beatsPerSlider = (OOFloat)lengthtime / msPerBeat;
                OOInt ticksPerSlider = floor(DifficultyManager::SliderTickRate * beatsPerSlider);

                if (ticksPerSlider > 0) {
                    OOInt divider = (OOInt)floor( (OOFloat)(screenPoints.size()) / (OOFloat)(ticksPerSlider + 1)) - 1;
                    for (int i = 0, j = divider; i < ticksPerSlider; i++, j+=divider) {
                        auto* tPoint = new HitObjectPoint();
                        tPoint->x = (OOInt)round(screenPoints[j].x);
                        tPoint->y = (OOInt)round(screenPoints[j].y);
                        ticks.push_back(tPoint);
                    }
                }

                if (points.empty()) {
                    fprintf(stderr, "[ERROR]: Zero points in slider. Skipping.\n");
                    ticks.clear();
                    ++mHitObjectRead;
                    continue;
                }

				object = new HitSlider(x, y, mNextObjectTime, lengthtime, points, ticks, repeats, type, sound, mNextObjectCombo, mNextObjectNumberInCombo);

                points.clear();
                ticks.clear();

				break;
			}
			
			case HIT_SPINNER:
			{
                osuParser::HitObject ho = mParser->hitObjects.at(mHitObjectRead);
                OOTime endtime = ho.spinner.end;
				object = new HitSpinner(mNextObjectTime, endtime, sound, mNextObjectCombo, mNextObjectNumberInCombo);
                mForceNewCombo = true;
				break;
			}
		}
		
		//track when the beatmap has ended
		mLastObjectEndTime = object->GetEndTime();
		
		//add to list
		hitObjectList.push_back(object);
		
		++mHitObjectRead;
		if (mHitObjectRead < mHitObjectCount) {
			ReadNextObject();
			object->SetPostCreateOptions(mForceNewCombo || mNextObjectCombo, mNextObjectX, mNextObjectY);

            // Hacky way to do stacking
            if (type != HitObjectType::HIT_SPINNER && mNextObjectType != HitObjectType::HIT_SLIDER && mStackingEnabled) {
                const OOInt span = floor(DifficultyManager::CircleDiameterPx / 12);
                const OOInt margin = 3;

                OOInt local_x = object->mX;
                OOInt local_y = object->mY;
                if (mStackSize > 0) {
                    local_x -= span*mStackSize;
                    local_y -= span*mStackSize;
                }
                bool exact_match = (abs(mNextObjectX - local_x) < margin) && (abs(mNextObjectY - local_y) < margin);
                bool slider_match = false;
                if (type == HitObjectType::HIT_SLIDER) {
                    slider_match = abs(((HitSlider*)object)->mEndPoint.x - mNextObjectX) < margin && abs(((HitSlider*)object)->mEndPoint.y - mNextObjectY) < margin;
                }
                if (slider_match || exact_match) {
                    mStackSize++;

                    mNextObjectX += span*mStackSize;
                    mNextObjectY += span*mStackSize;
                } else {
                    mStackSize = 0;
                }
            } else {
                mStackSize = 0;
            }
		} else {
			object->SetPostCreateOptions(true, 0, 0);
		}
	}
}

void Beatmap::ReadNextObject() {
    osuParser::HitObject ho = mParser->hitObjects.at(mHitObjectRead);
    mNextObjectCombo = ho.isNewCombo || (mHitObjectRead == 0);
    mNextObjectSound = (HitObjectSound)ho.soundMask;
    mNextObjectType = (HitObjectType)ho.type;
    mNextObjectTime = ho.time;
    mNextObjectX = ho.x;
    mNextObjectY = ho.y;
    mNextObjectNumberInCombo++;
    if (mNextObjectCombo) {
        mNextObjectNumberInCombo = 1;
    }
    if (mNextObjectNumberInCombo > 9) {
        mNextObjectNumberInCombo = 1;
        mNextObjectCombo = true;
    }
}

std::string& Beatmap::BeatmapChecksum() {
    assert(!mBeatmapChecksum.empty());
    return mBeatmapChecksum;
}
