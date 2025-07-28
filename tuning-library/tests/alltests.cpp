#define CATCH_CONFIG_RUNNER
#include "catch2.hpp"

#include "Tunings.h"
#include <iomanip>
#include <vector>
#include <locale>
#include <ctime>

/*
** ToDo
** tuning with non-contiguous kbm
** tuning with non-monotonic kbm
** few known tunings across the whole spectrun
*/

std::string testFile(std::string fn) { return std::string("tests/data/") + fn; }

std::vector<std::string> testSCLs()
{
    std::vector<std::string> res = {{"12-intune.scl", "12-shuffled.scl", "31edo.scl", "6-exact.scl",
                                     "marvel12.scl", "zeus22.scl", "ED4-17.scl", "ED3-17.scl",
                                     "31edo_dos_lineends.scl"}};
    return res;
}

std::vector<std::string> testKBMs()
{
    std::vector<std::string> res = {{"empty-note61.kbm", "empty-note69.kbm",
                                     "mapping-a440-constant.kbm", "mapping-a442-7-to-12.kbm",
                                     "mapping-whitekeys-a440.kbm", "mapping-whitekeys-c261.kbm",
                                     "shuffle-a440-constant.kbm"}};
    return res;
}

TEST_CASE("Loading tuning files")
{
    SECTION("Load a 12 tone standard tuning")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune.scl"));
        REQUIRE(s.count == 12);
        // FIXME - write a lot more here obviously
    }

    SECTION("Load a 12 tone standard tuning with no description")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune-nodesc.scl"));
        REQUIRE(s.count == 12);
        // FIXME - write a lot more here obviously
    }

    SECTION("KBM File from text")
    {
        std::ostringstream oss;
        oss << "! A scale file\n"
            << "! with zero size\n"
            << "0\n"
            << "! spanning the keybaord\n"
            << "0\n"
            << "127\n"
            << "! With C60 as constant and A as 452\n"
            << "60\n69\n452\n"
            << "! and an octave might as well be zero\n"
            << "0\n";

        REQUIRE_NOTHROW(Tunings::parseKBMData(oss.str()));
    }

    SECTION("Comments read properly")
    {
        auto s = Tunings::readSCLFile(testFile("rast.ascl"));
        REQUIRE(s.comments.size() == 24);
    }
}

TEST_CASE("Identity Tuning Tests")
{
    SECTION("12-intune tunes properly")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune.scl"));
        REQUIRE(s.count == 12);
        Tunings::Tuning t(s);
        REQUIRE(t.frequencyForMidiNote(69) == Approx(440.0).margin(1e-10));
        REQUIRE(t.frequencyForMidiNoteScaledByMidi0(60) == 32.0);
        REQUIRE(t.logScaledFrequencyForMidiNote(60) == 5.0);
    }

    SECTION("12-intune doubles properly")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune.scl"));
        Tunings::Tuning t(s);
        for (int i = 0; i < 12; ++i)
        {
            int note = -12 * 4 + i;
            auto sc = t.frequencyForMidiNoteScaledByMidi0(note);
            auto lc = t.logScaledFrequencyForMidiNote(note);
            while (note < 200)
            {
                note += 12;
                auto nlc = t.logScaledFrequencyForMidiNote(note);
                auto nsc = t.frequencyForMidiNoteScaledByMidi0(note);
                REQUIRE(nsc == Approx(sc * 2).margin(1e-8));
                REQUIRE(nlc == Approx(lc + 1).margin(1e-8));
                sc = nsc;
                lc = nlc;
            }
        }
    }

    SECTION("Scaling is constant")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune.scl"));
        Tunings::Tuning t(s);
        auto f60 = t.frequencyForMidiNote(60);
        auto fs60 = t.frequencyForMidiNoteScaledByMidi0(60);
        for (int i = -200; i < 200; ++i)
        {
            auto f = t.frequencyForMidiNote(i);
            auto fs = t.frequencyForMidiNoteScaledByMidi0(i);
            REQUIRE(f / fs == f60 / fs60);
        }
    }
}

TEST_CASE("Simple Keyboard Remapping Tunes A69")
{
    SECTION("A440")
    {
        auto k = Tunings::tuneA69To(440.0);
        Tunings::Tuning t(k);
        REQUIRE(t.frequencyForMidiNote(69) == Approx(440.0).margin(1e-10));
        REQUIRE(t.frequencyForMidiNote(60) == Approx(261.625565301).margin(1e-10));
    }

    SECTION("A432")
    {
        auto k = Tunings::tuneA69To(432.0);
        Tunings::Tuning t(k);
        REQUIRE(t.frequencyForMidiNote(69) == Approx(432.0).margin(1e-10));
        REQUIRE(t.frequencyForMidiNote(60) == Approx(261.625565301 * 432 / 440).margin(1e-10));
    }

    SECTION("Random As Scale Consistently")
    {
        Tunings::Tuning ut;

        for (int i = 0; i < 100; ++i)
        {
            auto fr = 400 + 80.0 * rand() / RAND_MAX;

            auto k = Tunings::tuneA69To(fr);
            Tunings::Tuning t(k);
            REQUIRE(t.frequencyForMidiNote(69) == Approx(fr).margin(1e-10));
            REQUIRE(t.frequencyForMidiNote(60) == Approx(261.625565301 * fr / 440).margin(1e-10));

            double ldiff =
                t.logScaledFrequencyForMidiNote(69) - ut.logScaledFrequencyForMidiNote(69);
            double ratio = t.frequencyForMidiNote(69) / ut.frequencyForMidiNote(69);

            for (int j = -200; j < 200; ++j)
            {
                REQUIRE(t.logScaledFrequencyForMidiNote(j) - ut.logScaledFrequencyForMidiNote(j) ==
                        Approx(ldiff).margin(1e-8));
                REQUIRE(t.frequencyForMidiNote(69) / ut.frequencyForMidiNote(69) ==
                        Approx(ratio).margin(1e-8));
            }
        }
    }
}

TEST_CASE("Internal Constraints between Measures")
{
    SECTION("Test All Constraints SCL only")
    {
        for (auto f : testSCLs())
        {
            INFO("Testing Constraints with " << f);
            auto s = Tunings::readSCLFile(testFile(f));
            Tunings::Tuning t(s);

            for (int i = 0; i < 127; ++i)
            {
                REQUIRE(t.frequencyForMidiNote(i) ==
                        t.frequencyForMidiNoteScaledByMidi0(i) * Tunings::MIDI_0_FREQ);
                REQUIRE(t.frequencyForMidiNoteScaledByMidi0(i) ==
                        pow(2.0, t.logScaledFrequencyForMidiNote(i)));
            }
        }
    }

    SECTION("Test All Constraints KBM only")
    {
        for (auto f : testKBMs())
        {
            INFO("Testing Constraints with " << f);
            auto k = Tunings::readKBMFile(testFile(f));
            Tunings::Tuning t(k);

            for (int i = 0; i < 127; ++i)
            {
                REQUIRE(t.frequencyForMidiNote(i) ==
                        t.frequencyForMidiNoteScaledByMidi0(i) * Tunings::MIDI_0_FREQ);
                REQUIRE(t.frequencyForMidiNoteScaledByMidi0(i) ==
                        pow(2.0, t.logScaledFrequencyForMidiNote(i)));
            }
        }
    }

    SECTION("Test All Constraints SCL and KBM")
    {
        for (auto fs : testSCLs())
            for (auto fk : testKBMs())
            {
                INFO("Testing Constraints with " << fs << " " << fk);
                auto s = Tunings::readSCLFile(testFile(fs));
                auto k = Tunings::readKBMFile(testFile(fk));

                if (k.octaveDegrees > s.count)
                    continue; // don't test this verion; trap it below as an error case

                Tunings::Tuning t(s, k);

                for (int i = 0; i < 127; ++i)
                {
                    REQUIRE(t.frequencyForMidiNote(i) ==
                            t.frequencyForMidiNoteScaledByMidi0(i) * Tunings::MIDI_0_FREQ);
                    REQUIRE(t.frequencyForMidiNoteScaledByMidi0(i) ==
                            pow(2.0, t.logScaledFrequencyForMidiNote(i)));
                }
            }
    }

    SECTION("Test All Constraints SCL and KBM in Spain")
    {
        bool spainAvailable = true;
        try
        {
            std::locale("es_ES");
        }
        catch (std::exception &e)
        {
            INFO("es_ES locale not availale on this machine; skpping test");
            spainAvailable = false;
        }

        if (spainAvailable)
        {
            auto priorLocale = std::locale("").name();

            std::locale::global(std::locale("es_ES"));
            for (auto fs : testSCLs())
                for (auto fk : testKBMs())
                {
                    INFO("Testing Constraints with " << fs << " " << fk);
                    auto s = Tunings::readSCLFile(testFile(fs));
                    auto k = Tunings::readKBMFile(testFile(fk));

                    if (k.octaveDegrees > s.count)
                        continue; // don't test this verion; trap it below as an error case

                    Tunings::Tuning t(s, k);

                    for (int i = 0; i < 127; ++i)
                    {
                        REQUIRE(t.frequencyForMidiNote(i) ==
                                t.frequencyForMidiNoteScaledByMidi0(i) * Tunings::MIDI_0_FREQ);
                        REQUIRE(t.frequencyForMidiNoteScaledByMidi0(i) ==
                                pow(2.0, t.logScaledFrequencyForMidiNote(i)));
                    }
                }
            std::locale::global(std::locale(priorLocale));
        }
    }
}

TEST_CASE("Several Sample Scales")
{
    SECTION("Non Monotonic 12 note")
    {
        auto s = Tunings::readSCLFile(testFile("12-shuffled.scl"));
        Tunings::Tuning t(s);
        REQUIRE(s.count == 12);
        REQUIRE(t.logScaledFrequencyForMidiNote(60) == 5);

        std::vector<int> order = {{0, 2, 1, 3, 5, 4, 6, 7, 8, 10, 9, 11, 12}};
        auto l60 = t.logScaledFrequencyForMidiNote(60);
        for (size_t i = 0; i < order.size(); ++i)
        {
            auto li = t.logScaledFrequencyForMidiNote(60 + i);
            auto oi = order[i];
            REQUIRE(li - l60 == Approx(oi / 12.0).margin(1e-6));
        }
    }

    SECTION("31 edo")
    {
        auto s = Tunings::readSCLFile(testFile("31edo.scl"));
        Tunings::Tuning t(s);
        REQUIRE(s.count == 31);
        REQUIRE(t.logScaledFrequencyForMidiNote(60) == 5);

        auto prev = t.logScaledFrequencyForMidiNote(60);
        for (int i = 1; i < 31; ++i)
        {
            auto curr = t.logScaledFrequencyForMidiNote(60 + i);
            REQUIRE(curr - prev == Approx(1.0 / 31.0).margin(1e-6));
            prev = curr;
        }
    }

    SECTION("ED3-17")
    {
        auto s = Tunings::readSCLFile(testFile("ED3-17.scl"));
        Tunings::Tuning t(s);
        REQUIRE(s.count == 17);
        REQUIRE(t.logScaledFrequencyForMidiNote(60) == 5);

        auto prev = t.logScaledFrequencyForMidiNote(60);
        for (int i = 1; i < 40; ++i)
        {
            auto curr = t.logScaledFrequencyForMidiNote(60 + i);
            REQUIRE(pow(2.0, 17 * (curr - prev)) == Approx(3.0).margin(1e-6));
            prev = curr;
        }
    }

    SECTION("ED4-17")
    {
        auto s = Tunings::readSCLFile(testFile("ED4-17.scl"));
        Tunings::Tuning t(s);
        REQUIRE(s.count == 17);
        REQUIRE(t.logScaledFrequencyForMidiNote(60) == 5);

        auto prev = t.logScaledFrequencyForMidiNote(60);
        for (int i = 1; i < 40; ++i)
        {
            auto curr = t.logScaledFrequencyForMidiNote(60 + i);
            REQUIRE(pow(2.0, 17 * (curr - prev)) == Approx(4.0).margin(1e-6));
            prev = curr;
        }
    }

    SECTION("6 exact")
    {
        auto s = Tunings::readSCLFile(testFile("6-exact.scl"));
        Tunings::Tuning t(s);
        REQUIRE(s.count == 6);
        REQUIRE(t.logScaledFrequencyForMidiNote(60) == 5);

        std::vector<double> knownValues = {{0, 0.22239, 0.41504, 0.58496, 0.73697, 0.87447, 1.0}};

        for (size_t i = 0; i < knownValues.size(); ++i)
            REQUIRE(t.logScaledFrequencyForMidiNote(60 + i) ==
                    Approx(t.logScaledFrequencyForMidiNote(60) + knownValues[i]).margin(1e-5));
    }

    SECTION("Carlos Alpha (one step scale)")
    {
        auto s = Tunings::readSCLFile(testFile("carlos-alpha.scl"));
        Tunings::Tuning t(s);
        REQUIRE(s.count == 1);
        REQUIRE(t.logScaledFrequencyForMidiNote(60) == 5);
        auto diff = pow(2.0, 78.0 / 1200.0);
        for (int i = 30; i < 80; ++i)
        {
            REQUIRE(t.frequencyForMidiNoteScaledByMidi0(i) * diff ==
                    Approx(t.frequencyForMidiNoteScaledByMidi0(i + 1)).margin(1e-5));
        }
    }
}

TEST_CASE("Remapping frequency with non-12-length scales")
{
    SECTION("6 exact")
    {
        auto s = Tunings::readSCLFile(testFile("6-exact.scl"));
        Tunings::Tuning t(s);

        for (int i = 0; i < 100; ++i)
        {
            int mn = rand() % 40 + 40;
            double freq = 150 + 300.0 * rand() / RAND_MAX;
            INFO("Setting " << mn << " to " << freq);
            auto k = Tunings::tuneNoteTo(mn, freq);
            Tunings::Tuning mapped(s, k);

            REQUIRE(mapped.frequencyForMidiNote(mn) == Approx(freq).margin(1e-6));

            // This scale is monotonic so test monotonicity still
            for (int i = 1; i < 127; ++i)
            {
                INFO("About to test at " << i);
                if (mapped.frequencyForMidiNote(i) > 1)
                    REQUIRE(mapped.frequencyForMidiNote(i) > mapped.frequencyForMidiNote(i - 1));
            }

            double n60ldiff =
                t.logScaledFrequencyForMidiNote(60) - mapped.logScaledFrequencyForMidiNote(60);
            for (int j = 0; j < 128; ++j)
            {
                REQUIRE(t.logScaledFrequencyForMidiNote(j) -
                            mapped.logScaledFrequencyForMidiNote(j) ==
                        Approx(n60ldiff).margin(1e-6));
            }
        }
    }

    SECTION("31 edo")
    {
        auto s = Tunings::readSCLFile(testFile("31edo.scl"));
        Tunings::Tuning t(s);

        for (int i = 0; i < 100; ++i)
        {
            int mn = rand() % 20 + 50;
            double freq = 150 + 300.0 * rand() / RAND_MAX;
            INFO("Setting " << mn << " to " << freq);
            auto k = Tunings::tuneNoteTo(mn, freq);
            Tunings::Tuning mapped(s, k);

            REQUIRE(mapped.frequencyForMidiNote(mn) == Approx(freq).margin(1e-6));

            // This scale is monotonic so test monotonicity still
            for (int i = 1; i < 127; ++i)
            {
                INFO("About to test at " << i);
                if (mapped.frequencyForMidiNote(i) > 1)
                    REQUIRE(mapped.frequencyForMidiNote(i) > mapped.frequencyForMidiNote(i - 1));
            }

            double n60ldiff =
                t.logScaledFrequencyForMidiNote(60) - mapped.logScaledFrequencyForMidiNote(60);
            for (int j = 0; j < 128; ++j)
            {
                REQUIRE(t.logScaledFrequencyForMidiNote(j) -
                            mapped.logScaledFrequencyForMidiNote(j) ==
                        Approx(n60ldiff).margin(1e-6));
            }
        }
    }

    SECTION("ED4-17")
    {
        auto s = Tunings::readSCLFile(testFile("ED4-17.scl"));
        Tunings::Tuning t(s);

        for (int i = 0; i < 100; ++i)
        {
            int mn = rand() % 40 + 40;
            double freq = 150 + 300.0 * rand() / RAND_MAX;
            INFO("Setting " << mn << " to " << freq);
            auto k = Tunings::tuneNoteTo(mn, freq);
            Tunings::Tuning mapped(s, k);

            REQUIRE(mapped.frequencyForMidiNote(mn) == Approx(freq).margin(1e-6));

            // This scale is monotonic so test monotonicity still
            for (int i = 1; i < 127; ++i)
            {
                INFO("About to test at " << i);
                if (mapped.frequencyForMidiNote(i) > 1)
                    REQUIRE(mapped.frequencyForMidiNote(i) > mapped.frequencyForMidiNote(i - 1));
            }

            double n60ldiff =
                t.logScaledFrequencyForMidiNote(60) - mapped.logScaledFrequencyForMidiNote(60);
            for (int j = 0; j < 128; ++j)
            {
                REQUIRE(t.logScaledFrequencyForMidiNote(j) -
                            mapped.logScaledFrequencyForMidiNote(j) ==
                        Approx(n60ldiff).margin(1e-6));
            }
        }
    }

    SECTION("ED3-17")
    {
        auto s = Tunings::readSCLFile(testFile("ED3-17.scl"));
        Tunings::Tuning t(s);

        for (int i = 0; i < 100; ++i)
        {
            int mn = rand() % 40 + 40;
            double freq = 150 + 300.0 * rand() / RAND_MAX;
            INFO("Setting " << mn << " to " << freq);
            auto k = Tunings::tuneNoteTo(mn, freq);
            Tunings::Tuning mapped(s, k);

            REQUIRE(mapped.frequencyForMidiNote(mn) == Approx(freq).margin(1e-6));

            // This scale is monotonic so test monotonicity still
            for (int i = 1; i < 127; ++i)
            {
                INFO("About to test at " << i);
                if (mapped.frequencyForMidiNote(i) > 1)
                    REQUIRE(mapped.frequencyForMidiNote(i) > mapped.frequencyForMidiNote(i - 1));
            }

            double n60ldiff =
                t.logScaledFrequencyForMidiNote(60) - mapped.logScaledFrequencyForMidiNote(60);
            for (int j = 0; j < 128; ++j)
            {
                REQUIRE(t.logScaledFrequencyForMidiNote(j) -
                            mapped.logScaledFrequencyForMidiNote(j) ==
                        Approx(n60ldiff).margin(1e-6));
            }
        }
    }
}

TEST_CASE("KBMs with Gaps")
{
    SECTION("12 Intune with Gap")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune.scl"));
        auto k = Tunings::readKBMFile(testFile("mapping-whitekeys-c261.kbm"));
        Tunings::Tuning t(s);
        Tunings::Tuning tm(s, k);

        REQUIRE(s.count == 12);
        REQUIRE(t.frequencyForMidiNote(69) == Approx(440.0).margin(1e-6));

        // That KBM maps the white keys to the chromatic start so
        std::vector<std::pair<int, int>> maps = {{60, 60}, {61, 62}, {62, 64}, {63, 65},
                                                 {64, 67}, {65, 69}, {66, 71}};
        for (auto p : maps)
        {
            REQUIRE(t.logScaledFrequencyForMidiNote(p.first) ==
                    Approx(tm.logScaledFrequencyForMidiNote(p.second)).margin(1e-5));
        }
    }
}

TEST_CASE("Scala KBMs from Issue 42")
{
    SECTION("Piano.kbm")
    {
        auto k = Tunings::readKBMFile(testFile("piano.kbm"));
        REQUIRE(k.count == 0);
    }

    SECTION("128.kbm")
    {
        auto k = Tunings::readKBMFile(testFile("128.kbm"));
        REQUIRE(k.count == 0);
    }
}

TEST_CASE("KBM ReOrdering")
{
    SECTION("Non Monotonic KBM note")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune.scl"));
        auto k = Tunings::readKBMFile(testFile("shuffle-a440-constant.kbm"));
        Tunings::Tuning t(s, k);

        REQUIRE(s.count == 12);
        REQUIRE(t.frequencyForMidiNote(69) == Approx(440.0).margin(1e-6));

        std::vector<int> order = {{0, 2, 1, 3, 4, 6, 5, 7, 8, 9, 11, 10, 12}};
        auto l60 = t.logScaledFrequencyForMidiNote(60);
        for (size_t i = 0; i < order.size(); ++i)
        {
            INFO("Testing note " << i);
            auto li = t.logScaledFrequencyForMidiNote(60 + i);
            auto oi = order[i];
            REQUIRE(li - l60 == Approx(oi / 12.0).margin(1e-6));
        }
    }
}

TEST_CASE("Exceptions and Bad Files")
{
    SECTION("Read Non-present files")
    {
        REQUIRE_THROWS_AS(Tunings::readSCLFile("blahlfdsfds"), Tunings::TuningError);
        REQUIRE_THROWS_AS(Tunings::readKBMFile("blahlfdsfds"), Tunings::TuningError);

        // Lets make sure what is reasonable
        try
        {
            Tunings::readSCLFile("MISSING");
        }
        catch (const Tunings::TuningError &e)
        {
            REQUIRE(std::string(e.what()) == "Unable to open file 'MISSING'");
        }

        try
        {
            Tunings::readKBMFile("MISSING");
        }
        catch (const Tunings::TuningError &e)
        {
            REQUIRE(std::string(e.what()) == "Unable to open file 'MISSING'");
        }
    }

    SECTION("Mappings bigger than Scales Throw")
    {
        bool testedAtLeastOne = false;
        for (auto fs : testSCLs())
            for (auto fk : testKBMs())
            {
                INFO("Looking for mis-sized pairs " << fs << " " << fk);
                auto s = Tunings::readSCLFile(testFile(fs));
                auto k = Tunings::readKBMFile(testFile(fk));

                if (k.octaveDegrees <= s.count)
                    continue;

                testedAtLeastOne = true;
                REQUIRE_THROWS_AS(Tunings::Tuning(s, k), Tunings::TuningError);
            }
        REQUIRE(testedAtLeastOne);
    }

    SECTION("Bad SCL")
    {
        // Trailing data is OK
        REQUIRE_NOTHROW(Tunings::readSCLFile(testFile("bad/extraline.scl")));

        REQUIRE_THROWS_AS(Tunings::readSCLFile(testFile("bad/badnote.scl")), Tunings::TuningError);
        REQUIRE_THROWS_AS(Tunings::readSCLFile(testFile("bad/blanknote.scl")),
                          Tunings::TuningError);
        REQUIRE_THROWS_AS(Tunings::readSCLFile(testFile("bad/missingnote.scl")),
                          Tunings::TuningError);
    }

    SECTION("Bad KBM")
    {
        REQUIRE_THROWS_AS(Tunings::readKBMFile(testFile("bad/blank-line.kbm")),
                          Tunings::TuningError);
        REQUIRE_THROWS_AS(Tunings::readKBMFile(testFile("bad/empty-bad.kbm")),
                          Tunings::TuningError);
        REQUIRE_THROWS_AS(Tunings::readKBMFile(testFile("bad/garbage-key.kbm")),
                          Tunings::TuningError);
        REQUIRE_NOTHROW(Tunings::readKBMFile(testFile("bad/empty-extra.kbm")));
        REQUIRE_NOTHROW(Tunings::readKBMFile(testFile("bad/extraline-long.kbm")));
        REQUIRE_THROWS_AS(Tunings::readKBMFile(testFile("bad/missing-note.kbm")),
                          Tunings::TuningError);
    }

    SECTION("Bad SCL Data")
    {
        REQUIRE_THROWS_AS(Tunings::parseSCLData(""), Tunings::TuningError);

        try
        {
            Tunings::parseSCLData("");
            REQUIRE(false);
        }
        catch (const Tunings::TuningError &e)
        {
            auto s = std::string(e.what());
            INFO(s);
            REQUIRE(s.find("Incomplete SCL") != std::string::npos);
            REQUIRE(s.find("Only able to read 0 lines") != std::string::npos);
        }
    }

    SECTION("Bad KBM Data")
    {
        REQUIRE_THROWS_AS(Tunings::parseKBMData(""), Tunings::TuningError);

        try
        {
            Tunings::parseKBMData("");
            REQUIRE(false);
        }
        catch (const Tunings::TuningError &e)
        {
            auto s = std::string(e.what());
            INFO(s);
            REQUIRE(s.find("Incomplete KBM") != std::string::npos);
            REQUIRE(s.find("Only able to read 0 lines") != std::string::npos);
        }
    }
}

TEST_CASE("Built in Generators")
{
    SECTION("ED2")
    {
        auto s = Tunings::evenDivisionOfSpanByM(2, 12);
        REQUIRE(s.count == 12);
        REQUIRE(s.rawText.size() > 1);
        Tunings::Tuning ut;
        Tunings::Tuning t(s);
        for (int i = 0; i < 128; ++i)
            REQUIRE(t.logScaledFrequencyForMidiNote(i) == ut.logScaledFrequencyForMidiNote(i));
    }

    SECTION("ED3-17")
    {
        auto s = Tunings::evenDivisionOfSpanByM(3, 17);
        auto sf = Tunings::readSCLFile(testFile("ED3-17.scl"));

        Tunings::Tuning ut(sf);
        Tunings::Tuning t(s);
        for (int i = 0; i < 128; ++i)
            REQUIRE(t.logScaledFrequencyForMidiNote(i) ==
                    Approx(ut.logScaledFrequencyForMidiNote(i)).margin(1e-6));
    }

    SECTION("ED4-17")
    {
        auto s = Tunings::evenDivisionOfSpanByM(4, 17);
        auto sf = Tunings::readSCLFile(testFile("ED4-17.scl"));

        Tunings::Tuning ut(sf);
        Tunings::Tuning t(s);
        for (int i = 0; i < 128; ++i)
            REQUIRE(t.logScaledFrequencyForMidiNote(i) ==
                    Approx(ut.logScaledFrequencyForMidiNote(i)).margin(1e-6));
    }

    SECTION("Constraints on random EDN-M")
    {
        for (int i = 0; i < 100; ++i)
        {
            int Span = rand() % 7 + 2;
            int M = rand() % 50 + 3;
            INFO("Constructing " << i << " scale ED " << Span << " - " << M);

            auto s = Tunings::evenDivisionOfSpanByM(Span, M);

            REQUIRE(s.count == M);
            REQUIRE(s.rawText.size() > 1);

            Tunings::Tuning t(s);
            REQUIRE(t.frequencyForMidiNoteScaledByMidi0(60) * Span ==
                    Approx(t.frequencyForMidiNoteScaledByMidi0(60 + M)).margin(1e-7));

            auto d0 = t.logScaledFrequencyForMidiNote(1) - t.logScaledFrequencyForMidiNote(0);
            for (auto i = 1; i < 128; ++i)
            {
                auto d =
                    t.logScaledFrequencyForMidiNote(i) - t.logScaledFrequencyForMidiNote(i - 1);
                REQUIRE(d == Approx(d0).margin(1e-7));
            }
        }
    }

    SECTION("EDMN Errors")
    {
        REQUIRE_THROWS_AS(Tunings::evenDivisionOfSpanByM(0, 12), Tunings::TuningError);
        REQUIRE_THROWS_AS(Tunings::evenDivisionOfSpanByM(2, 0), Tunings::TuningError);
        REQUIRE_THROWS_AS(Tunings::evenDivisionOfSpanByM(0, 0), Tunings::TuningError);

        REQUIRE_THROWS_AS(Tunings::evenDivisionOfSpanByM(-1, 12), Tunings::TuningError);
        REQUIRE_THROWS_AS(Tunings::evenDivisionOfSpanByM(2, -1), Tunings::TuningError);
        REQUIRE_THROWS_AS(Tunings::evenDivisionOfSpanByM(-1, -1), Tunings::TuningError);
    }

    SECTION("KBM Generator")
    {
        for (int i = 0; i < 100; ++i)
        {
            int n = rand() % 60 + 30;
            int fr = 1000.0 * rand() / RAND_MAX + 50;
            auto k = Tunings::tuneNoteTo(n, fr);
            REQUIRE(k.tuningConstantNote == n);
            REQUIRE(k.tuningFrequency == fr);
            REQUIRE(k.tuningPitch == k.tuningFrequency / Tunings::MIDI_0_FREQ);
            REQUIRE(k.rawText.size() > 1);
        }
    }
}

TEST_CASE("Dos Line Endings and Blanks")
{
    SECTION("SCL") { REQUIRE_NOTHROW(Tunings::readSCLFile(testFile("12-intune-dosle.scl"))); }

    SECTION("Properly read a file with DOS line endings")
    {
        auto s = Tunings::readSCLFile(testFile("31edo_dos_lineends.scl"));
        REQUIRE(s.count == 31);
        INFO("If coded with std::getline this will contain a \\r on unixes")
        REQUIRE(s.description == "31 equal divisions of octave");

        // the parsing should ive the same floatvalues independent of crlf status obviously
        auto q = Tunings::readSCLFile(testFile("31edo.scl"));
        for (int i = 0; i < q.count; ++i)
        {
            REQUIRE(q.tones[i].floatValue == s.tones[i].floatValue);
        }
    }

    SECTION("KBM")
    {
        REQUIRE_NOTHROW(Tunings::readKBMFile(testFile("empty-note69-dosle.kbm")));
        auto k = Tunings::readKBMFile(testFile("empty-note69-dosle.kbm"));
        REQUIRE(k.tuningConstantNote == 69);
    }

    SECTION("Blank SCL")
    {
        REQUIRE_THROWS_AS(Tunings::parseSCLData(""), Tunings::TuningError);

        // but what if we do construct a bad one?
        Tunings::Scale s;
        s.count = 0;
        s.tones.clear();
        REQUIRE_THROWS_AS(Tunings::Tuning(s), Tunings::TuningError);
    }
}

TEST_CASE("Tone API")
{
    // This is exercised a million times above so just a light test here
    SECTION("Valid Tones")
    {
        auto t1 = Tunings::toneFromString("130.0");
        REQUIRE(t1.type == Tunings::Tone::kToneCents);
        REQUIRE(t1.cents == 130.0);
        REQUIRE(t1.floatValue == 130.0 / 1200.0 + 1.0);

        auto t2 = Tunings::toneFromString("7/2");
        REQUIRE(t2.type == Tunings::Tone::kToneRatio);
        REQUIRE(t2.ratio_d == 2);
        REQUIRE(t2.ratio_n == 7);
        REQUIRE(t2.floatValue == Approx(log(7.0 / 2.0) / log(2.0) + 1.0).margin(1e-6));

        auto t3 = Tunings::toneFromString("3");
        REQUIRE(t3.type == Tunings::Tone::kToneRatio);
        REQUIRE(t3.ratio_d == 1);
        REQUIRE(t3.ratio_n == 3);
        REQUIRE(t3.floatValue == Approx(log(3.0 / 1.0) / log(2.0) + 1.0).margin(1e-6));

        auto t4 = Tunings::toneFromString("555/524 ! c# 138.75 Hz");
        REQUIRE(t4.type == Tunings::Tone::kToneRatio);
        REQUIRE(t4.ratio_d == 524);
        REQUIRE(t4.ratio_n == 555);
        REQUIRE(t4.floatValue == Approx(log(555.0 / 524.0) / log(2.0) + 1.0).margin(1e-6));
    }

    SECTION("Ridiculously Long Fraction Tones")
    {
        uint64_t top{3}, bottom{2};
        for (auto q = 0; q < 18; ++q)
        {
            auto frac = std::to_string(top) + "/" + std::to_string(bottom);
            INFO("Parsing " << frac << " at " << q);
            auto t = Tunings::toneFromString(frac);
            REQUIRE(t.type == Tunings::Tone::kToneRatio);
            REQUIRE(t.ratio_n == top);
            REQUIRE(t.ratio_d == bottom);
            top = top * 10;
            bottom = bottom * 10;
        }
    }

    SECTION("Error Tones")
    {
        REQUIRE_THROWS_AS(Tunings::toneFromString("Not a number"), Tunings::TuningError);
        // FIXME - these cases doesn't throw yet
        // REQUIRE_THROWS_AS( Tunings::toneFromString( "100.200 with extra stuff" ),
        // Tunings::TuningError ); REQUIRE_THROWS_AS( Tunings::toneFromString( "7/4/2" ),
        // Tunings::TuningError ); REQUIRE_THROWS_AS( Tunings::toneFromString( "7*2" ),
        // Tunings::TuningError );
    }
}

TEST_CASE("Scale Position")
{
    SECTION("Untuned")
    {
        Tunings::Tuning t;
        for (int i = 0; i < 127; ++i)
            REQUIRE(t.scalePositionForMidiNote(i) == i % 12);
    }

    SECTION("Untuned, Mapped")
    {
        {
            auto k = Tunings::startScaleOnAndTuneNoteTo(60, 69, 440);
            Tunings::Tuning t(k);

            for (int i = 0; i < 127; ++i)
            {
                INFO("Check " << i << " " << t.scalePositionForMidiNote(i));
                REQUIRE(t.scalePositionForMidiNote(i) == i % 12);
            }
        }
        for (int j = 0; j < 100; ++j)
        {
            int n = rand() % 60 + 30;
            auto k = Tunings::startScaleOnAndTuneNoteTo(n, 69, 440);
            Tunings::Tuning t(k);

            INFO("Checking scale position with 0 mapped to " << n << " " << n % 12);
            for (int i = 0; i < 127; ++i)
            {
                INFO("Check " << i << " " << t.scalePositionForMidiNote(i) << " with n=" << n);
                REQUIRE(t.scalePositionForMidiNote(i) == (i + 12 - n % 12) % 12);
            }
        }

        {
            // Check whitekeys
            auto k = Tunings::readKBMFile(testFile("mapping-whitekeys-c261.kbm"));
            Tunings::Tuning t(k);

            // That KBM maps the white keys to the chromatic start so
            std::vector<std::pair<int, int>> maps = {{0, 0}, {2, 1}, {4, 2}, {5, 3},
                                                     {7, 4}, {9, 5}, {11, 6}};
            for (int i = 0; i < 127; ++i)
            {
                auto spn = t.scalePositionForMidiNote(i);
                int expected = -1;
                for (auto p : maps)
                    if (i % 12 == p.first)
                        expected = p.second;
                INFO("Checking SPN at " << i << " " << expected << " " << spn);
                REQUIRE(spn == expected);
            }
        }
    }

    SECTION("Tuned, Unmapped")
    {
        // Check longer and shorter scales
        {
            auto s = Tunings::readSCLFile(testFile("zeus22.scl"));
            Tunings::Tuning t(s);

            int off = 60;
            while (off > 0)
                off -= s.count;

            for (int i = 0; i < 127; ++i)
            {
                INFO("Check " << i << " " << t.scalePositionForMidiNote(i) << off);
                REQUIRE(t.scalePositionForMidiNote(i) == (i - off) % s.count);
            }
        }

        // Check longer and shorter scales
        {
            auto s = Tunings::readSCLFile(testFile("6-exact.scl"));
            Tunings::Tuning t(s);

            int off = 60;
            while (off >= 0)
                off -= s.count;

            for (int i = 0; i < 127; ++i)
            {
                INFO("Check " << i << " " << t.scalePositionForMidiNote(i) << off);
                REQUIRE(t.scalePositionForMidiNote(i) == (i - off) % s.count);
            }
        }
    }

    SECTION("Tuned, Mapped")
    {
        // And check some combos
        for (int j = 0; j < 100; ++j)
        {
            int n = rand() % 60 + 30;

            auto s = Tunings::readSCLFile(testFile("zeus22.scl"));
            auto k = Tunings::startScaleOnAndTuneNoteTo(n, 69, 440);
            Tunings::Tuning t(s, k);

            int off = n;
            while (off > 0)
                off -= s.count;

            for (int i = 0; i < 127; ++i)
            {
                INFO("Check " << i << " " << t.scalePositionForMidiNote(i) << " " << off
                              << " n=" << n);
                REQUIRE(t.scalePositionForMidiNote(i) == (i - off) % s.count);
            }
        }
    }
}

TEST_CASE("Default KBM Constructor has Right Base")
{
    SECTION("All Scales with Default KBM")
    {
        for (auto scl : testSCLs())
        {
            INFO("Loading SCL " << scl);
            auto s = Tunings::readSCLFile(testFile(scl));
            Tunings::Tuning t(s);
            REQUIRE(t.frequencyForMidiNoteScaledByMidi0(60) == 32);
        }
    }
}

TEST_CASE("Different KBM period from Scale period")
{
    SECTION("31Edo with mean tone mapping")
    {
        /*
         * Even though we have a 31 note octave we have  12 key mapping.
         */
        auto s = Tunings::readSCLFile(testFile("31edo.scl"));
        auto k = Tunings::readKBMFile(testFile("31edo_meantone.kbm"));

        Tunings::Tuning t(s, k);
        REQUIRE(t.frequencyForMidiNote(69) == Approx(440.0));
        REQUIRE(t.frequencyForMidiNote(69 + 12) == Approx(880.0));
    }

    SECTION("Perfect 5th UnMapped")
    {
        auto s = Tunings::readSCLFile(testFile("12-ET-P5.scl"));
        Tunings::Tuning t(s);
        for (int i = 60 - 36; i < 127; i += 12)
        {
            INFO("Checking perfect 5th at " << i);
            auto f = t.frequencyForMidiNote(i);
            auto f5 = t.frequencyForMidiNote(i + 7);
            REQUIRE(f5 == Approx(f * 1.5).margin(1e-6));
        }
    }

    SECTION("Perfect 5th 07 mapping")
    {
        auto s = Tunings::readSCLFile(testFile("12-ET-P5.scl"));
        auto k = Tunings::readKBMFile(testFile("mapping-n60-fifths.kbm"));
        Tunings::Tuning t(s, k);

        for (int i = 60; i < 70; i += 2)
        {
            INFO("Checking perfect 5th at " << i);
            auto f = t.frequencyForMidiNote(i);
            auto f5 = t.frequencyForMidiNote(i + 1);
            REQUIRE(f5 == Approx(f * 1.5).margin(1e-6));
        }
    }
}

TEST_CASE("KBM Constructor RawText")
{
    SECTION("KBM")
    {
        auto k = Tunings::KeyboardMapping();
        INFO("Raw text is " << k.rawText);
        auto kparse = Tunings::parseKBMData(k.rawText);
        REQUIRE(k.count == kparse.count);
        REQUIRE(k.firstMidi == kparse.firstMidi);
        REQUIRE(k.lastMidi == kparse.lastMidi);
        REQUIRE(k.middleNote == kparse.middleNote);
        REQUIRE(k.tuningConstantNote == kparse.tuningConstantNote);
        REQUIRE(k.tuningFrequency == Approx(kparse.tuningFrequency));
        REQUIRE(k.octaveDegrees == kparse.octaveDegrees);
    }
}

TEST_CASE("Skipped Note API")
{
    SECTION("Default Tuning skips Nothing")
    {
        auto t = Tunings::Tuning();
        for (int i = 0; i < 128; ++i)
            REQUIRE(t.isMidiNoteMapped(i));
    }

    SECTION("SCL-only Tuning skips Nothing")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune.scl"));
        auto t = Tunings::Tuning(s);
        for (int i = 0; i < 128; ++i)
            REQUIRE(t.isMidiNoteMapped(i));
    }

    SECTION("KBM-only Tuning absent skips skips Nothing")
    {
        auto k = Tunings::readKBMFile(testFile("empty-note69.kbm"));
        auto t = Tunings::Tuning(k);
        for (int i = 0; i < 128; ++i)
            REQUIRE(t.isMidiNoteMapped(i));
    }

    SECTION("Fully Mapped")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune.scl"));
        auto k = Tunings::readKBMFile(testFile("empty-note69.kbm"));
        auto t = Tunings::Tuning(s, k);
        for (int i = 0; i < 128; ++i)
            REQUIRE(t.isMidiNoteMapped(i));
    }

    SECTION("Gaps in the Maps")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune.scl"));
        auto k = Tunings::readKBMFile(testFile("mapping-whitekeys-a440.kbm"));
        auto t = Tunings::Tuning(s, k);
        for (int k = 0; k < 128; ++k)
        {
            int i = k % 12;
            bool isOn = i == 0 || i == 2 || i == 4 || i == 5 || i == 7 || i == 9 || i == 11;
            INFO(k << " scpos=" << i << " isOn = " << isOn);
            REQUIRE(t.isMidiNoteMapped(k) == isOn);
        }
    }

    SECTION("Gaps in the Maps KBM Only")
    {
        auto k = Tunings::readKBMFile(testFile("mapping-whitekeys-a440.kbm"));
        auto t = Tunings::Tuning(k);
        for (int k = 0; k < 128; ++k)
        {
            int i = k % 12;
            bool isOn = i == 0 || i == 2 || i == 4 || i == 5 || i == 7 || i == 9 || i == 11;
            INFO(k << " scpos=" << i << " isOn = " << isOn);
            REQUIRE(t.isMidiNoteMapped(k) == isOn);
        }
    }

    SECTION("Tuning with Gaps")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune.scl"));
        auto k = Tunings::readKBMFile(testFile("mapping-whitekeys-a440.kbm"));
        auto t = Tunings::Tuning(s, k);
        for (int k = 2; k < 128; ++k)
        {
            int i = k % 12;
            bool isOn = i == 0 || i == 2 || i == 4 || i == 5 || i == 7 || i == 9 || i == 11;
            int priorOn = (i == 0 || i == 5) ? 1 : 2;
            INFO(k << " scpos=" << i << " isOn = " << isOn << " priorOn " << priorOn);

            if (isOn)
                REQUIRE(t.logScaledFrequencyForMidiNote(k) >
                        t.logScaledFrequencyForMidiNote(k - priorOn));
        }
    }

    SECTION("Tuning with Gaps and Interpolation")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune.scl"));
        auto k = Tunings::readKBMFile(testFile("mapping-whitekeys-a440.kbm"));
        auto t = Tunings::Tuning(s, k).withSkippedNotesInterpolated();
        for (int k = 2; k < 128; ++k)
        {
            // Now we have filled in, all the APIs should be monotonic
            INFO("Testing monotnicity note " << k);
            int i = k % 12;
            bool isOn = i == 0 || i == 2 || i == 4 || i == 5 || i == 7 || i == 9 || i == 11;
            REQUIRE(t.isMidiNoteMapped(k) == isOn);

            REQUIRE(t.logScaledFrequencyForMidiNote(k) > t.logScaledFrequencyForMidiNote(k - 1));
            REQUIRE(t.frequencyForMidiNote(k) > t.frequencyForMidiNote(k - 1));
            REQUIRE(t.frequencyForMidiNoteScaledByMidi0(k) >
                    t.frequencyForMidiNoteScaledByMidi0(k - 1));
        }
    }
}

TEST_CASE("Skipped Note and Root")
{
    SECTION("Tuning from 60 works")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune.scl"));
        auto k = Tunings::readKBMFile(testFile("mapping-whitekeys-a440.kbm"));
        auto t = Tunings::Tuning(s, k);
        REQUIRE(t.isMidiNoteMapped(60));
        REQUIRE(t.isMidiNoteMapped(69));
        REQUIRE(t.frequencyForMidiNote(69) == Approx(440.0).margin(0.01));
        REQUIRE(t.frequencyForMidiNote(60) == Approx(246.9416506281).margin(0.01));
    }

    SECTION("Tuning from 59 throws")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune.scl"));
        auto k = Tunings::readKBMFile(testFile("mapping-whitekeys-from-59-a440.kbm"));
        REQUIRE_THROWS(Tunings::Tuning(s, k));
    }

    SECTION("Tuning from 59 no throw")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune.scl"));
        auto k = Tunings::readKBMFile(testFile("mapping-whitekeys-from-59-a440.kbm"));
        auto t = Tunings::Tuning(s, k, true);
        REQUIRE(t.frequencyForMidiNote(59) == Approx(246.94).margin(0.01));
    }

    SECTION("Tuning from 59 altmapping")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune.scl"));
        auto k = Tunings::readKBMFile(testFile("mapping-whitekeysalt-from-59-a440.kbm"));
        REQUIRE_NOTHROW(Tunings::Tuning(s, k, true));
        REQUIRE_THROWS(Tunings::Tuning(s, k, false));
        REQUIRE_THROWS(Tunings::Tuning(s, k));
        auto t = Tunings::Tuning(s, k, true);
        REQUIRE(t.frequencyForMidiNote(59) == Approx(440.0 * pow(2.f, -(5.5 / 12))).margin(0.01));
    }

    SECTION("Tuning from 48 works")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune.scl"));
        auto k = Tunings::readKBMFile(testFile("mapping-whitekeys-from-48-a440.kbm"));
        auto t = Tunings::Tuning(s, k);
        REQUIRE(t.isMidiNoteMapped(60));
        REQUIRE(t.isMidiNoteMapped(69));
        REQUIRE(t.frequencyForMidiNote(60) == Approx(246.9416506281).margin(0.01));
        REQUIRE(t.frequencyForMidiNote(69) == Approx(440.0).margin(0.01));
    }

    SECTION("Tuning from 48 works with Interpolation")
    {
        auto s = Tunings::readSCLFile(testFile("12-intune.scl"));
        auto k = Tunings::readKBMFile(testFile("mapping-whitekeys-from-48-a440.kbm"));
        auto t = Tunings::Tuning(s, k);
        t = t.withSkippedNotesInterpolated();
        REQUIRE(t.isMidiNoteMapped(60));
        REQUIRE(t.isMidiNoteMapped(69));
        REQUIRE(t.frequencyForMidiNote(60) == Approx(246.9416506281).margin(0.01));
        REQUIRE(t.frequencyForMidiNote(69) == Approx(440.0).margin(0.01));
    }
}

TEST_CASE("Wrapped KBMs")
{
    for (const auto &kbm :
         {"empty-c-100.kbm", "fournote-c-100.kbm", "eightnote-c-100.kbm", "twelve-c-100.kbm"})
    {
        DYNAMIC_SECTION("Testing KBM " << kbm)
        {
            auto scale = Tunings::readSCLFile(testFile("kbm-fix-012023/exact4.scl"));
            auto map = Tunings::readKBMFile(testFile(std::string("kbm-fix-012023/") + kbm));
            auto tun = Tunings::Tuning(scale, map);

            REQUIRE(tun.frequencyForMidiNote(60) == 100.0);
            for (int i = 61; i < 76; ++i)
            {
                auto ni = tun.frequencyForMidiNote(i);
                auto np = tun.frequencyForMidiNote(i - 1);
                auto diff = i <= 64 ? 25 : (i <= 68 ? 50 : (i <= 72 ? 100 : 200));
                REQUIRE(ni > np);
                REQUIRE(ni - np == Approx(diff).margin(0.001));
            }
        }
    }
    DYNAMIC_SECTION("Skipper One")
    {
        auto scale = Tunings::readSCLFile(testFile("kbm-fix-012023/exact4.scl"));
        auto map = Tunings::readKBMFile(testFile("kbm-fix-012023/threenote-c-100.kbm"));
        auto tun = Tunings::Tuning(scale, map);
        int note = 60;
        for (const auto &v : {100.0, 125.0, 175., 200., 250., 350., 400.})
        {
            REQUIRE(tun.frequencyForMidiNote(note) == Approx(v).margin(0.001));
            note++;
        }
    }
    DYNAMIC_SECTION("Skipper 59")
    {
        auto scale = Tunings::readSCLFile(testFile("kbm-fix-012023/exact4.scl"));
        auto map = Tunings::readKBMFile(testFile("kbm-fix-012023/threenote-c-100-from-1.kbm"));
        auto tun = Tunings::Tuning(scale, map);
        int note = 60;
        for (const auto &v : {100.0, 140., 160., 200., 280., 320.})
        {
            REQUIRE(tun.frequencyForMidiNote(note) == Approx(v).margin(0.001));
            note++;
        }
    }
    DYNAMIC_SECTION("Skipper Long")
    {
        auto scale = Tunings::readSCLFile(testFile("kbm-fix-012023/exact4.scl"));
        auto map = Tunings::readKBMFile(testFile("kbm-fix-012023/sixnote-c-100.kbm"));
        auto tun = Tunings::Tuning(scale, map);
        int note = 60;
        for (const auto &v : {100.0, 125.0, 175., 200., 250., 350., 400.})
        {
            REQUIRE(tun.frequencyForMidiNote(note) == Approx(v).margin(0.001));
            note++;
        }
    }
}

TEST_CASE("Retuning API")
{
    SECTION("12TET is retuned zero")
    {
        auto s = Tunings::evenTemperament12NoteScale();
        auto t = Tunings::Tuning(s);
        for (int i = 0; i < 128; ++i)
        {
            REQUIRE(t.retuningFromEqualInSemitonesForMidiNote(i) == Approx(0.f).margin(1e-7));
            REQUIRE(t.retuningFromEqualInCentsForMidiNote(i) == Approx(0.f).margin(1e-7));
        }
    }

    for (int diff = -8; diff < 8; ++diff)
    {
        DYNAMIC_SECTION("12TET with A moved by " << diff << " keys")
        {
            auto s = Tunings::evenTemperament12NoteScale();
            auto k = Tunings::tuneNoteTo(69 + diff, 440.0);
            auto t = Tunings::Tuning(s, k);
            for (int i = 0; i < 128; ++i)
            {
                // This minus sign is because we are tuning note 69+diff so at -1, we tune
                // note 68 to 440. This means when you press note 69 you want to get 466
                // which is the same as tuning note 69 *up* one semitone.  Just a test
                // construction artifact
                REQUIRE(t.retuningFromEqualInSemitonesForMidiNote(i) == Approx(-diff).margin(1e-7));
                REQUIRE(t.retuningFromEqualInCentsForMidiNote(i) ==
                        Approx(-diff * 100.f).margin(1e-7));
            }
        }
    }

    SECTION("24 TET centered at zero")
    {
        auto s = Tunings::evenDivisionOfSpanByM(2, 24);
        auto t = Tunings::Tuning(s);
        for (int i = 60 - 24; i <= 60 + 24; ++i)
        {
            auto dist = 60 - i;
            auto rt = dist * 0.5;
            REQUIRE(t.retuningFromEqualInSemitonesForMidiNote(i) == Approx(rt).margin(1e-7));
            REQUIRE(t.retuningFromEqualInCentsForMidiNote(i) == Approx(rt * 100.0).margin(1e-7));
        }
    }
}

TEST_CASE("Surge 7822 non uniform mapping misses scale center")
{
    SECTION("At Note 57 - no wrapping")
    {
        auto scale = Tunings::readSCLFile(testFile("kbm-wrapping-7822/31edo2.scl"));
        auto map = Tunings::readKBMFile(testFile("kbm-wrapping-7822/31edo2-subset-57.kbm"));
        auto t = Tunings::Tuning(scale, map);
        REQUIRE(t.frequencyForMidiNote(60) == Approx(400.0));
        REQUIRE(t.frequencyForMidiNote(61) == Approx(418.2936581199));
    }
    SECTION("At Note 69 - wrapping")
    {
        auto scale = Tunings::readSCLFile(testFile("kbm-wrapping-7822/31edo2.scl"));
        auto map = Tunings::readKBMFile(testFile("kbm-wrapping-7822/31edo2-subset.kbm"));
        auto t = Tunings::Tuning(scale, map);
        REQUIRE(t.frequencyForMidiNote(60) == Approx(400.0));
        REQUIRE(t.frequencyForMidiNote(61) == Approx(418.2936581199));
    }
}

TEST_CASE("Loading Ableton scales")
{
    SECTION("Good ASCL file")
    {
        auto s = Tunings::readASCLFile(testFile("rast.ascl"));
        REQUIRE(s.scale.count == 12);
        REQUIRE(s.source == "Inside Arabic Music, Chapter 11 (description of Tuning System); Ch "
                            "14-16 (descriptions of Ajnas); Ch 24 (Sayr diagrams)");
        REQUIRE(s.link == "https://www.ableton.com/learn-more/tuning-systems/rast-1");
        REQUIRE(s.rawTexts.size() == 4);
        REQUIRE(s.rawTexts[0] == "! @ABL NOTE_NAMES C D♭ D \"E♭\" E1/2♭ F F♯ G A♭ A B♭ \"B1/2♭\"");
        REQUIRE(s.notationMapping.count == 12);
        REQUIRE(s.notationMapping.names[11] == "C");
        REQUIRE(s.keyboardMapping.count == 12);
        REQUIRE(s.keyboardMapping.keys.size() == 12);
        REQUIRE(s.keyboardMapping.tuningFrequency == Approx(261.6256));
        REQUIRE(s.keyboardMapping.middleNote == 60);
        REQUIRE(s.keyboardMapping.tuningConstantNote == 60);
    }

    SECTION("ASCL compared with Ableton-generated KBM")
    {
        std::string files[3] = {"maqamat", "31-edo", "liwung-tbn"};
        for (std::string file : files)
        {
            auto s = Tunings::readASCLFile(testFile(file + ".ascl"));
            auto k = Tunings::readKBMFile(testFile(file + ".kbm"));
            REQUIRE(s.keyboardMapping.count == k.count);
            REQUIRE(s.keyboardMapping.tuningFrequency == k.tuningFrequency);
            REQUIRE(s.keyboardMapping.middleNote == k.middleNote);
            REQUIRE(s.keyboardMapping.tuningConstantNote == k.tuningConstantNote);
        }
    }

    SECTION("Bad ASCL file")
    {
        REQUIRE_THROWS_AS(Tunings::readASCLFile(testFile("bad/bad-rast.ascl")),
                          Tunings::TuningError);
    }

    SECTION("Tuning read with ASCL")
    {
        auto s = Tunings::readASCLFile(testFile("rast.ascl"));
        Tunings::Tuning t(s);
        REQUIRE(t.frequencyForMidiNote(s.keyboardMapping.tuningConstantNote) ==
                Approx(s.referencePitchFreq));
        REQUIRE(t.scalePositionForMidiNote(s.keyboardMapping.tuningConstantNote) ==
                s.referencePitchIndex);
        REQUIRE(t.midiNoteForNoteName("C", 3) == 60);
        REQUIRE(t.midiNoteForNoteName("C", -100) == 0);
        REQUIRE(t.midiNoteForNoteName("C", 100) == 511);
        REQUIRE(t.midiNoteForNoteName("E1/2♭", 3) == 64);
        REQUIRE(t.midiNoteForNoteName("E1/2♭", 2) == 64 - 12);
        REQUIRE_THROWS_AS(t.midiNoteForNoteName("E1/3♭", 3), Tunings::TuningError);
        REQUIRE(t.noteNameForScalePosition(0) == "C");
        REQUIRE(t.noteNameForScalePosition(4) == "E1/2♭");
        REQUIRE(t.noteNameForScalePosition(4 + t.scale.count) == "E1/2♭");
    }

    SECTION("Tuning read without ASCL")
    {
        auto s = Tunings::readASCLFile(testFile("31edo.scl"));
        Tunings::Tuning t(s);
        REQUIRE_THROWS_AS(t.midiNoteForNoteName("E1/3♭", 3), Tunings::TuningError);
    }
}

int main(int argc, char **argv)
{
    if (getenv("LANG") != nullptr)
    {
        try
        {
            std::locale::global(std::locale(getenv("LANG")));
            std::cout << "Setting LOCALE to '" << getenv("LANG") << "'. ";

            time_t rawtime;
            struct tm *timeinfo;
            char buffer[512];

            time(&rawtime);
            timeinfo = localtime(&rawtime);

            // work around a windows g++ warning error
            const char *fmt = "%A %e %B %Y";
            strftime(buffer, sizeof(buffer), fmt, timeinfo);

            std::cout << "Date in this locale: '" << buffer << "'" << std::endl;
            ;
        }
        catch (std::exception &e)
        {
            std::locale::global(std::locale("C"));
        }
    }
    int result = Catch::Session().run(argc, argv);
    return result;
}
