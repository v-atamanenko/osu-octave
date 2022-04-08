////////////////////////////////////////////////////////////////
//                                                            //
//      OsuParser.h                                           //
//      HaselLoyance 2017-2018, Unlicense                     //
//      https://github.com/HaselLoyance/osu-parser            //
//                                                            //
////////////////////////////////////////////////////////////////

#ifndef OSU_PARSER_OSU_H
#define OSU_PARSER_OSU_H

#include <sstream>
#include "OsTypes.h"
#include "OsUtils.h"

namespace osuParser
{
	// Object for parsing "*.osu" files from preloaded stream
	// For file format please visit: https://osu.ppy.sh/help/wiki/osu!_File_Formats/Osu_(file_format)
	class OsuParser
	{
	public:
		// Creates a parser from input data stream
		OsuParser(std::istream * stream);
		~OsuParser();

		// Goes through istream and reads all data
		void Parse();

		// Version of beatmap file
		uint8_t formatVersion;

		// Location of the audio file
		std::string audioFilename;

		// Amount of time added before the audio file begins playing
		OsTime audioLeadIn;

		// When the audio file should begin playing when selected in the song selection menu
		OsTime previewTime;

		// Whether or not a countdown occurs before the first hit object appears
		bool countdown;

		// What set of hit sounds will be used
		SampleSet sampleSet;

		// How often closely placed hit objects will be stacked together
		double stackLeniency;

		// Defines the game mode
		GameMode mode;

		// Whether the letterbox appears during breaks
		bool letterboxInBreaks;

		// Whether or not the storyboard should be widescreen
		bool widescreenStoryboard;

		// Times of editor bookmarks
		std::vector<OsTime> bookmarks;

		// Multiplier for the "Distance Snap" feature
		double distanceSpacing;

		// Beat division for placing objects
		uint8_t beatDivisor;

		// Size of the grid for the "Grid Snap" feature
		uint8_t gridSize;

		// Grid level as it appears in the editor
		uint8_t gridLevel;

		// Zoom in the editor timeline
		double timelineZoom;

		// Title of the song limited to ASCII characters
		std::string title;

		// Title of the song with unicode support. If not present, title is used
		std::string titleUnicode;

		// Name of the song's artist limited to ASCII characters
		std::string artist;

		// Name of the song's artist with unicode support. If not present, artist is used
		std::string artistUnicode;

		// Username of the mapper
		std::string creator;

		// Name of the beatmap's difficulty
		std::string version;

		// Describes the origin of the song
		std::string source;

		// Vector of words describing the song
		std::vector<std::string> tags;

		// ID of the single beatmap
		OsInteger beatmapID;

		// ID of the beatmap set
		OsInteger beatmapSetID;

		// HP drain difficulty
		double hpDrainRate, HP;

		// Size of hit object circles
		double circleSize, CS;

		// Amount of time allowed to click a hit object on time
		double overallDifficulty, OD;

		// Amount of time taken for the approach circle and hit object to appear
		double approachRate, AR;

		// Radius of a circle based on CS in osu! pixels
		double circleRadiusPx;

		// How much time is allowed to hit each score
		OsTime hitWindow300, hitWindow100, hitWindow50;

		// Harshness of spinners based on OD. Rotations per second required
		double requiredRPS;

		// Amount of time since object's start to fade in to perfect hit
		OsTime preemptMs;

		// Amount of time it takes for object to completely fade in
		OsTime fadeInMs;

		// Multiplier for the slider velocity
		double sliderMultiplier;

		// How often slider ticks appear
		double sliderTickRate;

		// Events related to background and breaks
		std::vector<Event> events;

		// Vector of timing points that describe a number of
		//   properties regarding beats per minute and hit sounds
		std::vector<TimingPoint> timingPoints;

		// All values of msPerBeat in the beatmap that are inheritable
		std::vector<double> msPerBeats;

		// Lowest beatmap BPM (highest msPerBeat)
		double lowestBPM;

		// Highest beatmap BPM (lowest msPerBeat)
		double highestBPM;

		// Average beatmap BPM
		double averageBPM;

		// Vector of the colors of the combos
		std::vector<RGBAColor> colors;

		// Vector of all the hit objects in a beatmap
		std::vector<HitObject> hitObjects;

	private:
		// Methods that extract and parse data from beatmap file
		void _GetBeatmapVersion();
		void _ExtractStructure();
		_OsSection _GetSection(const std::string & name = "General");
		template<typename T>
		T _ParseSectionField(const _OsSection & section, const std::string & fieldName, const T & defaultTo);
		template<typename T>
		std::vector<T> _ParseSectionFieldAsList(const _OsSection & section, const std::string & fieldName, const std::string & delim);
		Event _ParseFieldAsEvent(const std::string & field);
		TimingPoint _ParseFieldAsTimingPoint(const std::string & field);
		RGBAColor _ParseFieldAsRGBAColor(const std::string & field);
		HitObject _ParseFieldAsHitObject(const std::string & field);
		void _ExtractExtras(const std::string & s, HitObject & o);

		// When hit objects' extras are adjusted based on inheritance from
		//   timing points this counter is used, so there are no three leveled
		//   references through methods
		size_t _tpIndex;

		// Structure of all of the sections of the beatmap with all fields
		_OsBeatmap _b;

		// Pointer to the passed data stream in constructor
		std::istream * _s;
	};
}
#endif
