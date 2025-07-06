/*
** Make sure we don't have duplicated symbols by compiling a pair of objects
** and linking them
*/

#include "Tunings.h"

double symbolcheck1()
{
    auto k = Tunings::tuneNoteTo(60, 100);
    return k.tuningFrequency;
}
