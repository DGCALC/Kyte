//
// Created by Lucius Schoenbaum on 3/18/22.
//


#include <kyte/byte.h>
#include <kyte/convert.h>
#include <cassert>

namespace Kyte {

Byte::Byte(const char * in) {
    unsigned char x = 0;
    if (in[1] == '\0') goto base10;
    if (in[1] == 'x') {
        x = std::strtol(in, nullptr, 16);
    }
    else if ((in[0] >> 2) == 0x0b || (in[0] >> 1) == 0x1d) {
        for (int i = 0; i < 2; ++i) {
            unsigned char y1, y2;
            switch (in[2 * i]) {
                case ('.'):
                    y1 = 0;
                    break;
                case (','):
                    y1 = 1;
                    break;
                case (':'):
                    y1 = 2;
                    break;
                case (';'):
                    y1 = 3;
                    break;
                default:
                    assert(0 == 1);
            }
            switch (in[2 * i + 1]) {
                case ('0'):
                    y2 = 0;
                    break;
                case ('1'):
                    y2 = 1;
                    break;
                case ('2'):
                    y2 = 2;
                    break;
                case ('3'):
                    y2 = 3;
                    break;
                default:
                    assert(0 == 1);
            }
            x |= ((y1 << 2) | y2) << 4 * (1 - i);
        }
    }
    else {
base10:
        x = std::strtol(in, nullptr, 10);
    }
    data = x;
}

Byte::Byte(std::string in): Byte(in.c_str()) {}

std::string Byte::str_di() const {
    bool neg = (data >> 7);
    std::string out = (neg)? "-": "";
        if (varies()) {
            unsigned char x = (data >> 4);
            unsigned char y = (x & 0xc);
            bool leading = true;
            if (y) {
                if (y == 0x8) {out += ':'; leading = false;}
                else if (y == 0xa) {out += ','; leading = false;}
                x = (x & 0x3);
                if (x == 0 && (neg || !leading)) {out += '0'; leading = false;}
                else if (x == 1) {out += '1'; leading = false;}
                else if (x == 2) {out += '2'; leading = false;}
                else if (x == 3 && (!neg || !leading)) {out += '3'; leading = false;}
            }
            y = (data & 0xc);
            if (y == 0x0 && (neg || !leading)) out += '.';
            else if (y == 0x8) out += ':';
            else if (y == 0xa) out += ',';
            else if (y == 0xc && (!neg || !leading)) out += ';';
            x = (data & 0x3);
            if (x == 0) out += '0';
            else if (x == 1) out += '1';
            else if (x == 2) out += '2';
            else out += '3';
        }
        else {
            out += (neg)? "1": "0";
        }
    return out;
}


std::string Byte::str_dat() const {
    std::string out;
    // unroll
    for (int i = 1; i >= 0; --i) {
        unsigned char x = (data >> 4*i);
        unsigned char y = (x & 0xc);
        if (y == 0x0) out += '.';
        else if (y == 0x8) out += ':';
        else if (y == 0xa) out += ',';
        else out += ';';
        x = (x & 0x3);
        if (x == 0) out += '0';
        else if (x == 1) out += '1';
        else if (x == 2) out += '2';
        else out += '3';
    }
    return out;
}

std::string Byte::str() const {
    return str_dat();
}

std::string Byte::str_hex() const {
    return Convert::tohex(data);
}

std::string Byte::str_bin() const {
    return Convert::tobin(data);
}

}