// -*-c++-*-
/**
 * TuningsImpl.h
 * Copyright 2019-2020 Paul Walker
 * Released under the MIT License. See LICENSE.md
 *
 * This contains the nasty nitty gritty implementation of the api in Tunings.h. You probably
 * don't need to read it unless you have found and are fixing a bug, are curious, or want
 * to add a feature to the API. For usages of this library, the documentation in Tunings.h and
 * the usages in tests/all_tests.cpp should provide you more than enough guidance.
 */

#ifndef __INCLUDE_TUNINGS_IMPL_H
#define __INCLUDE_TUNINGS_IMPL_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include <sstream>
#include <cctype>
#include <cmath>
#include <regex>
#include <numeric>
#include <limits>
#include <algorithm>

namespace Tunings
{
// Thank you to: https://gist.github.com/josephwb/df09e3a71679461fc104
inline std::istream &getlineEndingIndependent(std::istream &is, std::string &t)
{
    t.clear();

    std::istream::sentry se(is, true);
    if (!se)
        return is;

    std::streambuf *sb = is.rdbuf();

    for (;;)
    {
        int c = sb->sbumpc();
        switch (c)
        {
        case '\n':
            return is;
        case '\r':
            if (sb->sgetc() == '\n')
            {
                sb->sbumpc();
            }
            return is;
        case EOF:
            is.setstate(std::ios::eofbit);
            if (t.empty())
            {
                is.setstate(std::ios::badbit);
            }
            return is;
        default:
            t += (char)c;
        }
    }
}

inline double locale_atof(const char *s)
{
    double result = 0;
    std::istringstream istr(s);
    istr.imbue(std::locale("C"));
    istr >> result;
    return result;
}

inline unsigned positive_mod(int v, unsigned m)
{
    int mod = v % (int)m;
    if (mod < 0)
        mod += m;
    return mod;
}

inline Tone toneFromString(const std::string &fullLine, int lineno)
{
    Tone t;
    t.stringRep = fullLine;
    t.lineno = lineno;

    // Allow end-of-line comments, e.g. "555/524 ! c# 138.75 Hz"
    std::string line = fullLine.substr(0, fullLine.find("!", 0));

    if (line.find('.') != std::string::npos)
    {
        t.type = Tone::kToneCents;
        t.cents = locale_atof(line.c_str());
    }
    else
    {
        t.type = Tone::kToneRatio;
        auto slashPos = line.find('/');
        if (slashPos == std::string::npos)
        {
            t.ratio_n = atoll(line.c_str());
            t.ratio_d = 1;
        }
        else
        {
            t.ratio_n = atoll(line.substr(0, slashPos).c_str());
            t.ratio_d = atoll(line.substr(slashPos + 1).c_str());
        }

        if (t.ratio_n == 0 || t.ratio_d == 0)
        {
            std::string s = "Invalid tone in SCL file.";
            if (lineno >= 0)
                s += "Line " + std::to_string(lineno) + ".";
            s += " Line is '" + line + "'.";
            throw TuningError(s);
        }
        // 2^(cents/1200) = n/d
        // cents = 1200 * log(n/d) / log(2)

        t.cents = 1200 * log(1.0 * t.ratio_n / t.ratio_d) / log(2.0);
    }
    t.floatValue = t.cents / 1200.0 + 1.0;
    return t;
}

inline Scale readSCLStream(std::istream &inf)
{
    std::string line;
    const int read_header = 0, read_count = 1, read_note = 2, trailing = 3;
    int state = read_header;

    Scale res;
    std::ostringstream rawOSS;
    int lineno = 0;
    while (getlineEndingIndependent(inf, line))
    {
        rawOSS << line << "\n";
        lineno++;

        if ((state == read_note && line.empty()) || line[0] == '!')
        {
            res.comments.push_back(line);
            continue;
        }
        switch (state)
        {
        case read_header:
            res.description = line;
            state = read_count;
            break;
        case read_count:
            res.count = atoi(line.c_str());
            if (res.count < 1)
            {
                throw TuningError("Invalid SCL note count.");
            }
            state = read_note;
            break;
        case read_note:
            auto t = toneFromString(line, lineno);
            res.tones.push_back(t);
            if ((int)res.tones.size() == res.count)
                state = trailing;

            break;
        }
    }

    if (!(state == read_note || state == trailing))
    {
        std::ostringstream oss;
        oss << "Incomplete SCL content. Only able to read " << lineno
            << " lines of data. Found content up to ";
        switch (state)
        {
        case read_header:
            oss << "reading header.";
            break;
        case read_count:
            oss << "reading scale count.";
            break;
        default:
            oss << "unknown state.";
            break;
        }
        throw TuningError(oss.str());
    }

    if ((int)res.tones.size() != res.count)
    {
        std::string s =
            "Read fewer notes than count in file. Count = " + std::to_string(res.count) +
            " notes. Array size = " + std::to_string(res.tones.size());
        throw TuningError(s);
    }
    res.rawText = rawOSS.str();
    return res;
}

inline Scale readSCLFile(std::string fname)
{
    std::ifstream inf;
    inf.open(fname);
    if (!inf.is_open())
    {
        std::string s = "Unable to open file '" + fname + "'";
        throw TuningError(s);
    }

    auto res = readSCLStream(inf);
    res.name = fname;
    return res;
}

inline Scale parseSCLData(const std::string &d)
{
    std::istringstream iss(d);
    auto res = readSCLStream(iss);
    res.name = "Scale from patch";
    return res;
}

inline Scale evenTemperament12NoteScale()
{
    std::string data = R"SCL(! 12 Tone Equal Temperament.scl
!
12 Tone Equal Temperament | ED2-12 - Equal division of harmonic 2 into 12 parts
 12
!
 100.00000
 200.00000
 300.00000
 400.00000
 500.00000
 600.00000
 700.00000
 800.00000
 900.00000
 1000.00000
 1100.00000
 2/1
)SCL";
    return parseSCLData(data);
}

inline Scale evenDivisionOfSpanByM(int Span, int M)
{
    if (Span <= 0)
        throw Tunings::TuningError("Span should be a positive number. You entered " +
                                   std::to_string(Span));
    if (M <= 0)
        throw Tunings::TuningError(
            "You must divide the period into at least one step. You entered " + std::to_string(M));

    std::ostringstream oss;
    oss.imbue(std::locale("C"));
    oss << "! Automatically generated ED" << Span << "-" << M << " scale\n";
    oss << "Automatically generated ED" << Span << "-" << M << " scale\n";
    oss << M << "\n";
    oss << "!\n";

    double topCents = 1200.0 * log(1.0 * Span) / log(2.0);
    double dCents = topCents / M;
    for (int i = 1; i < M; ++i)
        oss << std::fixed << dCents * i << "\n";
    oss << Span << "/1\n";

    return parseSCLData(oss.str());
}

inline Scale evenDivisionOfCentsByM(float Cents, int M, const std::string &lastLabel)
{
    if (Cents <= 0)
        throw Tunings::TuningError("Cents should be a positive number. You entered " +
                                   std::to_string(Cents));
    if (M <= 0)
        throw Tunings::TuningError(
            "You must divide the period into at least one step. You entered " + std::to_string(M));

    std::ostringstream oss;
    oss.imbue(std::locale("C"));
    oss << "! Automatically generated Even Division of " << Cents << " ct into " << M << " scale\n";
    oss << "Automatically generated Even Division of " << Cents << " ct into " << M << " scale\n";
    oss << M << "\n";
    oss << "!\n";

    double topCents = Cents;
    double dCents = topCents / M;
    for (int i = 1; i < M; ++i)
        oss << std::fixed << dCents * i << "\n";
    if (lastLabel.empty())
        oss << Cents << "\n";
    else
        oss << lastLabel << "\n";

    return parseSCLData(oss.str());
}

inline KeyboardMapping readKBMStream(std::istream &inf)
{
    std::string line;

    KeyboardMapping res;
    std::ostringstream rawOSS;
    res.keys.clear();

    enum parsePosition
    {
        map_size = 0,
        first_midi,
        last_midi,
        middle,
        reference,
        freq,
        degree,
        keys,
        trailing
    };
    parsePosition state = map_size;

    int lineno = 0;
    while (getlineEndingIndependent(inf, line))
    {
        rawOSS << line << "\n";
        lineno++;
        if (line[0] == '!')
        {
            continue;
        }

        if (line == "x")
            line = "-1";
        else if (state != trailing)
        {
            const char *lc = line.c_str();
            bool validLine = line.length() > 0;
            char badChar = '\0';
            while (validLine && *lc != '\0')
            {
                if (!(*lc == ' ' || std::isdigit(*lc) || *lc == '.' || *lc == (char)13 ||
                      *lc == '\n'))
                {
                    validLine = false;
                    badChar = *lc;
                }
                lc++;
            }
            if (!validLine)
            {
                throw TuningError("Invalid line " + std::to_string(lineno) + ". line='" + line +
                                  "'. Bad character is '" + badChar + "/" +
                                  std::to_string((int)badChar) + "'");
            }
        }

        int i = std::atoi(line.c_str());
        double v = locale_atof(line.c_str());

        switch (state)
        {
        case map_size:
            res.count = i;
            break;
        case first_midi:
            res.firstMidi = i;
            break;
        case last_midi:
            res.lastMidi = i;
            break;
        case middle:
            res.middleNote = i;
            break;
        case reference:
            res.tuningConstantNote = i;
            break;
        case freq:
            res.tuningFrequency = v;
            res.tuningPitch = res.tuningFrequency / MIDI_0_FREQ;
            break;
        case degree:
            res.octaveDegrees = i;
            break;
        case keys:
            res.keys.push_back(i);
            if ((int)res.keys.size() == res.count)
                state = trailing;
            break;
        case trailing:
            break;
        }
        if (!(state == keys || state == trailing))
            state = (parsePosition)(state + 1);
        if (state == keys && res.count == 0)
            state = trailing;
    }

    if (!(state == keys || state == trailing))
    {
        std::ostringstream oss;
        oss << "Incomplete KBM stream. Only able to read " << lineno << " lines. Read up to ";
        switch (state)
        {
        case map_size:
            oss << "map size.";
            break;
        case first_midi:
            oss << "first midi note.";
            break;
        case last_midi:
            oss << "last midi note.";
            break;
        case middle:
            oss << "scale zero note.";
            break;
        case reference:
            oss << "scale reference note.";
            break;
        case freq:
            oss << "scale reference frequency.";
            break;
        case degree:
            oss << "scale degree.";
            break;
        default:
            oss << "unknown state";
            break;
        }
        throw TuningError(oss.str());
    }

    if ((int)res.keys.size() != res.count)
    {
        throw TuningError("Different number of keys than mapping file indicates. Count is " +
                          std::to_string(res.count) + " and we parsed " +
                          std::to_string(res.keys.size()) + " keys.");
    }

    res.rawText = rawOSS.str();
    return res;
}

inline KeyboardMapping readKBMFile(std::string fname)
{
    std::ifstream inf;
    inf.open(fname);
    if (!inf.is_open())
    {
        std::string s = "Unable to open file '" + fname + "'";
        throw TuningError(s);
    }

    auto res = readKBMStream(inf);
    res.name = fname;
    return res;
}

inline KeyboardMapping parseKBMData(const std::string &d)
{
    std::istringstream iss(d);
    auto res = readKBMStream(iss);
    res.name = "Mapping from patch";
    return res;
}

inline Tuning::Tuning() : Tuning(evenTemperament12NoteScale(), KeyboardMapping()) {}
inline Tuning::Tuning(const Scale &s) : Tuning(s, KeyboardMapping()) {}
inline Tuning::Tuning(const KeyboardMapping &k) : Tuning(evenTemperament12NoteScale(), k) {}
inline Tuning::Tuning(const AbletonScale &as) : Tuning(as.scale, as.keyboardMapping)
{
    this->notationMapping = as.notationMapping;
}

inline Tuning::Tuning(const Scale &s_, const KeyboardMapping &k_, bool allowTuningCenterOnUnmapped_)
    : allowTuningCenterOnUnmapped(allowTuningCenterOnUnmapped_)
{
    // Shadow on purpose to make sure we use the modified version from rotation - use for dev
    // int *scale{0}, keyboardMapping{0};
    this->scale = s_;
    this->keyboardMapping = k_;

    Scale s = s_;
    KeyboardMapping k = k_;
    int oSP;
    if (s.count <= 0)
        throw TuningError("Unable to tune to a scale with no notes. Your scale provided " +
                          std::to_string(s.count) + " notes.");

    int useMiddleNote{k.middleNote};
    if (k.count > 0)
    {
        // Is the KBM not spanning the tuning note
        auto mapStart = useMiddleNote;
        auto mapEnd = useMiddleNote + k.count;
        while (mapStart > k.tuningConstantNote)
        {
            useMiddleNote -= k.count;
            mapStart = useMiddleNote;
            mapEnd = useMiddleNote + k.count;
            // throw std::logic_error("Blah");
        }
        while (mapEnd < k.tuningConstantNote)
        {
            useMiddleNote += k.count;
            mapStart = useMiddleNote;
            mapEnd = useMiddleNote + k.count;
            // throw std::logic_error("Blah");
        }
    }

    int kbmRotations{1};
    for (const auto &kv : k.keys)
    {
        kbmRotations = std::max(kbmRotations, (int)std::ceil(1.0 * kv / s.count));
    }

    if (kbmRotations > 1)
    {
        // This means the KBM has mapped note 5 in a 4 note scale or some such
        // which implies an 'unwrap' operation. So what we are going to do is
        // create a new scale which is extended then update the kbm octave position
        // accordingly.
        Scale newS = s;
        newS.count = s.count * kbmRotations;
        auto backCents = s.tones.back().cents;
        auto pushOff = backCents;
        for (int i = 1; i < kbmRotations; ++i)
        {
            for (const auto &t : s.tones)
            {
                Tunings::Tone tCopy = t;
                tCopy.type = Tone::kToneCents;
                tCopy.cents += pushOff;
                tCopy.floatValue = tCopy.cents / 1200.0 + 1;

                newS.tones.push_back(tCopy);
            }
            pushOff += backCents;
        }
        s = newS;
        k.octaveDegrees *= kbmRotations;
        if (k.octaveDegrees == 0)
            k.octaveDegrees = s.count;
    }

    // From the KBM Spec: When not all scale degrees need to be mapped, the size of the map can be
    // smaller than the size of the scale.
    if (k.octaveDegrees > s.count)
    {
        throw TuningError("Unable to apply mapping of size " + std::to_string(k.octaveDegrees) +
                          " to smaller scale of size " + std::to_string(s.count));
    }

    int posPitch0 = 256 + k.tuningConstantNote;
    int posScale0 = 256 + useMiddleNote;

    double pitchMod = log(k.tuningPitch) / log(2) - 1;

    int scalePositionOfTuningNote = k.tuningConstantNote - useMiddleNote;
    if (k.count > 0)
    {
        while (scalePositionOfTuningNote >= k.count)
        {
            scalePositionOfTuningNote -= k.count;
        }
        while (scalePositionOfTuningNote < 0)
        {
            scalePositionOfTuningNote += k.count;
        }
        oSP = scalePositionOfTuningNote;
        scalePositionOfTuningNote = k.keys[scalePositionOfTuningNote];
        if (scalePositionOfTuningNote == -1 && !allowTuningCenterOnUnmapped)
        {
            std::string s = "Keyboard mapping is tuning an unmapped key. ";
            s += "Your tuning mapping is mapping key " + std::to_string(k.tuningConstantNote) +
                 " as " + "the tuning constant note, but that is scale note " +
                 std::to_string(oSP) + " given your scale root of " + std::to_string(k.middleNote) +
                 " which your mapping does not assign. Please set your tuning constant "
                 "note to a mapped key.";
            throw TuningError(s);
        }
    }
    double tuningCenterPitchOffset;
    if (scalePositionOfTuningNote == 0)
        tuningCenterPitchOffset = 0;
    else
    {
        if (scalePositionOfTuningNote == -1 && allowTuningCenterOnUnmapped)
        {
            int low, high;
            bool octave_up = false;
            bool octave_down = false;
            float pitch_high;
            float pitch_low;
            // find next closest mapped note
            for (int i = oSP - 1; i != oSP; i = (i - 1) % k.count)
            {
                if (k.keys[i] != -1)
                {
                    low = k.keys[i];
                    break;
                }

                if (i > oSP)
                {
                    octave_down = true;
                }
            }
            for (int i = oSP + 1; i != oSP; i = (i + 1) % k.count)
            {
                if (k.keys[i] != -1)
                {
                    high = k.keys[i];
                    break;
                }

                if (i < oSP)
                {
                    octave_up = true;
                }
            }

            // determine high and low pitches
            double dt = s.tones[s.count - 1].cents;
            pitch_low =
                octave_down ? s.tones[low - 1].cents - dt : s.tones[low - 1].floatValue - 1.0;
            pitch_high =
                octave_up ? s.tones[high - 1].cents + dt : s.tones[high - 1].floatValue - 1.0;
            tuningCenterPitchOffset = (pitch_high + pitch_low) / 2.f;
        }
        else
        {
            double tshift = 0;
            double dt = s.tones[s.count - 1].floatValue - 1.0;
            while (scalePositionOfTuningNote < 0)
            {
                scalePositionOfTuningNote += s.count;
                tshift += dt;
            }
            while (scalePositionOfTuningNote > s.count)
            {
                scalePositionOfTuningNote -= s.count;
                tshift -= dt;
            }

            if (scalePositionOfTuningNote == 0)
                tuningCenterPitchOffset = -tshift;
            else
                tuningCenterPitchOffset =
                    s.tones[scalePositionOfTuningNote - 1].floatValue - 1.0 - tshift;
        }
    }

    double pitches[N];

    for (int i = 0; i < N; ++i)
    {
        // TODO: ScaleCenter and PitchCenter are now two different notes.
        int distanceFromPitch0 = i - posPitch0;
        int distanceFromScale0 = i - posScale0;

        if (distanceFromPitch0 == 0)
        {
            pitches[i] = 1;
            lptable[i] = pitches[i] + pitchMod;
            ptable[i] = pow(2.0, lptable[i]);

            if (k.count > 0)
            {
                int mappingKey = distanceFromScale0 % k.count;
                if (mappingKey < 0)
                    mappingKey += k.count;

                int cm = k.keys[mappingKey];
                if (!allowTuningCenterOnUnmapped && cm < 0)
                {
                    std::string s = "Keyboard mapping is tuning an unmapped key. ";
                    s += "Your tuning mapping is mapping key " + std::to_string(posPitch0 - 256) +
                         " as " + "the tuning constant note, but that is scale note " +
                         std::to_string(mappingKey) + " given your scale root of " +
                         std::to_string(k.middleNote) +
                         " which your mapping does not assign. Please set your tuning constant "
                         "note to a mapped key.";
                    throw TuningError(s);
                }
            }
            scalepositiontable[i] = scalePositionOfTuningNote % s.count;
#if DEBUG_SCALES
            std::cout << "PITCH: i=" << i << " n=" << i - 256 << " p=" << pitches[i]
                      << " lp=" << lptable[i] << " tp=" << ptable[i]
                      << " fr=" << ptable[i] * 8.175798915 << std::endl;
#endif
        }
        else
        {
            /*
              We used to have this which assumed 1-12
              Now we have our note number, our distance from the
              center note, and the key remapping
              int rounds = (distanceFromScale0-1) / s.count;
              int thisRound = (distanceFromScale0-1) % s.count;
            */

            int rounds;
            int thisRound;
            int disable = false;
            if (k.count == 0)
            {
                rounds = (distanceFromScale0 - 1) / s.count;
                thisRound = (distanceFromScale0 - 1) % s.count;
            }
            else
            {
                /*
                ** Now we have this situation. We are at note i so we
                ** are m away from the center note which is distanceFromScale0
                **
                ** If we mod that by the mapping size we know which note we are on
                */
                int mappingKey = distanceFromScale0 % k.count;
                int rotations = 0;
                if (mappingKey < 0)
                {
                    mappingKey += k.count;
                }
                // Now have we gone off the end
                int dt = distanceFromScale0;
                if (dt > 0)
                {
                    while (dt >= k.count)
                    {
                        dt -= k.count;
                        rotations++;
                    }
                }
                else
                {
                    while (dt < 0)
                    {
                        dt += k.count;
                        rotations--;
                    }
                }

                int cm = k.keys[mappingKey];

                int push = 0;
                if (cm < 0)
                {
                    disable = true;
                }
                else
                {
                    if (cm > s.count)
                    {
                        throw TuningError(std::string(
                            "Mapping KBM note longer than scale; key=" + std::to_string(cm) +
                            " scale count=" + std::to_string(s.count)));
                    }
                    push = mappingKey - cm;
                }

                if (k.octaveDegrees > 0 && k.octaveDegrees != k.count)
                {
                    rounds = rotations;
                    thisRound = cm - 1;
                    if (thisRound < 0)
                    {
                        thisRound = k.octaveDegrees - 1;
                        rounds--;
                    }
                }
                else
                {
                    rounds = (distanceFromScale0 - push - 1) / s.count;
                    thisRound = (distanceFromScale0 - push - 1) % s.count;
                }

#ifdef DEBUG_SCALES
                if (i > 256 + 53 && i < 265 + 85)
                    std::cout << "MAPPING n=" << i - 256 << " pushes ds0=" << distanceFromScale0
                              << " cmc=" << k.count << " tr=" << thisRound << " r=" << rounds
                              << " mk=" << mappingKey << " cm=" << cm << " push=" << push
                              << " dis=" << disable << " mk-p-1=" << mappingKey - push - 1
                              << " rotations=" << rotations << " od=" << k.octaveDegrees
                              << std::endl;
#endif
            }

            if (thisRound < 0)
            {
                thisRound += s.count;
                rounds -= 1;
            }

            if (disable)
            {
                pitches[i] = 0;
                scalepositiontable[i] = -1;
            }
            else
            {
                pitches[i] = s.tones[thisRound].floatValue +
                             rounds * (s.tones[s.count - 1].floatValue - 1.0) -
                             tuningCenterPitchOffset;
                scalepositiontable[i] = (thisRound + 1) % s.count;
            }

            lptable[i] = pitches[i] + pitchMod;
            ptable[i] = pow(2.0, pitches[i] + pitchMod);

#if DEBUG_SCALES
            if (i > 296 && i < 340)
                std::cout << "PITCH: i=" << i << " n=" << i - 256 << " ds0=" << distanceFromScale0
                          << " dp0=" << distanceFromPitch0 << " r=" << rounds << " t=" << thisRound
                          << " p=" << pitches[i] << " t=" << s.tones[thisRound].floatValue
                          << " c=" << s.tones[thisRound].cents << " dis=" << disable
                          << " tp=" << ptable[i] << " fr=" << ptable[i] * 8.175798915 << " tcpo="
                          << tuningCenterPitchOffset

                          //<< " l2p=" << log(otp)/log(2.0)
                          //<< " l2p-p=" << log(otp)/log(2.0) - pitches[i] - rounds - 3
                          << std::endl;
#endif
        }
    }

    /*
     * Finally we may have constructed an invalid tuning
     */
}

inline double Tuning::frequencyForMidiNote(int mn) const
{
    auto mni = std::min(std::max(0, mn + 256), N - 1);
    return ptable[mni] * MIDI_0_FREQ;
}

inline double Tuning::frequencyForMidiNoteScaledByMidi0(int mn) const
{
    auto mni = std::min(std::max(0, mn + 256), N - 1);
    return ptable[mni];
}

inline double Tuning::logScaledFrequencyForMidiNote(int mn) const
{
    auto mni = std::min(std::max(0, mn + 256), N - 1);
    return lptable[mni];
}

inline double Tuning::retuningFromEqualInCentsForMidiNote(int mn) const
{
    return retuningFromEqualInSemitonesForMidiNote(mn) * 100.0;
}
inline double Tuning::retuningFromEqualInSemitonesForMidiNote(int mn) const
{
    return logScaledFrequencyForMidiNote(mn) * 12 - mn;
}

inline int Tuning::scalePositionForMidiNote(int mn) const
{
    auto mni = std::min(std::max(0, mn + 256), N - 1);
    return scalepositiontable[mni];
}

inline bool Tuning::isMidiNoteMapped(int mn) const
{
    auto mni = std::min(std::max(0, mn + 256), N - 1);
    return scalepositiontable[mni] >= 0;
}

inline int Tuning::midiNoteForNoteName(std::string noteName, int octave) const
{
    const auto it = std::find(notationMapping.names.begin(), notationMapping.names.end(), noteName);
    if (it == notationMapping.names.end())
    {
        std::string s = "Invalid note name '" + noteName + "'";
        throw TuningError(s);
    }
    int scalePosition = positive_mod(it - notationMapping.names.begin() + 1, notationMapping.count);
    int referencePitchOctave = ceil((keyboardMapping.middleNote - 21) / 12);
    return std::min(
        std::max(0, scalePosition + keyboardMapping.middleNote +
                        keyboardMapping.octaveDegrees * (octave - referencePitchOctave)),
        N - 1);
}

inline std::string Tuning::noteNameForScalePosition(int scalePosition) const
{
    return notationMapping.names.at(positive_mod(scalePosition - 1, notationMapping.count));
}

inline Tuning Tuning::withSkippedNotesInterpolated() const
{
    Tuning res = *this;
    for (int i = 1; i < N - 1; ++i)
    {
        if (scalepositiontable[i] < 0)
        {
            int nxt = i + 1;
            int prv = i - 1;
            while (prv >= 0 && scalepositiontable[prv] < 0)
                prv--;
            while (nxt < N && scalepositiontable[nxt] < 0)
                nxt++;
            float dist = (float)(nxt - prv);
            float frac = (float)(i - prv) / dist;
            res.lptable[i] = (1.0 - frac) * lptable[prv] + frac * lptable[nxt];
            res.ptable[i] = pow(2.0, res.lptable[i]);
        }
    }
    return res;
}

inline KeyboardMapping::KeyboardMapping()
    : count(0), firstMidi(0), lastMidi(127), middleNote(60), tuningConstantNote(60),
      tuningFrequency(MIDI_0_FREQ * 32.0), tuningPitch(32.0), octaveDegrees(0), rawText(""),
      name("")
{
    std::ostringstream oss;
    oss.imbue(std::locale("C"));
    oss << "! Default KBM file\n";
    oss << count << "\n"
        << firstMidi << "\n"
        << lastMidi << "\n"
        << middleNote << "\n"
        << tuningConstantNote << "\n"
        << tuningFrequency << "\n"
        << octaveDegrees << "\n";
    rawText = oss.str();
}

inline KeyboardMapping tuneA69To(double freq) { return tuneNoteTo(69, freq); }

inline KeyboardMapping tuneNoteTo(int midiNote, double freq)
{
    return startScaleOnAndTuneNoteTo(60, midiNote, freq);
}

inline KeyboardMapping startScaleOnAndTuneNoteTo(int scaleStart, int midiNote, double freq)
{
    std::ostringstream oss;
    oss.imbue(std::locale("C"));
    oss << "! Automatically generated mapping, tuning note " << midiNote << " to " << freq
        << " Hz\n"
        << "!\n"
        << "! Size of map\n"
        << 0 << "\n"
        << "! First and last MIDI notes to map - map the entire keyboard\n"
        << 0 << "\n"
        << 127 << "\n"
        << "! Middle note where the first entry in the scale is mapped.\n"
        << scaleStart << "\n"
        << "! Reference note where frequency is fixed\n"
        << midiNote << "\n"
        << "! Frequency for MIDI note " << midiNote << "\n"
        << freq << "\n"
        << "! Scale degree for formal octave. This is an empty mapping, so:\n"
        << 0 << "\n"
        << "! Mapping. This is an empty mapping so list no keys\n";

    return parseKBMData(oss.str());
}

inline AbletonScale readASCLStream(std::istream &inf)
{
    AbletonScale as;

    /**
     * Reverse engineering Ableton Tuning web app to understand ASCL to KBM export.
     *
     * @see https://tuning.ableton.com/squigadooServer/squigadoo/modular-playground
     *
     * Start tracing at `scalaKbmFile`.
     */

    // Read the scale and create default KBM parameters
    as.scale = readSCLStream(inf);
    as.keyboardMapping.count = as.scale.count;
    as.keyboardMapping.firstMidi = 0;
    as.keyboardMapping.lastMidi = 127;
    as.keyboardMapping.middleNote = as.midiNoteForScalePosition(0);
    as.keyboardMapping.tuningConstantNote = as.midiNoteForScalePosition(0);
    as.keyboardMapping.octaveDegrees = as.keyboardMapping.count;
    as.keyboardMapping.keys = std::vector<int>(as.keyboardMapping.count);
    std::iota(as.keyboardMapping.keys.begin(), as.keyboardMapping.keys.end(), 0);

    // Parse the scale comments to detect @ABL extensions
    for (const auto &comment : as.scale.comments)
    {
        std::smatch command;
        if (!std::regex_match(comment, command, std::regex("!\\s+@ABL\\s+(.*?)\\s+(.*?)$")))
            continue;
        as.rawTexts.push_back(command[0]);
        if (command[1] == "NOTE_NAMES")
        {
            std::string rawText = command[2];
            std::smatch note_names;
            std::regex note_name_regex("\\s*(?:\"(\\S+)\"|(\\S+))\\s*");
            std::string::const_iterator search_start(rawText.cbegin());
            while (std::regex_search(search_start, rawText.cend(), note_names, note_name_regex))
            {
                as.notationMapping.names.push_back(std::string(note_names[1]) +
                                                   std::string(note_names[2]));
                search_start = note_names.suffix().first;
            }

            // Move first note to last to correspond to scale.tones
            std::rotate(as.notationMapping.names.begin(), as.notationMapping.names.begin() + 1,
                        as.notationMapping.names.end());

            as.notationMapping.count = as.notationMapping.names.size();
            if (as.notationMapping.count != as.scale.count)
            {
                std::string s = "Invalid NOTE_NAMES entry '" + rawText + "': Expecting " +
                                std::to_string(as.scale.count) + " entries but received " +
                                std::to_string(as.notationMapping.count);
                throw TuningError(s);
            }
        }
        else if (command[1] == "REFERENCE_PITCH")
        {
            std::string rp = command[2];
            std::smatch reference_pitch;
            if (std::regex_match(rp, reference_pitch,
                                 std::regex("\\s*(\\d+)\\s*(\\d+)\\s*([\\d.]+)\\s*$")))
            {
                as.referencePitchOctave = std::stoi(reference_pitch[1]);
                as.referencePitchIndex = std::stoi(reference_pitch[2]);
                as.referencePitchFreq = locale_atof(reference_pitch.str(3).c_str());
                as.keyboardMapping.tuningFrequency = as.referencePitchFreq;
                as.keyboardMapping.tuningPitch = as.keyboardMapping.tuningFrequency / MIDI_0_FREQ;
                as.keyboardMapping.tuningConstantNote =
                    as.midiNoteForScalePosition(as.referencePitchIndex);
                as.keyboardMapping.middleNote = as.midiNoteForScalePosition(0);
            }
            else
            {
                std::string s = "Invalid REFERENCE_PITCH entry '" + rp + "'";
                throw TuningError(s);
            }
        }
        else if (command[1] == "NOTE_RANGE_BY_FREQUENCY")
        {
            // TODO
        }
        else if (command[1] == "NOTE_RANGE_BY_INDEX")
        {
            // TODO
        }
        else if (command[1] == "SOURCE")
        {
            as.source = command[2];
        }
        else if (command[1] == "LINK")
        {
            as.link = command[2];
        }
        else
        {
            std::string s = "Unhandled Ableton command '" + std::string(command[1]) + "'";
            throw TuningError(s);
        }
    }

    return as;
}

inline int AbletonScale::midiNoteForScalePosition(int scalePosition)
{
    auto middleFreq = MIDI_0_FREQ * (1 << 5);
    auto middleIndex = scalePositionForFrequency(middleFreq);
    return std::max(0, std::min(60 + (scalePosition - middleIndex), 127));
}

inline int AbletonScale::scalePositionForFrequency(double freq)
{
    auto n = 0;
    auto r = frequencyForScalePosition(n);
    auto o = freq - r;
    auto i = o > 0 ? 1 : -1;
    auto s = std::abs(o);
    auto a = n;
    auto l = false;
    if (s <= std::numeric_limits<double>::epsilon())
        return n;
    while (!l)
    {
        n += i;
        r = frequencyForScalePosition(n);
        o = std::abs(freq - r);
        if (o < s)
        {
            s = o;
            a = n;
        }
        if (i > 0)
            l = r > freq;
        else
            l = r < freq;
    }
    return a;
}

inline double AbletonScale::frequencyForScalePosition(int scalePosition)
{
    return referencePitchFreq * pow(2, (centsForScalePosition(scalePosition) -
                                        centsForScalePosition(referencePitchIndex)) /
                                           1200);
}

inline double AbletonScale::centsForScalePosition(int scalePosition)
{
    auto n = scale.tones.size();
    auto t = scale.tones[positive_mod(scalePosition, n)];
    return t.cents + floor(1.0 * scalePosition / n) * scale.tones.back().cents;
}

inline AbletonScale readASCLFile(std::string fname)
{
    std::ifstream inf;
    inf.open(fname);
    if (!inf.is_open())
    {
        std::string s = "Unable to open file '" + fname + "'";
        throw TuningError(s);
    }

    auto res = readASCLStream(inf);
    res.scale.name = fname;
    return res;
}

inline AbletonScale parseASCLData(const std::string &d)
{
    std::istringstream iss(d);
    auto res = readASCLStream(iss);
    res.scale.name = "AbletonScale from patch";
    return res;
}

} // namespace Tunings
#endif
