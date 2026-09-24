//
// Created by Lucius Schoenbaum on 3/13/22.
//

#pragma once

#include <string>

namespace Kyte::Convert {

    // convert double to hex, as binary data
    std::string tohex(double data);

    // convert unsigned short to a single hex character
    std::string tohex(unsigned char x);

    // convert uint64_t to hex, as binary data, with spacing elements
    std::string tohex(uint64_t x, bool spacer = true);

    // 64-bit system: uint64_t and std::size_t *should* match.
    // otherwise, this should upcast/downcast to a 8-byte width uint.
    std::string tohex(std::size_t x, bool spacer = true);

    // the missing method... double to formatted binary representation.
    std::string tobin(double data);

    // uint64 to formatted binary representation.
    std::string tobin(uint64_t data);

    // unsigned char to string of 1's and 0's
    std::string tobin(unsigned char ain);

}

