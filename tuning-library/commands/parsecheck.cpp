#include <cstring>
#include <iomanip>
#include "Tunings.h"
#include "TuningsImpl.h"

int main(int argc, char **argv)
{
    for (int i=1; i<argc; ++i)
    {
        std::cout << std::setw(50) << argv[i];
        try {
            if (strstr(argv[i], ".scl")) {
                Tunings::readSCLFile(argv[i]);
                std::cout << " PASSED";
            }
            else if (strstr(argv[i], ".kbm")) {
                Tunings::readKBMFile(argv[i]);
                std::cout << " PASSED";
            }
            else
            {
                std::cout << " SKIPPED";
            }

        } catch(Tunings::TuningError &t)
        {
            std::cout << " FAILED : " << t.what();
        }
        std::cout << std::endl;
    }
}
