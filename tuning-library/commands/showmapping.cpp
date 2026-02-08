#include <iostream>
#include <iomanip>
#include "Tunings.h"

using namespace Tunings;

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout
            << "Usage: " << argv[0] << " scl-file/ascl-file [kbm-file]\n\n"
            << "Will show the frequency mapping across the midi keyboard for the scl/kbm combo"
            << std::endl;
        exit(1);
    }

    try
    {
        auto t = Tuning();
        if (strstr(argv[1], ".ascl"))
        {
            auto s = readASCLFile(argv[1]);
            t = Tuning(s);
        }
        else
        {
            auto s = readSCLFile(argv[1]);
            KeyboardMapping k;

            if (argc == 3)
            {
                k = readKBMFile(argv[2]);
            }

            t = Tuning(s, k);
        }

        std::cout << std::setw(4) << "Note," << std::setw(18) << "Freq (Hz)," << std::setw(18)
                  << "ScaledFrq," << std::setw(18) << "logScaled," << std::setw(6) << "Pos,"
                  << " Name" << std::endl;

        for (int i = 0; i < 128; ++i)
        {
            if (t.isMidiNoteMapped(i))
            {
                std::cout << std::setw(4) << i << ", " << std::setw(16) << std::setprecision(10)
                          << std::fixed << t.frequencyForMidiNote(i) << ", " << std::setw(16)
                          << std::setprecision(10) << std::fixed
                          << t.frequencyForMidiNoteScaledByMidi0(i) << ", " << std::setw(16)
                          << std::setprecision(10) << std::fixed
                          << t.logScaledFrequencyForMidiNote(i) << ", " << std::setw(4)
                          << t.scalePositionForMidiNote(i) << ", "
                          << (t.notationMapping.count
                                  ? t.noteNameForScalePosition(t.scalePositionForMidiNote(i))
                                  : "N/A")
                          << std::endl;
            }
            else
            {
                std::cout << std::setw(4) << i << "  [unmapped]" << std::endl;
            }
        }
    }
    catch (const Tunings::TuningError &e)
    {
        std::cout << "Tuning threw an exception: " << e.what() << std::endl;
    }
}
