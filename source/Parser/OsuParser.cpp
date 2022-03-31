////////////////////////////////////////////////////////////////
//                                                            //
//      OsuParser.cpp                                         //
//      HaselLoyance 2017-2018, Unlicense                     //
//      https://github.com/HaselLoyance/osu-parser            //
//                                                            //
////////////////////////////////////////////////////////////////

#include "OsuParser.h"

using namespace std;
using namespace osuParser;

// Creates a parser from istream
OsuParser::OsuParser(istream* stream)
{
    _s = stream;
    formatVersion = 0;
    audioFilename.clear();
    audioLeadIn = 0;
    previewTime = 0;
    countdown = false;
    sampleSet = ssAuto;
    stackLeniency = 0.0;
    mode = gmStandard;
    letterboxInBreaks = false;
    widescreenStoryboard = false;
    bookmarks = {};
    distanceSpacing = 0.0;
    beatDivisor = 0;
    gridSize = 0;
    gridLevel = 0;
    timelineZoom = 0.0;
    title.clear();
    titleUnicode.clear();
    artist.clear();
    artistUnicode.clear();
    creator.clear();
    version.clear();
    source.clear();
    tags = {};
    beatmapID = 0;
    beatmapSetID = 0;
    hpDrainRate = 0.0;
    HP = 0.0;
    circleSize = 0.0;
    CS = 0.0;
    circleRadiusPx = 0.0;
    overallDifficulty = 0.0;
    OD = 0.0;
    hitWindow300 = 0;
    hitWindow100 = 0;
    hitWindow50 = 0;
    requiredRPS = 0.0;
    approachRate = 0.0;
    AR = 0.0;
    preemptMs = 0;
    fadeInMs = 0;
    sliderMultiplier = 0.0;
    sliderTickRate = 0.0;
    events = {};
    timingPoints = {};
    msPerBeats = {};
    lowestBPM = 0.0;
    highestBPM = 0.0;
    averageBPM = 0.0;
    colors = {};
    hitObjects = {};

    _tpIndex = 0;
    _b = {};
}

OsuParser::~OsuParser() {};

// Goes through istream and reads all data
void OsuParser::Parse()
{
    _s->seekg(0);

    _GetBeatmapVersion();

    _ExtractStructure();

    _s->seekg(0);

    _OsSection defaultSection = _OsSection({});

    // GENERAL
    _OsSection t = _GetSection("General");
    if (t != defaultSection)
    {
        audioFilename = _ParseSectionField<string>(t, "AudioFilename", "Unknown");
        audioLeadIn = _ParseSectionField<OsTime>(t, "AudioLeadIn", 0);
        previewTime = _ParseSectionField<OsTime>(t, "PreviewTime", 0);
        countdown = _ParseSectionField<bool>(t, "Countdown", false);
        sampleSet = StringToEnum<SampleSet>(_ParseSectionField<string>(t, "SampleSet", "Unknown"), _sampleSetNames);
        // TODO: Implement this https://osu.ppy.sh/help/wiki/Beatmap_Editor/Song_Setup
        stackLeniency = _ParseSectionField<double>(t, "StackLeniency", 0.7);
        mode = (GameMode)_ParseSectionField<int>(t, "Mode", 0);
        letterboxInBreaks = _ParseSectionField<bool>(t, "LetterboxInBreaks", false);
        widescreenStoryboard = _ParseSectionField<bool>(t, "WidescreenStoryboard", false);
    }

    // EDITOR
    t = _GetSection("Editor");
    if (t != defaultSection)
    {
        bookmarks = _ParseSectionFieldAsList<OsTime>(t, "Bookmarks", ",");
        distanceSpacing = _ParseSectionField<double>(t, "DistanceSpacing", 1.22);
        beatDivisor = _ParseSectionField<int>(t, "BeatDivisor", 4); //-V112
        gridSize = _ParseSectionField<int>(t, "GridSize", 4); //-V112
        gridLevel = -round(log2(gridSize)) + 6;
        timelineZoom = _ParseSectionField<double>(t, "TimelineZoom", 1.0);
    }

    // METADATA
    t = _GetSection("Metadata");
    if (t != defaultSection)
    {
        title = _ParseSectionField<string>(t, "Title", "Unknown");
        titleUnicode = _ParseSectionField<string>(t, "TitleUnicode", title);
        artist = _ParseSectionField<string>(t, "Artist", "Unknown");
        artistUnicode = _ParseSectionField<string>(t, "ArtistUnicode", artist);
        creator = _ParseSectionField<string>(t, "Creator", "Unknown");
        version = _ParseSectionField<string>(t, "Version", "Unknown");
        source = _ParseSectionField<string>(t, "Source", "Unknown");
        tags = _ParseSectionFieldAsList<string>(t, "Tags", " ");
        beatmapID = _ParseSectionField<OsInteger>(t, "BeatmapID", 0);
        beatmapSetID = _ParseSectionField<OsInteger>(t, "BeatmapSetID", 0);
    }

    // DIFFICULTY
    t = _GetSection("Difficulty");
    if (t != defaultSection)
    {
        hpDrainRate = _ParseSectionField<double>(t, "HPDrainRate", 5.0);
        HP = hpDrainRate;
        circleSize = _ParseSectionField<double>(t, "CircleSize", 4.0);
        CS = circleSize;
        overallDifficulty = _ParseSectionField<double>(t, "OverallDifficulty", 6.0);
        OD = overallDifficulty;
        approachRate = _ParseSectionField<double>(t, "ApproachRate", 7.0);
        AR = approachRate;
        sliderMultiplier = _ParseSectionField<double>(t, "SliderMultiplier", 1.4);
        sliderTickRate = _ParseSectionField<double>(t, "SliderTickRate", 1.0);

        preemptMs = (AR < 5.0 || IsEqualDouble(AR, 5.0))
                    ? (1800.0 - 120.0 * AR)
                    : (1950.0 - 150.0 * AR);

        fadeInMs = (AR < 5.0 || IsEqualDouble(AR, 5.0))
                   ? (1200.0 - 80.0 * AR)
                   : (1300.0 - 100.0 * AR);

        circleRadiusPx = 54.4 - 4.48 * CS;

        hitWindow300 = 80.0 - 6.0 * OD;
        hitWindow100 = 140.0 - 8.0 * OD;
        hitWindow50 = 200.0 - 10.0 * OD;

        // TODO: add mapDifficulty function for readability sake
        requiredRPS = (OD < 5.0 || IsEqualDouble(OD, 5.0))
                      ? (3.0 - 2.0 / 5.0 * OD)
                      : (5.0 - 5.0 / 2.0 + OD / 2.0);
    }

    // EVENTS
    t = _GetSection("Events");
    if (t != defaultSection)
    {
        for (auto&& f : t.second)
        {
            if (Event e = _ParseFieldAsEvent(f); e.type != eUnknown)
            {
                events.push_back(e);
            }
        }
    }

    // TIMING POINTS
    t = _GetSection("TimingPoints");
    if (t != defaultSection)
    {
        for (auto&& f : t.second)
        {
            timingPoints.push_back(_ParseFieldAsTimingPoint(f));
        }

        lowestBPM = 1234567.0;
        highestBPM = -1234567.0;
        averageBPM = 0.0;

        for (auto&& msPerBeat : msPerBeats)
        {
            double bpm = 60000.0 / msPerBeat;

            if (bpm > highestBPM)
            {
                highestBPM = bpm;
            }

            if (bpm < lowestBPM)
            {
                lowestBPM = bpm;
            }

            averageBPM += bpm;
        }

        averageBPM /= (double)msPerBeats.size();
    }

    // COLORS
    t = _GetSection("Colours");
    if (t != defaultSection)
    {
        for (auto&& f : t.second)
        {
            // Hmmm... so apparently there are "Special coloUrs" for slider
            // TODO (not a priority) them some time

            colors.push_back(_ParseFieldAsRGBAColor(f));
        }
    }

    // HIT OBJECTS
    t = _GetSection("HitObjects");
    if (t != defaultSection)
    {
        for (auto&& f : t.second)
        {
            hitObjects.push_back(_ParseFieldAsHitObject(f));
        }
    }

    _b.clear();
}

void OsuParser::_GetBeatmapVersion()
{
    string fileVersionString = "";
    getline(*_s, fileVersionString);

    if (size_t len = fileVersionString.find('v'); len != string::npos)
    {
        fileVersionString.erase(0, len + 1);
        formatVersion = stoi(fileVersionString);

        return;
    }

    formatVersion = (uint8_t)-1;
}

void OsuParser::_ExtractStructure()
{
    string t;

    while (!_s->eof() && !_s->fail())
    {
        getline(*_s, t);
        TrimString(t);

        if (t.empty())
        {
            continue;
        }

        size_t sz = t.size();

        if (t[0] != '[' || t[sz - 1] != ']')
        {
            continue;
        }

        _OsSection section;
        section.first = t.substr(1, sz - 2);

        while (!_s->eof() && !_s->fail())
        {
            getline(*_s, t);
            TrimString(t);

            if (t.empty())
            {
                break;
            }

            // Skip commented lines
            if (t[0] == '#' || t.find("//") == 0)
            {
                continue;
            }

            section.second.push_back(t);
        }

        _b.push_back(section);
    }
}

_OsSection OsuParser::_GetSection(const std::string& name)
{
    for (auto&& section : _b)
    {
        if (section.first == name)
        {
            return section;
        }
    }

    return {};
}

template<typename T>
T OsuParser::_ParseSectionField(const _OsSection& section, const string& fieldName, const T& defaultTo)
{
    for (auto&& field : section.second)
    {
        if (field.find(fieldName) == string::npos)
        {
            continue;
        }

        string f = field;
        if (size_t len = f.find(':'); len != string::npos)
        {
            f.erase(0, len + 1);

            while (f[0] == ' ') //remove whitespaces at the beginning
                f.erase(0, 1);

            // OH BOY, LOOK, THIS RANDOM constexpr OVER HERE MAKES IT WORK
            if constexpr (is_same_v<T, string>)
            {
                return f;
            }
            else
            {
                T t;
                stringstream(f) >> t;
                return t;
            }
        }
    }

    return defaultTo;
}

template<typename T>
vector<T> OsuParser::_ParseSectionFieldAsList(const _OsSection& section, const string& fieldName, const string& delim)
{
    for (auto&& field : section.second)
    {
        if (field.find(fieldName) == string::npos)
        {
            continue;
        }

        string f = field;
        if (size_t len = f.find(':'); len != string::npos)
        {
            f.erase(0, len + 1);

            while (f[0] == ' ') //remove whitespaces at the beginning
                f.erase(0, 1);

            vector<string> listValues;
            vector<T> listT;
            SplitString(f, delim, listValues);

            for (auto&& val : listValues)
            {
                if constexpr (is_same_v<T, std::string>)
                {
                    listT.push_back(val);
                }
                else
                {
                    T t;
                    stringstream(val) >> t;
                    listT.push_back(t);
                }
            }

            return listT;
        }
    }

    return {};
}

Event OsuParser::_ParseFieldAsEvent(const string& field)
{
    string f = field;

    vector<string> args;
    SplitString(f, ",", args);

    if (args[0] == "0")
    {
        std::string fn = args[2];
        TrimStringQuotes(fn);
        return { eBackground, fn };
    }
    else if (args[0] == "2")
    {
        return { eBreak, "", stoll(args[1]), stoll(args[2]) };
    }
    else if (args[0] == "Video")
    {
        size_t sz = f.find('"') + 1;
        return { eVideo, string(f, sz, f.find_last_of('"') - sz), stoll(args[1]) };
    }

    return { eUnknown };
}

TimingPoint OsuParser::_ParseFieldAsTimingPoint(const string& field)
{
    string f = field;

    vector<string> args;
    SplitString(f, ",", args);

    TimingPoint tp;

    tp.offset = stoll(args[0]);
    tp.msPerBeat = stod(args[1]);

    if (tp.msPerBeat > 0)
    {
        tp.adjustedMsPerBeat = tp.msPerBeat;

        msPerBeats.push_back(tp.msPerBeat);
    }
    else
    {
        tp.adjustedMsPerBeat = (tp.msPerBeat / -100.0) * msPerBeats.back();
    }

    tp.beatsPerMeasure = (uint8_t)stoi(args[2]);
    tp.sampleSet = (SampleSet)stoi(args[3]);
    tp.sampleIndex = (uint8_t)stoi(args[4]);
    tp.volume = (uint8_t)stoi(args[5]);
    if (args.size() > 6) {
        tp.isInheritable = (bool)stoi(args[6]);
        if (args.size() > 7) {
            tp.isKiaiMode = (bool) stoi(args[7]);
        }
    }

    return tp;
}

RGBAColor OsuParser::_ParseFieldAsRGBAColor(const string& field)
{
    string f = field; //-V808

    if (size_t len = f.find(':'); len != string::npos)
    {
        f.erase(0, len + 1);

        vector<string> args;
        SplitString(f, ",", args);

        return {
                (uint8_t)stoi(args[0]),
                (uint8_t)stoi(args[1]),
                (uint8_t)stoi(args[2]),
        };
    }

    return {};
}

HitObject OsuParser::_ParseFieldAsHitObject(const string& field)
{
    string f = field;

    vector<string> args;
    SplitString(f, ",", args);

    HitObject o;

    o.x = (uint16_t)stoi(args[0]);
    o.y = (uint16_t)stoi(args[1]);
    o.time = stoll(args[2]);
    o.mask = (HitObjectMask)stoi(args[3]);

    if (IsBitSet(o.mask, 0))
    {
        o.type = oCircle;
    }

    if (IsBitSet(o.mask, 1))
    {
        o.type = oSlider;
    }

    if (IsBitSet(o.mask, 3))
    {
        o.type = oSpinner;
    }

    if (IsBitSet(o.mask, 7))
    {
        o.type = oHoldNote;
    }

    o.isNewCombo = IsBitSet(o.mask, 2);

    if (o.isNewCombo)
    {
        o.skipComboColors =
                ((uint8_t)IsBitSet(o.mask, 4) << 0) |
                ((uint8_t)IsBitSet(o.mask, 5) << 1) |
                ((uint8_t)IsBitSet(o.mask, 6) << 2);
    }

    o.soundMask = (HitSoundMask)stoi(args[4]);

    _ExtractExtras(args.back(), o);

    // TODO: maybe safety checks for args size for different type of hit objects
    if (o.type == oSpinner)
    {
        o.spinner.isSpinner = true;
        o.spinner.end = stoll(args[5]);
        o.spinner.duration = o.spinner.end - o.time;
    }

    if (o.type == oSlider)
    {
        o.slider.isSlider = true;
        o.slider.curvePoints.push_back({ o.x, o.y });

        switch (args[5][0])
        {
            case 'B': o.slider.type = sBezier; break;
            case 'C': o.slider.type = sCatmull; break;
            case 'P': o.slider.type = sPerfect; break;
            default: o.slider.type = sLinear; break;
        }

        args[5].erase(0, 2);

        vector<string> params;
        SplitString(args[5], "|", params);

        for (auto&& p : params)
        {
            vector<string> values;
            SplitString(p, ":", values);

            int x = stoi(values[0]);
            int y = stoi(values[1]);

            if (x < 1) x = 1;
            if (y < 1) y = 1;

            o.slider.curvePoints.push_back({(uint16_t)x, (uint16_t)y});
        }

        o.slider.nRepeats = (uint8_t)stoi(args[6]);
        o.slider.length = stod(args[7]);
        o.slider.duration = (o.slider.length * o.slider.nRepeats) / (100.0 * sliderMultiplier) * timingPoints[_tpIndex].adjustedMsPerBeat;
        o.slider.durationPerRepeat = (o.slider.length) / (100.0 * sliderMultiplier) * timingPoints[_tpIndex].adjustedMsPerBeat;
        o.slider.end = o.time + o.slider.duration;

        if (args.size() > 9) { //there might be incomplete slider hitobjects
            SplitString(args[8], "|", params);

            for (size_t i = 0; i < params.size(); i++)
            {
                o.slider.edgeHitSounds.push_back((HitSoundMask)stoi(params[i]));
            }

            SplitString(args[9], "|", params);

            for (size_t i = 0; i < params.size(); i++)
            {
                vector<string> values;
                SplitString(params[i], ":", values);

                /*o.slider.curvePoints.push_back({
                                                       (uint16_t)stoi(values[0]),
                                                       (uint16_t)stoi(values[1]),
                                               });*/
            }
        }
        else {
            for (int i = 0; i < 2; i++) {
                o.slider.edgeHitSounds.push_back((HitSoundMask)0);

                /*o.slider.curvePoints.push_back({
                                                       (uint16_t)0,
                                                       (uint16_t)0,
                                               });*/
            }
        }
    }

    return o;
}

void OsuParser::_ExtractExtras(const string& s, HitObject& o)
{
    vector<string> params;
    SplitString(s, ":", params);

    // Valid extras have at least 4 values
    if (params.size() < 4) //-V112
    {
        return;
    }

    int extra_ss_tmp = stoi(params[0]);
    if (extra_ss_tmp < 0 || extra_ss_tmp > ssMaxAllowedValue) {
        o.extra.sampleSet = ssAuto;
    } else {
        o.extra.sampleSet = (SampleSet)extra_ss_tmp;
    }

    o.extra.additionSet = (SampleSet)stoi(params[1]);
    o.extra.customIndex = (uint8_t)stoi(params[2]);
    o.extra.volume = (uint8_t)stoi(params[3]);

    if (params.size() == 5 && mode != gmMania)
    {
        o.extra.volume = (uint8_t)stoi(params[4]);
        //TODO: Find out what happens here. "&& mode != gmMania" this might have helped
        //Failed on string "0:0:0:50:LR_Drum J Long Med Finish.wav"
    }

    o.adjustedExtra = o.extra;

    // Keep track of the internal timing point counter
    while (_tpIndex + 1 < timingPoints.size() && o.time >= timingPoints[_tpIndex + 1].offset)
    {
        _tpIndex++;
    }

    const TimingPoint& tp = timingPoints[_tpIndex];

    if (o.extra.sampleSet == ssAuto)
    {
        o.adjustedExtra.sampleSet = tp.sampleSet;
    }

    o.adjustedExtra.additionSet = tp.sampleSet;

    if (o.extra.customIndex == 0)
    {
        o.adjustedExtra.customIndex = tp.sampleIndex;
    }
}