//
// Created by Lucius Schoenbaum on 3/21/22.
//

#pragma once


#include <Tbasis.h>

#include <kyte/convert.h>

using namespace Kyte;

class TConvert {


public: // API:

    static void Ttohex() {
        for (unsigned char x = 0; x < 128; x = x+8) {
            cout << static_cast<uint32_t>(x) << " = " << Convert::tohex(x) << endl;
        }
    }

};
