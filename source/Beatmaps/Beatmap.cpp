#include "Beatmap.h"

Beatmap::Beatmap(const char* filename, const char* basedir)
{
	fLoadable = false;
	
	mFilename = filename;
	mBaseDir = basedir;
    mParser = nullptr;

	mChecksumString = "";

    std::ifstream file(mFilename);
    if (!file) {
        fprintf(stderr, "Couldn't read .osu file %s", mFilename.c_str());
        mValid = false;
        return;
    }

    osuParser::OsuParser p(&file);
    p.Parse();

    mTitle = p.title;
    mArtist = p.artist;
    mCreator = p.creator;
    mVersion = p.version;
    mAudioFilename = mBaseDir + "/" + p.audioFilename;

    for (const osuParser::Event& e : p.events) {
        if (e.type == osuParser::eBackground) {
            mBackgroundFilename = mBaseDir + "/" + e.file;
        }
    }

    if (p.mode != osuParser::gmStandard) {
        mValid = false;
    }

    fLoadable = true;
	fReady = false;
}

void Beatmap::Initialize()
{
    if (fReady) {
        return;
    }

    if (!fLoadable) {
        fprintf(stderr, "Cannot load .osu file %s", mFilename.c_str());
        return;
    }

    //chdir(mBaseDir.c_str());
    std::ifstream file(mFilename);
    if (!file) {
        fprintf(stderr, "Couldn't read .osu file %s", mFilename.c_str());
        return;
    }

    mParser = new osuParser::OsuParser(&file);
    mParser->Parse();

    mTitle = mParser->title;
    mArtist = mParser->artist;
    mCreator = mParser->creator;
    mVersion = mParser->version;
    mAudioFilename = mBaseDir + "/" + mParser->audioFilename;

    AudioManager::Engine().MusicLoad(mAudioFilename);

    DifficultyManager::DifficultyHpDrain = (uint8_t)mParser->hpDrainRate;
    DifficultyManager::DifficultyCircleSize = (uint8_t)mParser->circleSize;
    DifficultyManager::DifficultyOverall = (uint8_t)mParser->overallDifficulty;
    DifficultyManager::SliderMultiplier = (uint8_t)mParser->sliderMultiplier;
    DifficultyManager::SliderTickRate = (uint8_t)mParser->sliderTickRate;

    DifficultyManager::DifficultyPeppyStars = 0; // TODO: Learn how to count peppy stars
    DifficultyManager::DifficultyEyupStars = 0; // TODO: Learn how to count eyup stars

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
	GraphicsManager::Graphics().LoadBeatmapBackground(mBackgroundFilename);

	mHitObjectCount = mParser->hitObjects.size();
	mHitObjectRead = 0;
	mLastObjectEndTime = 0;
	mForceNewCombo = true;

    //read ahead
	ReadNextObject();
	mFirstObjectTime = mNextObjectTime;

    //the time to skip to is the first object - 8 beats
    mSkipTime = MathHelper::Max(0, mNextObjectTime - (int32_t)(BeatmapElements::Element().GetTimingPoint(mNextObjectTime).BeatTime*8));

    //strangely calling this in ctor of BeatmapElements causes game to not load :/
	BeatmapElements::Element().ResetColours(true);

	//now we can play this map
	fReady = true;
}

void Beatmap::Buffer(std::list<HitObject*>& hitObjectList)
{
	if (!fReady)
	{
		printf("\x1b[0;std::listt ready to buffer");
		return;
	}

    //we buffer objects to 10 seconds ahead
	while (mHitObjectRead < mHitObjectCount && mNextObjectTime < GameClock::Clock().Time() + 3000)
	{
		HitObject* object;
		
		//all coordinates are s16 in file but s32 in RAM
		
		HitObjectType type = mNextObjectType;
		int32_t x = mNextObjectX;
		int32_t y = mNextObjectY;
		HitObjectSound sound = mNextObjectSound;
		
		if (mForceNewCombo)
		{
            mNextObjectIsNewCombo = true;
			mForceNewCombo = false;
		}
		
		switch (type) //ignore HIT_COMBO
		{
			case HIT_NORMAL:
			{
				object = new HitCircle(x, y, mNextObjectTime, type, sound);
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

                for (auto & screenPoint : screenPoints) {
                    auto* tPoint = new HitObjectPoint();
                    tPoint->x = osuPixelsXtoScreenX(round(screenPoint.x));
                    tPoint->y = osuPixelsYtoScreenY(round(screenPoint.y));
                    points.push_back(tPoint);
                }

                std::vector<HitObjectPoint*> ticks;
                //FIXME: Add ticks

                if (points.empty()) {
                    fprintf(stderr, "[ERROR]: Zero points in slider. Skipping.\n");
                    ticks.clear();
                    ++mHitObjectRead;
                    continue;
                }
				
				object = new HitSlider(x, y, mNextObjectTime, lengthtime, points, ticks, repeats, type, sound);

                points.clear();
                ticks.clear();

				break;
			}
			
			case HIT_SPINNER:
			{
                osuParser::HitObject ho = mParser->hitObjects.at(mHitObjectRead);
				int64_t endtime = ho.spinner.end;
				object = new HitSpinner(mNextObjectTime, endtime, sound);
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
			object->SetPostCreateOptions(mNextObjectIsNewCombo || mForceNewCombo, mNextObjectX, mNextObjectY);
		}
		else
		{
			object->SetPostCreateOptions(true, 0, 0);
		}
	}
}

void Beatmap::ReadNextObject()
{
    osuParser::HitObject ho = mParser->hitObjects.at(mHitObjectRead);
	mNextObjectTime = ho.time;
    mNextObjectType = (HitObjectType)ho.type;

    mNextObjectIsNewCombo = ho.isNewCombo;
    mNextObjectX = osuPixelsXtoScreenX(ho.x); //s32 x
    mNextObjectY = osuPixelsYtoScreenY(ho.y); //s32 y
    mNextObjectSound = (HitObjectSound)ho.soundMask;
}

/*
std::string& Beatmap::BeatmapChecksum() {
	std::string checksum = "";
	//TODO: figure out some way to uniquely identify every beatmap without it being too intensive
	return nullptr;
}*/