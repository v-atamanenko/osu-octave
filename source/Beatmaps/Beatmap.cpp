#include "Beatmap.h"



Beatmap::Beatmap(const char* filename, const char* basedir)
{
	fLoadable = false;
	
	mFilename = filename;
	mBaseDir = basedir;
	mReader = nullptr;

	mChecksumString = "";
	
	FileReader r(filename);
	
	if (r.Ready())
	{
		//check header before processing
		char id[4] = { r.ReadInt8(), r.ReadInt8(), r.ReadInt8(), 0 };
		if (strcmp(id, "ODS") == 0)
		{
			odsver = r.ReadInt8();
			
			mTitle = r.ReadString();
			mArtist = r.ReadString();
			mCreator = r.ReadString();
			mVersion = r.ReadString();
			mAudioFilename = r.ReadString();
				
			fLoadable = true;
		} else {
		}
	}
	
	fReady = false;
}

void Beatmap::Initialize()
{
	if (!fReady)
	{
		if (!fLoadable)
		{
			printf("\x1b[0;0Hcannot load this file");
			//return;
		}
		
		chdir(mBaseDir.c_str());
		mReader = new FileReader(mFilename);
		
		//skip header
		mReader->Skip(3);
		
		odsver = mReader->ReadInt8();
		
		mTitle = mReader->ReadString();
		mArtist = mReader->ReadString();
		mCreator = mReader->ReadString();
		mVersion = mReader->ReadString();
		mAudioFilename = mReader->ReadString();

		DifficultyManager::DifficultyHpDrain = mReader->ReadInt8();
		DifficultyManager::DifficultyCircleSize = mReader->ReadInt8();
		DifficultyManager::DifficultyOverall = mReader->ReadInt8();
		DifficultyManager::SliderMultiplier = mReader->ReadFloat();
		DifficultyManager::SliderTickRate = mReader->ReadFloat();
		DifficultyManager::DifficultyHpDrainRate = mReader->ReadFloat();
		DifficultyManager::DifficultyPeppyStars = mReader->ReadInt8();
		DifficultyManager::DifficultyEyupStars = mReader->ReadFloat();

		uint32_t tpointcount = mReader->ReadVarInt();
		for (uint32_t j=0; j<tpointcount; ++j)
		{
			int32_t time = mReader->ReadInt32();
			float beattime = mReader->ReadFloat();
			uint8_t samplesetid = mReader->ReadInt8();
			
			BeatmapElements::Element().AddTimingPoint(time, beattime, samplesetid);
		}

		uint32_t breakcount = mReader->ReadVarInt();

		for (uint32_t j=0; j<breakcount; ++j)
		{
			int32_t starttime = mReader->ReadInt32();
			int32_t endtime = mReader->ReadInt32();
			
			BeatmapElements::Element().AddBreakPoint(starttime, endtime);
		}

		printf("\x1b[2J");
	}
}

void Beatmap::CleanUp()
{
	//set object back to uninitialised state
	fReady = false;
	delete mReader;
	mReader = nullptr;
}

Beatmap::~Beatmap()
{
	delete mReader;
}

void Beatmap::InitBG() {
	if (odsver > 1) {
        size_t framebuf_size = 256 * 192 * sizeof(uint16_t);
        auto* pixels = static_cast<uint16_t *>(malloc(framebuf_size));

        for (int i = 0; i < 256*192; i++) {
			*(uint16_t *) (pixels + i) = mReader->ReadInt16();
        }

        int bpp = 16;
        Uint32 Rmask, Gmask, Bmask, Amask;

        SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ABGR1555, &bpp, &Rmask, &Gmask, &Bmask, &Amask);

        SDL_Surface* surface =
                    SDL_CreateRGBSurfaceFrom(
                            pixels,
                            256,
                            192,
                            2*8,  // in bits, so bytes per pixel * 8
                            2*256, // 2 bytes per pixel * pixels per row
                            Rmask,
                            Gmask,
                            Bmask,
                            Amask
                    );

        if (surface == nullptr) {
            fprintf(stderr, "\nFailed to create surface for beatmap bg: %s\n", SDL_GetError());
        }

        GraphicsManager::Graphics().CreateTextureFromSurface(surface, TX_CURRENT_BG);
        free(pixels);
	}

	mHitObjectCount = mReader->ReadVarInt();
	mHitObjectRead = 0;
	mLastObjectEndTime = 0;
	mForceNewCombo = true;
    printf("mHitObjectCount: %i\n", mHitObjectCount);
	//read ahead
	ReadNextObject();
	mFirstObjectTime = mNextObjectTime;

    //the time to skip to is the first object - 8 beats
    mSkipTime = MathHelper::Max(0, (int32_t)mNextObjectTime - (BeatmapElements::Element().GetTimingPoint(mNextObjectTime).BeatTime*8));

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
		HitObjectSound sound = (HitObjectSound)mReader->ReadInt8();
		
		if (mForceNewCombo)
		{
			type = (HitObjectType)(type|HIT_COMBO);
			mForceNewCombo = false;
		}
		
		switch (type & ~HIT_COMBO) //ignore HIT_COMBO
		{
			case HIT_NORMAL:
			{
				object = new HitCircle(x, y, mNextObjectTime, type, sound);
				break;
			}
			
			case HIT_SLIDER:
			{
				uint32_t repeats = mReader->ReadInt16();
                uint32_t lengthtime = mReader->ReadInt32();

                uint32_t pointcount = mReader->ReadVarInt();
				std::vector<HitObjectPoint*> points;
				points.reserve(pointcount);
				
				for (uint32_t i=0; i<pointcount; ++i)
				{
					HitObjectPoint* tPoint = new HitObjectPoint();
					tPoint->x = mapXToScreen((int16_t)mReader->ReadInt16()); //s32 x
					tPoint->y = mapYToScreen((int16_t)mReader->ReadInt16()); //s32 y
					tPoint->angle = mReader->ReadInt32(); //s32 angle
					
					points.push_back(tPoint);
				}
				
				uint32_t tickcount = mReader->ReadVarInt();
				std::vector<HitObjectPoint*> ticks;
				ticks.reserve(tickcount);
				
				for (uint32_t i=0; i<tickcount; ++i)
				{
					HitObjectPoint* tPoint = new HitObjectPoint();
                    tPoint->x = mapXToScreen((int16_t)mReader->ReadInt16()); //s32 x
                    tPoint->y = mapYToScreen((int16_t)mReader->ReadInt16()); //s32 y

					ticks.push_back(tPoint);
				}
				
				object = new HitSlider(x, y, mNextObjectTime, lengthtime, points, ticks, repeats, type, sound);
				
				//free allocated memory
				for (uint32_t i=0; i<pointcount; ++i)
					delete points[i];
				
				for (uint32_t i=0; i<tickcount; ++i)
					delete ticks[i];
				
				break;
			}
			
			case HIT_SPINNER:
			{
				int32_t endtime = mReader->ReadInt32();
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
			object->SetPostCreateOptions((mNextObjectType & HIT_COMBO) | mForceNewCombo, mNextObjectX, mNextObjectY);
		}
		else
		{
			object->SetPostCreateOptions(true, 0, 0);
		}
	}
}

void Beatmap::ReadNextObject()
{
	mNextObjectTime = mReader->ReadInt32();
	mNextObjectType = (HitObjectType)mReader->ReadInt8();
    mNextObjectX = mapXToScreen((int16_t)mReader->ReadInt16()); //s32 x
    mNextObjectY = mapYToScreen((int16_t)mReader->ReadInt16()); //s32 y
}


std::string& Beatmap::BeatmapChecksum() {
	std::string checksum = "";
	//TODO: figure out some way to uniquely identify every beatmap without it being too intensive
	return checksum;
}