//
// Created by Lucius Traylor Schoenbaum on 2/14/21.
//

/*
 * Bytearray
 *
 *
 *
 *
 */

#pragma once

#include <DGCALC.h>
#include <kyte/byte.h>
#include <kyte/convert.h>
#include <string>

namespace Kyte {
template <std::size_t size_>
class Bytearray {
public: // static fields:

    static const std::size_t size = size_;

private: // fields:

    Byte data[size];

public: // ctor:

    Bytearray()
    {
        memset(data, 0, sizeof(unsigned char)*size);
    }

public: // copy:

public: // dtor:

    ~Bytearray() = default;

private: // methods:



public: // API:

    // Bracket notation
    Byte & operator[](size_t idx) {
        return data[idx];
    }

    const Byte & operator[](size_t idx) const {
        return data[idx];
    }


public: // printing:

    // str_di: hide the leading ones or zeros.
    // if there are leading ones, represent this with a symbol "-".
    // NOTE: careful, this is not quite the
    // same as decimal "negative" printing.
    std::string str_di() const {
        std::string out;
        bool first = true;
        for (int i = 0; i < size - 1; ++i) {
            if (data[i].varies()) {
                if (first) {
                    first = false;
                    out += data[i].str_di();
                }
                else {
                    if (i % 4 == 0) out += "_";
                    out += data[i].str_dat();
                }
            }
        }
        out += (first)? data[size-1].str_di(): data[size-1].str_dat();
        return out;
    }

    std::string str_dat() const {
        std::string out;
        for (int i = 0; i < size; ++i) {
            if (i != 0 && i % 4 == 0) out += "_";
            out += data[i].str();
        }
        return out;
    }

    std::string str() const {
        return str_dat();
    }

    std::string str_hex() const {
        std::string out;
        for (int i = 0; i < size; ++i) {
            out += data[i].str_hex();
            if (i % 4 == 0) out += "_";
        }
        return out;
    }

};
}

