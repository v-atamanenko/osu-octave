////////////////////////////////////////////////////////////////
//                                                            //
//      OsTypes.h                                             //
//      HaselLoyance 2017-2018, Unlicense                     //
//      https://github.com/HaselLoyance/osu-parser            //
//                                                            //
////////////////////////////////////////////////////////////////

#ifndef OSU_PARSER_TYPES_H
#define OSU_PARSER_TYPES_H

#include <vector>
#include <string>

#define LIFE_BAR_POINT_LENGTH 2
#define ACTION_LENGTH 4

namespace osuParser
{
	// For readability and to follow format docs I'm typedefing all over the place
	// NOTE: OsTime will always be represented in milliseconds
	typedef int64_t OsTime;
	typedef uint8_t OsByte;
	typedef uint16_t OsShort;
	typedef uint32_t OsInteger;
	typedef uint64_t OsLong;
	typedef OsInteger ModMask;
	typedef OsByte InputMask;

	// Some typedefs for osu beatmap structure
	typedef std::pair<std::string, std::vector<std::string>> _OsSection;
	typedef std::vector<_OsSection> _OsBeatmap;
	typedef OsByte HitObjectMask;
	typedef OsByte HitSoundMask;

	// Valid input key values. Used in InputMask. Enum values represent the bits' number
	// that are active in InputMask
	enum InputType
	{
		iMouse1 = 0,
		iMouse2 = 1,
		iKey1 = 2,
		iKey2 = 3,
	};

	// Valid mods values. Used in ModMask. Enum values represent the bits' number
	// that are active in ModMask
	enum ModType
	{
		mNoFail = 0,
		mEasy = 1,
		mNoVideo = 2,
		mHidden = 3,
		mHardRock = 4,
		mSuddenDeath = 5,
		mDoubleTime = 6,
		mRelax = 7,
		mHalfTime = 8,
		mNightcore = 9,
		mFlashlight = 10,
		mAutoplay = 11,
		mSpunOut = 12,
		mRelax2 = 13,
		mPerfect = 14,
		mKey4 = 15,
		mKey5 = 16,
		mKey6 = 17,
		mKey7 = 18,
		mKey8 = 19,
		mkeyMod = 20,
		mFadeIn = 21,
		mRandom = 22,
		mLastMod = 23,
		mTargetPractice = 24,
		mKey9 = 25,
		mCoop = 26,
		mKey1 = 27,
		mKey3 = 28,
		mKey2 = 29,
	};

	// Valid game mode values
	enum GameMode
	{
		gmStandard = 0,
		gmTaiko = 1,
		gmCTB = 2,
		gmMania = 3,
	};

	// Valid sample set values
	enum SampleSet
	{
		ssAuto = 0,
		ssNormal = 1,
		ssSoft = 2,
		ssDrum = 3,
	};

	// Valid events for beatmap [Events] section
	// TODO (not a priority): possible support of storyboards?
	enum EventType
	{
		eBackground = 0,
		eVideo = 1,
		eBreak = 2,
		eUnknown = 3,
	};

	// Valid hit object values for beatmap [HitObjects] section. Used in HitObjectMask.
	// Enum values represent the bits' number that are active in HitObjectMask
	// THERE ARE SPECIFIC REASONS WHY I USE 'o' AS A PREFIX, PLEASE UNDERSTAND
	enum HitObjectType
	{
		oCircle = 0,
		oSlider = 1,
		oSpinner = 3,
		oHoldNote = 7,
	};

	// Valid hit sound values. Used in HitSoundMask. Enum values represent the bits' number
	// that are active in HitSoundMask
	enum HitSoundType
	{
		hsNormal = 0,
		hsWhistle = 1,
		hsFinish = 2,
		hsClap = 3,
	};

	// Valid slider types for slider hit objects
	enum SliderType
	{
		sLinear = 0,
		sPerfect = 1,
		sBezier = 2,
		sCatmull = 3,
	};

	// A point on the lifebar graph
	//   time, OsTime, time of the point on lifebar graph
	//   life, double, value of the lifebar from 0.0 (empty) to 1.0 (full)
	struct LifeBarPoint
	{
		OsTime time = 0;
		double life = 0.0;
	};

	// A replay action
	//   sinceLast, OsTime, action time since last action
	//   sinceStart, OsTime, action time since the start of the song
	//   x, double, x position of the cursor in osu!pixels
	//   y, double, y position of the cursor in osu!pixels
	//   inputs, InputMask, active inputs in this action
	struct Action
	{
		OsTime sinceLast = 0;
		OsTime sinceStart = 0;
		double x = 0.0;
		double y = 0.0;
		InputMask inputs = 0;
	};

	// A beatmap event in [Events] section
	//   type, EventType, type of the event
	//   file, string, name of the file that is specified for event
	//   begin, OsTime, when event should begin
	//   end, OsTime, when event should end
	struct Event
	{
		EventType type = eBackground;
		std::string file = "";
		OsTime begin = 0;
		OsTime end = 0;
	};

	// A beatmap timing point in [TimingPoints] section
	//   offset, OsTime, when the timing point starts
	//   msPerBeat, double, defines the duration of one beat. When positive, it is
	//     faithful to its name. When negative, it is a percentage of previous
	//     non-negative milliseconds per beat, which is stored in adjustedMsPerBeat
	//   adjustedMsPerBeat, double, adjusted duration of each bit based on the sign
	//     of msPerBeat
	//   beatsPerMeasure, uint8_t, number of beats in a measure
	//   sampleSet, SampleSet, default sample set for hit objects
	//   sampleIndex, uint8_t, default custom index
	//   isInheritable, bool, if the timing point can be inherited from
	//   is KiaiMode, bool, whether or not Kiai Time effects are active
	struct TimingPoint
	{
		OsTime offset = 0;
		double msPerBeat = 500.0;
		double adjustedMsPerBeat = 500.0;
		uint8_t beatsPerMeasure = 4; //-V112
		SampleSet sampleSet = ssNormal;
		uint8_t sampleIndex = 0;
		uint8_t volume = 100;
		bool isInheritable = true;
		bool isKiaiMode = false;
	};

	// A beatmap combo color in [Colours] section
	//   r, uint8_t, value of red channel
	//   g, uint8_t, value of green channel
	//   b, uint8_t, value of blue channel
	//   a, uint8_t, value of alpha channel
	struct RGBAColor
	{
		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;
		uint8_t a = 255;
	};

	// Additional parameters related to the hit sound samples
	//   sampleSet, SampleSet, changes the sample set of the normal hit sound
	//   additionSet, SampleSet, changes the sample set for the other hit
	//     sounds (whistle, finish, clap)
	//   customIndex , uint8_t, custom sample set index
	//   volume, uint8_t, volume of the sample
	//   filename, string, names an audio file in the folder to play instead
	//     of sounds from sample sets
	struct Extra
	{
		SampleSet sampleSet = ssAuto;
		SampleSet additionSet = ssAuto;
		uint8_t customIndex = 0;
		uint8_t volume = 0;
		std::string filename = "";
	};

	// Represents a curve point data for sliders
	//   x, uint16_t, x coordinate describing position of the control point
	//   y, uint16_t, y coordinate describing position of the control point
	struct CurvePoint
	{
		uint16_t x = 0;
		uint16_t y = 0;
	};

	// Sample sets to apply to the circles of the slider
	//   sampleSet, SampleSet, changes the sample set of the normal hit sound
	//   additionSet, SampleSet, changes the sample set for the other hit
	//     sounds (whistle, finish, clap)
	struct EdgeAddition
	{
		SampleSet sampleSet;
		SampleSet additionSet;
	};

	// Information about spinner hit object
	//   isSpinner, bool, if the hit object is actually a spinner
	//   duration, OsTime, duration of the spinner
	//   end, OsTime, when the spinner will end
	struct SpinnerData
	{
		bool isSpinner = false;
		OsTime duration = 0;
		OsTime end = 0;
	};

	// Information about slider hit object
	//   isSlider, bool, if the hit object is actually a slider
	//   sliderType, SliderType, type of the slider
	//   curvePoints, vector<CurvePoint>, points of the slider (including initial x and y)
	//   nRepeats, uint8_t, number of slider repeats
	//   length, double, total length of the slider in osu!pixels
	//   duration, OsTime, duration of the slider
	//   end, OsTime, when the slider will end
	//   edgeHitSounds, vector<HitSoundMask>, hit sounds to apply to the circles of the slider
	//   edgeAdditions, vector<EdgeAddition>, samples sets to apply to the circles of the slider
	//   adjustedEdgeAdditions, vector<EdgeAddition>, if for sampleSet or additionSet in edgeAdditions
	//     value is 0 (or ssAuto) then those values in adjustedEdgeAdditions will be inherited
	//     from the last related timing point (as it says to do so in format docs)
	struct SliderData
	{
		bool isSlider = false;
		SliderType type = sLinear;
		std::vector<CurvePoint> curvePoints;
		uint8_t nRepeats = 0;
		double length = 0;
		OsTime duration = 0;
        OsTime durationPerRepeat = 0;
		OsTime end = 0;
		std::vector<HitSoundMask> edgeHitSounds = {};
		std::vector<EdgeAddition> edgeAdditions = {};
		std::vector<EdgeAddition> adjustedEdgeAdditions = {};
	};

	// A beatmap hit object in [HitObjects] section
	//   x, uint16_t, x position of the center of the hit object in osu!pixels
	//   y, uint16_t, y position of the center of the hit object in osu!pixels
	//   time, OsTime, number of milliseconds from the beginning of the song, and
	//     specifies when the hit begins
	//   mask, HitObjectMask, bitmap specifying the object type and attributes
	//   type, HitObjectType, type of the hit object (from bits 0/1/3/7 in mask)
	//   isNewCombo, bool, whether or not this hit object starts a new combo (from bit 2 in mask)
	//   skipComboColors, uint8_t, number of combo colours to skip (from bits 4-6 in mask)
	//     The combo skip value is ignored when the new combo bit is not set
	//   soundMask, HitSoundMask, sounds to play when the hit object is successfully hit
	//   extra, Extra, additional parameters related to the hit sound samples
	//   adjustedExtra, Extra, if for any of sampleSet/additionSet/customIndex in extra
	//     value is 0 (or ssAuto) then those values in adjustedExtra will be inherited
	//     from the last related timing point (as it says to do so in format docs)
	//   spinner, SpinnerData, spinner information (if type==oSpinner)
	//   slider, SliderData, slider information (if type==oSlider)
	struct HitObject
	{
		OsTime time = 0; // At the top, because V802
		uint16_t x = 0;
		uint16_t y = 0;
		HitObjectMask mask = 0;
		HitObjectType type = oCircle;
		bool isNewCombo = false;
		uint8_t skipComboColors = 0;
		HitSoundMask soundMask = hsNormal;
		Extra extra = {};
		Extra adjustedExtra = {};
		SpinnerData spinner = {};
		SliderData slider = {};
	};
}

#endif
