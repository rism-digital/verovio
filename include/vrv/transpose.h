//
// Name: transpose.h
// Author: Zach DeCook
// Created: December 5 2019
//

#ifndef __VRV_TRANSPOSE_H__
#define __VRV_TRANSPOSE_H__

#include <iostream>

namespace vrv {

class TPitch;

class TPitch {
public:
    int pname; // diatonic pitch class name: C = 0, D = 1, ... B = 6.
    int accid; // chromatic alteration: 0 = natural, 1 = sharp, -2 = flat, +2 = double sharp
    int oct; // octave number: 4 = middle-C octave

    TPitch(){};
    TPitch(int aPname, int anAccid, int anOct);
    TPitch(const TPitch &pitch);
    TPitch &operator=(const TPitch &pitch);
    bool isValid(int maxAccid);
    void setPitch(int aPname, int anAccid, int anOct);
};

std::ostream &operator<<(std::ostream &out, const TPitch &pitch);

} // namespace vrv

#endif
