/*
** Make sure we don't have duplicated symbols by compiling a pair of objects
** and linking them
*/

#include <iostream>
#include <iomanip>
#include "Tunings.h"

double symbolcheck2()
{
    auto k = Tunings::tuneNoteTo(60, 200);
    return k.tuningFrequency;
}

int main(int argc, char **argv)
{
    extern double symbolcheck1();
    std::cout << "100 and 200 are " << symbolcheck1() << " and " << symbolcheck2() << std::endl;
}
