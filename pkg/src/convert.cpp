//
// Created by Lucius Schoenbaum on 3/18/22.
//

#include <kyte/convert.h>
#include <sstream>

namespace Kyte::Convert {

std::string tohex(double data) {
    uint64_t i;
    // 8 bytes = 64 bits
    std::memcpy(&i, &data, 8);
    char buf[17];
    snprintf(buf, sizeof(buf), "%016llx", i);
    buf[16] = '\0';
    return {buf};
}

// This uses std::stringstream and std::hex.
// It is slightly more complicated only in order to add
// an underscore each 16 bits for ease of readability.
std::string tohex(uint64_t x, bool spacer) {
    if (x == 0) return "0";
    else {
        uint64_t tmp;
        std::stringstream stream;
        std::string out;
        bool interior = false;
        int i = 3;
        std::string sp[4] = {"", ".", "..", "..."};
        do {
            tmp = (x & 0xffff000000000000) >> 48;
            x = x << 16;
            if (interior || tmp != 0) {
                interior = true;
                stream << std::hex << tmp;
                if (spacer)
                    stream << sp[i];
            }
            if (x == 0) break;
        } while (--i != -1);
        out = stream.str();
        return out;
    }
}

std::string tohex(std::size_t x, bool spacer) {
    return tohex(static_cast<uint64_t>(x));
}

std::string tohex(unsigned char x) {
    char buf[3];
    snprintf(buf, sizeof(buf), "%02x", x);
    buf[2] = '\0';
    return {buf};
}


std::string tobin(double data) {
    std::string out;
    uint64_t i;
    // 8 bytes = 64 bits
    std::memcpy(&i,&data,8);
    char buf[17];
    snprintf(buf, sizeof(buf),"%016llX", i);
    buf[16] = '\0';
    char ch;
    int count = 0;
    while (true) {
        ch = buf[count];
        switch (ch) {
            case '0':
                if (count == 0) out += "0_000";
                else out += "0000";
                break;
            case '1':
                if (count == 0) out += "0_001";
                else out += "0001";
                break;
            case '2':
                if (count == 0) out += "0_010";
                else out += "0010";
                break;
            case '3':
                if (count == 0) out += "0_011";
                else out += "0011";
                break;
            case '4':
                if (count == 0) out += "0_100";
                else out += "0100";
                break;
            case '5':
                if (count == 0) out += "0_101";
                else out += "0101";
                break;
            case '6':
                if (count == 0) out += "0_110";
                else out += "0110";
                break;
            case '7':
                if (count == 0) out += "0_111";
                else out += "0111";
                break;
            case '8':
                if (count == 0) out += "1_000";
                else out += "1000";
                break;
            case '9':
                if (count == 0) out += "1_001";
                else out += "1001";
                break;
            case 'A':
            case 'a':
                if (count == 0) out += "1_010";
                else out += "1010";
                break;
            case 'B':
            case 'b':
                if (count == 0) out += "1_011";
                else out += "1011";
                break;
            case 'C':
            case 'c':
                if (count == 0) out += "1_100";
                else out += "1100";
                break;
            case 'D':
            case 'd':
                if (count == 0) out += "1_101";
                else out += "1101";
                break;
            case 'E':
            case 'e':
                if (count == 0) out += "1_110";
                else out += "1110";
                break;
            case 'F':
            case 'f':
                if (count == 0) out += "1_111";
                else out += "1111";
                break;
            default:
                out += "X"; break;
        }
        if (count == 15) break;
        count++;
        if (count != 3) out += ".";
        if (count == 3) out += "_";
    }
    return out;
}

std::string tobin(uint64_t data) {
    std::string out;
    uint64_t i;
    // 8 bytes = 64 bits
    std::memcpy(&i,&data,8);
    char buf[17];
    snprintf(buf, sizeof(buf),"%016llX", i);
    buf[16] = '\0';
    char ch;
    int count = 0;
    while (true) {
        ch = buf[count];
        switch (ch) {
            case '0':
                out += "0000";
                break;
            case '1':
                out += "0001";
                break;
            case '2':
                out += "0010";
                break;
            case '3':
                out += "0011";
                break;
            case '4':
                out += "0100";
                break;
            case '5':
                out += "0101";
                break;
            case '6':
                out += "0110";
                break;
            case '7':
                out += "0111";
                break;
            case '8':
                out += "1000";
                break;
            case '9':
                out += "1001";
                break;
            case 'A':
            case 'a':
                out += "1010";
                break;
            case 'B':
            case 'b':
                out += "1011";
                break;
            case 'C':
            case 'c':
                out += "1100";
                break;
            case 'D':
            case 'd':
                out += "1101";
                break;
            case 'E':
            case 'e':
                out += "1110";
                break;
            case 'F':
            case 'f':
                out += "1111";
                break;
            default:
                out += "X"; break;
        }
        if (count == 15) break;
        count++;
        out += "_";
    }
    return out;
}


std::string tobin(unsigned char ain) {
    std::string out;
    for (int i = 7; i >= 0; --i) {
        out += std::to_string((ain >> i) & 0x1);
    }
    return out;
}

}