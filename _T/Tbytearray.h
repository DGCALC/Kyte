//
// Created by Lucius Traylor Schoenbaum on 3/8/22
//

#pragma once

#include <Tbasis.h>

#include <kyte/bytearray.h>

using namespace Kyte;

class TBytearray {

public: // API:

    static void Tdryrun() {
        Bytearray<10> bt;
        cout << "Hello, my byte array is " << bt.str() << endl;
        cout << "in hex: " << bt.str_hex() << endl;
    }

    static void Tbyte_print() {
        Byte x = Byte(".0;0");
        cout << "x = " << static_cast<int>(x.data) << endl;
        cout << "x = " << x.str() << endl;
        cout << "x = " << x.str_di() << endl;
        Byte y = Byte(".0.2");
        cout << "y = " << static_cast<int>(y.data) << endl;
        cout << "y = " << y.str() << endl;
        cout << "y = " << y.str_di() << endl;
        Byte z = Byte("0xa3");
        cout << "z = " << static_cast<int>(z.data) << endl;
        cout << "z = " << z.str_hex() << endl;
        cout << "z = " << z.str() << endl;
        cout << "z = " << z.str_di() << endl;
        Byte u = Byte("45");
        cout << "u = " << static_cast<int>(u.data) << endl;
        cout << "u = " << u.str() << endl;
        cout << "u = " << u.str_di() << endl;
        Byte v = Byte("8");
        cout << "v = " << static_cast<int>(v.data) << endl;
        cout << "v = " << v.str() << endl;
        cout << "v = " << v.str_di() << endl;
    }

    static void Tbytearray_print() {
        Byte b1(";3;3");
        Byte b2(".0.0");
        Byte b3(".0:2");
        Bytearray<4> b;
        for (int i = 0; i < 4; ++i) {
            b[i] = b1;
        }
        cout << b.str_dat() << endl;
        cout << b.str_di() << endl;
        for (int i = 0; i < 4; ++i) {
            b[i] = b2;
        }
        cout << b.str_dat() << endl;
        cout << b.str_di() << endl;
        for (int i = 0; i < 4; ++i) {
            b[i] = b3;
        }
        cout << b.str_dat() << endl;
        cout << b.str_di() << endl;
    }





};
