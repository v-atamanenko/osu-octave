#include "Beatmap.h"

bool Beatmap::LoadEntryData(const std::string &filename, const std::string &basedir, BeatmapEntry& bm) {
    std::ifstream file(basedir  + "/" + filename);
    if (!file) {
        fprintf(stderr, "Couldn't read .osu file %s\n", filename.c_str());
        return false;
    }

    osuParser::OsuParser p(&file);
    p.Parse();

    if (p.mode != osuParser::gmStandard) {
        // Not an osu! map
        return false;
    }

    bm.Filename = filename;
    bm.BaseDir = basedir;
    bm.Title = p.title;
    bm.Artist = p.artist;
    bm.Creator = p.creator;
    bm.Version = p.version;
    bm.AudioFilename = basedir + "/" + p.audioFilename;

    for (const osuParser::Event& e : p.events) {
        if (e.type == osuParser::eBackground) {
            bm.BackgroundFilename = e.file;
            bm.BackgroundFilepath = basedir + "/" + e.file;
        }
    }

    bm.Checksum = md5(p.title + p.artist + p.version + std::to_string(p.beatmapID));

    std::map<std::string, Output> sr = calculateStarRating(basedir  + "/" + filename);
    bm.starRating = sr["nomod"].total;
    return true;
}

Beatmap::Beatmap(const std::string &filename, const std::string &basedir)
{
	fLoadable = false;
	
	mFilename = filename;
	mBaseDir = basedir;
    mParser = nullptr;

	//mChecksumString = "";

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

    for (const osuParser::Event& e : p.events) {
        if (e.type == osuParser::eBackground) {
            mBackgroundFilepath = mBaseDir + "/" + e.file;
            mBackgroundFilename = e.file;
        }
        if (e.type == osuParser::eBreak) {
            mBreakPoints.push_back({e.begin, e.end});
        }
    }

    mBeatmapChecksum = md5(p.title + p.artist + p.version + std::to_string(p.beatmapID));

    fLoadable = true;
	fReady = false;
}

void Beatmap::Initialize()
{
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

    DifficultyManager::DifficultyHpDrain = (uint8_t)mParser->hpDrainRate;
    DifficultyManager::DifficultyCircleSize = mParser->circleSize;
    DifficultyManager::DifficultyOverall = mParser->overallDifficulty;
    DifficultyManager::SliderMultiplier = mParser->sliderMultiplier;
    DifficultyManager::SliderTickRate = mParser->sliderTickRate;

    DifficultyManager::preempt_time_new = mParser->preemptMs;

    float hoscale = (Settings::get_float("hoscale") + 100) / 100;
    DifficultyManager::circle_size = mParser->circleSize;
    DifficultyManager::circle_size_new = (mParser->circleRadiusPx * 2) * hoscale;
    DifficultyManager::hit_window_300 = mParser->hitWindow300;
    DifficultyManager::hit_window_100 = mParser->hitWindow100;
    DifficultyManager::hit_window_50 = mParser->hitWindow50;
    DifficultyManager::hit_window = mParser->hitWindow50 * 2;
    DifficultyManager::spinner_rps = mParser->requiredRPS;
    DifficultyManager::fadeInMs = mParser->fadeInMs;

    DifficultyManager::DifficultyPeppyStars = 0; // TODO: Learn how to count peppy stars

    mStackingEnabled = Settings::get_bool("enableStacking");

    for (osuParser::TimingPoint tp : mParser->timingPoints) {
        int64_t time = tp.offset;
        double beattime = tp.adjustedMsPerBeat;
        uint8_t samplesetid = tp.sampleSet;
        if (samplesetid < 1 || samplesetid > 2) // TODO: Lear what are samplesets 0 and 3
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

void Beatmap::CleanUp()
{
	//set object back to uninitialised state
	fReady = false;
	delete mParser;
    mParser = nullptr;
}

Beatmap::~Beatmap()
{
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
	while (mHitObjectRead < mHitObjectCount && mNextObjectTime < GameClock::Clock().Time() + 3000)
	{
		HitObject* object;
		
		//all coordinates are s16 in file but s32 in RAM
		
		HitObjectType type = mNextObjectType;
		int32_t x = mNextObjectX;
		int32_t y = mNextObjectY;
		HitObjectSound sound = mNextObjectSound;
        mForceNewCombo = false;
		
		switch (type) //ignore HIT_COMBO
        {
			case HIT_NORMAL:
			{
				object = new HitCircle(x, y, mNextObjectTime, type, sound, mNextObjectCombo, mNextObjectNumberInCombo);
				break;
			}
			
			case HIT_SLIDER:
			{
                osuParser::HitObject ho = mParser->hitObjects.at(mHitObjectRead);

				uint32_t repeats = ho.slider.nRepeats;
                uint32_t lengthtime = ho.slider.durationPerRepeat;

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
                /*printf("Pushing points:\n");*/
                for (osuParser::CurvePoint cp : ho.slider.curvePoints) {
                    /*printf("x: %i, y: %i;\n", cp.x, cp.y);*/
                    pts.emplace_back(cp.x, cp.y);
                }
                OsuSliderCurve *m_curve = OsuSliderCurve::createCurve(curveType, pts, ho.slider.length);

                std::vector<Vector2> screenPoints = m_curve->getPoints();

                /*
                printf("Got points:\n");
                printf("x: %f, y: %f;\n", screenPoints.at(0).x, screenPoints.at(0).y);
                printf("x: %f, y: %f;\n", screenPoints.at((int)floor(screenPoints.size()/2)).x, screenPoints.at((int)floor(screenPoints.size()/2)).y);
                printf("x: %f, y: %f;\n", screenPoints.at(screenPoints.size()-1).x, screenPoints.at(screenPoints.size()-1).y);
                */

                int32_t lastAngle = 0;
                for (int i = 0; i < screenPoints.size(); i++) {
                    auto* tPoint = new HitObjectPoint();
                    tPoint->x = (int32_t)round(screenPoints[i].x);
                    tPoint->y = (int32_t)round(screenPoints[i].y);

                    float ballAngle;
                    if (i+1 < screenPoints.size()) {
                        ballAngle = rad2deg( atan2(screenPoints[i+1].y - screenPoints[i].y, screenPoints[i+1].x - screenPoints[i].x) );
                    } else {
                        ballAngle = rad2deg( atan2(screenPoints[i].y - screenPoints[i-1].y, screenPoints[i].x - screenPoints[i-1].x) );
                    }

                    tPoint->angle = ballAngle;

                    points.push_back(tPoint);
                }

                std::vector<HitObjectPoint*> ticks;
                float msPerBeat = BeatmapElements::Element().GetTimingPoint(mNextObjectTime).BeatTime;
                float beatsPerSlider = (float)lengthtime / msPerBeat;
                int ticksPerSlider = floor((float)DifficultyManager::SliderTickRate * beatsPerSlider);

                if (ticksPerSlider > 0) {
                    int divider = floor((screenPoints.size())/(ticksPerSlider+1)) - 1;
                    for (int i = 0, j = divider; i < ticksPerSlider; i++, j+=divider) {
                        auto* tPoint = new HitObjectPoint();
                        tPoint->x = (int32_t)round(screenPoints[j].x);
                        tPoint->y = (int32_t)round(screenPoints[j].y);
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
				int64_t endtime = ho.spinner.end;
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
		if (mHitObjectRead < mHitObjectCount)
		{
			ReadNextObject();
			object->SetPostCreateOptions(mForceNewCombo || mNextObjectCombo, mNextObjectX, mNextObjectY);

            // Hacky way to do stacking
            if (type != HitObjectType::HIT_SPINNER && mNextObjectType != HitObjectType::HIT_SLIDER && mStackingEnabled) {
                const int span = floor(DifficultyManager::circle_size_new / 12);
                const int margin = 3;

                int32_t local_x = object->mX;
                int32_t local_y = object->mY;
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
		}
		else
		{
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
