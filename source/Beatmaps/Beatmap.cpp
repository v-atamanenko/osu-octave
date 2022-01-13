#include "Beatmap.h"

Beatmap::Beatmap(const char* filename, const char* basedir)
{
	fLoadable = false;
	
	mFilename = filename;
	mBaseDir = basedir;
	mReader = NULL;

	mChecksumString = "";
	
	FileReader r(filename);
	
	if (r.Ready())
	{
		//check header before processing
		char id[4] = { r.ReadInt8(), r.ReadInt8(), r.ReadInt8(), 0 };
		if (strcmp(id, "ODS") == 0)
		{
			
			u8 odsver = r.ReadInt8();
			
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
			iprintf("\x1b[0;0Hcannot load this file");
			//return;
		}
		
		chdir(mBaseDir.c_str());
		mReader = new FileReader(mFilename);
		
		//skip header
		mReader->Skip(3);
		
		u8 odsver = mReader->ReadInt8();
		
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

		u32 tpointcount = mReader->ReadVarInt();
		for (u32 j=0; j<tpointcount; ++j)
		{
			s32 time = mReader->ReadInt32();
			float beattime = mReader->ReadFloat();
			u8 samplesetid = mReader->ReadInt8();
			
			BeatmapElements::Element().AddTimingPoint(time, beattime, samplesetid);
		}

		u32 breakcount = mReader->ReadVarInt();

		for (u32 j=0; j<breakcount; ++j)
		{
			s32 starttime = mReader->ReadInt32();
			s32 endtime = mReader->ReadInt32();
			
			BeatmapElements::Element().AddBreakPoint(starttime, endtime);
		}

		iprintf("\x1b[2J");
		mHitObjectCount = mReader->ReadVarInt();
		mHitObjectRead = 0;
		mLastObjectEndTime = 0;
		mForceNewCombo = true;
		
		//read ahead
		ReadNextObject();
		mFirstObjectTime = mNextObjectTime;
		
		//the time to skip to is the first object - 8 beats
		mSkipTime = MathHelper::Max(0, (s32)mNextObjectTime - (BeatmapElements::Element().GetTimingPoint(mNextObjectTime).BeatTime*8));
		
		//strangely calling this in ctor of BeatmapElements causes game to not load :/
		BeatmapElements::Element().ResetColours(true);
		
		//now we can play this map
		fReady = true;
	}
}

void Beatmap::CleanUp()
{
	//set object back to uninitialised state
	fReady = false;
	
	if (mReader != NULL)
		delete mReader;
	mReader = NULL;
}

Beatmap::~Beatmap()
{
	if (mReader != NULL)
		delete mReader;
}

void Beatmap::Buffer(std::list<HitObject*>& hitObjectList)
{
	if (!fReady)
	{
		iprintf("\x1b[0;0Hnot ready to buffer");
		return;
	}
	
	//we buffer objects to 10 seconds ahead
	while (mHitObjectRead < mHitObjectCount && mNextObjectTime < GameClock::Clock().Time() + 3000)
	{
		HitObject* object;
		
		//all coordinates are s16 in file but s32 in RAM
		
		HitObjectType type = mNextObjectType;
		s32 x = mNextObjectX;
		s32 y = mNextObjectY;
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
				u32 repeats = mReader->ReadInt16();
				u32 lengthtime = mReader->ReadInt32();
				
				u32 pointcount = mReader->ReadVarInt();
				std::vector<HitObjectPoint*> points;
				points.reserve(pointcount);
				
				for (u32 i=0; i<pointcount; ++i)
				{
					HitObjectPoint* tPoint = new HitObjectPoint();
					tPoint->x = (s16)mReader->ReadInt16(); //s32 x
					tPoint->y = (s16)mReader->ReadInt16(); //s32 y
					tPoint->angle = mReader->ReadInt32(); //s32 angle
					
					points.push_back(tPoint);
				}
				
				u32 tickcount = mReader->ReadVarInt();
				std::vector<HitObjectPoint*> ticks;
				ticks.reserve(tickcount);
				
				for (u32 i=0; i<tickcount; ++i)
				{
					HitObjectPoint* tPoint = new HitObjectPoint();
					tPoint->x = (s16)mReader->ReadInt16(); //s32 x
					tPoint->y = (s16)mReader->ReadInt16(); //s32 y


					ticks.push_back(tPoint);
				}
				
				object = new HitSlider(x, y, mNextObjectTime, lengthtime, points, ticks, repeats, type, sound);
				
				//free allocated memory
				for (u32 i=0; i<pointcount; ++i)
					delete points[i];
				
				for (u32 i=0; i<tickcount; ++i)
					delete ticks[i];
				
				break;
			}
			
			case HIT_SPINNER:
			{
				s32 endtime = mReader->ReadInt32();
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
	mNextObjectX = (s16)mReader->ReadInt16();
	mNextObjectY = (s16)mReader->ReadInt16();
}


std::string& Beatmap::BeatmapChecksum() {
	std::string checksum = "";
	//TODO: figure out some way to uniquely identify every beatmap without it being too intensive
	return checksum;
}