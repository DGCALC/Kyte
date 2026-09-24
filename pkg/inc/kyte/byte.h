//
// Created by Lucius Schoenbaum on 3/21/22.
//

#pragma once

#include <string>

namespace Kyte {
class Byte {
public: // fields:

    unsigned char data;

public: // ctor:

    Byte(unsigned char in = 0):
        data(in)
    {}

    template <typename itype>
    Byte(itype in):
        data(static_cast<unsigned char>(in))
    {}

    explicit Byte(const char * in);

    explicit Byte(std::string in);

public: // API:

    [[nodiscard]] bool neg() const {
        return (data > 0x80);
    }

    [[nodiscard]] bool varies() const {
        return (data != 0x0 && data != 0xFF);
    }

public: // printing:

    [[nodiscard]] std::string str_di() const;

    [[nodiscard]] std::string str_dat() const;

    [[nodiscard]] std::string str() const;

    [[nodiscard]] std::string str_hex() const;

    [[nodiscard]] std::string str_bin() const;

};
}
