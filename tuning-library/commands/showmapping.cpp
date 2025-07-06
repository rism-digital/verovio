#include <iostream>
#include <iomanip>
#include "Tunings.h"

using namespace Tunings;

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout
            << "Usage: " << argv[0] << " scl-file [kbm-file]\n\n"
            << "Will show the frequency mapping across the midi keyboard for the scl/kbm combo"
            << std::endl;
        exit(1);
    }

    try
    {
        auto s = readSCLFile(argv[1]);
        KeyboardMapping k;

        if (argc == 3)
        {
            k = readKBMFile(argv[2]);
        }

        auto t = Tuning(s, k);
        std::cout << "Note ,"
                  << " Freq (Hz) , "
                  << "  ScaledFrq , "
                  << " logScaled " << std::endl;

        for (int i = 0; i < 128; ++i)
        {
            if (t.isMidiNoteMapped(i))
            {
                std::cout << std::setw(4) << i << ", " << std::setw(10) << std::setprecision(10)
                          << std::fixed << t.frequencyForMidiNote(i) << ", " << std::setw(10)
                          << std::setprecision(10) << std::fixed
                          << t.frequencyForMidiNoteScaledByMidi0(i) << ", " << std::setw(10)
                          << std::setprecision(10) << std::fixed
                          << t.logScaledFrequencyForMidiNote(i) << ", "
                    << t.scalePositionForMidiNote(i) << std::endl;
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
