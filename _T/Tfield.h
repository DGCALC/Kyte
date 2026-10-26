//
// Created by Lucius Schoenbaum on 10/28/21.
//

#pragma once

#include <Tbasis.h>

#include <kyte/field.h>
#include <kyte/convert.h>


class Tfield {

    template <DGCALC::logsize_t logsize>
    static void try_multiply_no_guarantee(const field<logsize> & a, const field<logsize> & b) {
        cout << "a = " << a.str() << "\n";
        cout << "b = " << b.str() << "\n";
        cout << "a*b = " << a.multiply_no_guarantee(b).str() << "\n\n";
    }

    template <DGCALC::logsize_t logsize>
    static void try_multiply(const field<logsize> & a, const field<logsize> & b) {
        cout << "a = " << a.str() << "\n";
        cout << "b = " << b.str() << "\n";
        cout << "a*b = " << (a*b).str() << "\n\n";
    }

    template <DGCALC::logsize_t logsize>
    static void try_shift(const field<logsize> & a, int b, bool bin) {
        if (bin) {
            std::string astr = a.str_bin();
            cout << astr << " << " << b << " = " << (a << b).str_bin() << endl;
            cout << astr << " >> " << b << " = " << (a >> b).str_bin() << "\n\n";
        }
        else {
            std::string astr = a.str();
            cout << astr << " << " << b << " = " << (a << b).str() << endl;
            cout << astr << " >> " << b << " = " << (a >> b).str() << "\n\n";
        }
    }


public: // API:

    static void Tmultiply_no_guarantee() {
        try_multiply_no_guarantee<6>({2}, {3});
        cout << "7:" << endl;
        try_multiply_no_guarantee<7>({2, 0}, {0, 3});
        try_multiply_no_guarantee<7>({2, 0}, {3, 0});
        cout << "8:" << endl;
        try_multiply_no_guarantee<8>({0, 2}, {3, 1234});
        try_multiply_no_guarantee<8>({2, 0}, {3, 0});
        cout << "9:" << endl;
        try_multiply_no_guarantee<9>({0, 2}, {0, 3});
        try_multiply_no_guarantee<9>({2, 0}, {3, 0});
        cout << "bye" << endl;
    }

    static void Tmultiplication() {
        cout << "base type:\n";
        try_multiply(field<6>(2), field<6>(3));
        cout << "7:\n";
        try_multiply(field<7>(2), field<7>(3));
        cout << "8:\n";
        try_multiply(field<8>(2), field<8>(3));
        cout << "8:\n";
        try_multiply(field<8>(123456, 7890), field<8>(234, 8554));
    }

    static void Tshift() {
        cout << "base type:\n";
        bool bin = true;
        try_shift(field<6>(2), 0, bin);
        try_shift(field<6>(2), 2, bin);
        try_shift(field<6>(2), 32, bin);
        cout << "7:\n";
        try_shift(field<7>(2), 0, bin);
        try_shift(field<7>(2), 2, bin);
        try_shift(field<7>(2), 68, bin);
        cout << "8:\n";
        try_shift(field<8>(2), 0, bin);
        try_shift(field<8>(2), 2, bin);
        try_shift(field<8>(2), 120, bin);
        cout << "8:\n";
        try_shift(field<8>(123456, 7890), 3, bin);
    }

    static void Tset_clear_print_sanity_check() {
        cout << field<6>::size << endl;
        cout << field<7>::size << endl;
        cout << field<8>::size << endl;
        cout << Kyte::Convert::tohex(static_cast<uint64_t>((uint64_t)1 << 50)) << endl;
        cout << "if all that looks ok, then:" << endl;
        uint64_t x = -1;
        cout << Kyte::Convert::tohex(x) << endl;
        DGCALC::clear_bit<uint64_t>(x, 50);
        cout << Kyte::Convert::tohex(x) << endl;
    }

    static void Tset_clear() {
        const int s = 6;
        field<s> a = field<s>::ones();
        field<s> b = field<s>::zeros();
        cout << "before:" << endl;
        cout << a.str() << endl;
        cout << b.str() << endl;
        for (int i = 0; i < DGCALC::ipow2<std::size_t>(s); i = i + 3) {
            a.clear(i);
            b.set(i);
        }
        cout << "after:" << endl;
        cout << a.str() << endl;
        cout << b.str() << endl;
        cout << a.str_bin() << endl;
        cout << b.str_bin() << endl;
    }

    static void Tsignbit() {
        const int s6 = 6;
        const int s7 = 7;
        const int s8 = 8;
        const int s9 = 9;
        field<s6> a = field<s6>::ones();
        field<s7> b = field<s7>::zeros();
        field<s8> c = field<s8>::ones();
        field<s9> d = field<s9>::zeros();
        cout << a.str() << " has sign bit " << a.signbit() << endl;
        cout << b.str() << " has sign bit " << b.signbit() << endl;
        cout << c.str() << " has sign bit " << c.signbit() << endl;
        cout << d.str() << " has sign bit " << d.signbit() << endl;
    }

    static void Tbracket_notation() {
#define str str
//#define str str_hex
//#define str str_bin
        // to get a sense of accomplishment, try turning up this dial a tad.
        const int s = 6;
        field<s> a = field<s>::ones();
        field<s> b = field<s>::zeros();
        for (int i = 0; i < DGCALC::ipow2<std::size_t>(s); i = i + 5) {
            a.clear(i);
            b.set(i);
        }
        cout << "a = " << a.str() << endl;
        cout << "b = " << b.str() << endl;
        cout << "a: ";
        for (int i = sizeof(field<s>)-1; i >= 0; --i) {
            cout << "[" << i << "]" << a[i].str();
        }
        cout << "\nb: ";
        for (int i = sizeof(field<s>)-1; i >= 0; --i) {
            cout << "[" << i << "]" << b[i].str();
        }
        cout << endl;
        Byte x = Byte(".1:3");
        cout << "x = " << x.str() << endl;
        a[DGCALC::ipow2<int>(s-1)+2] = x;
        cout << "a = " << a.str() << endl;
#undef str
    }

};
