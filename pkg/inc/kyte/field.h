//
// Created by Lucius Schoenbaum on 3/13/22
//

/*
 * field.h
 *
 *
 *
 */

#pragma once

#include <DGCALC.h>
#include <kyte/byte.h>
#include <kyte/bytearray.h>

namespace Kyte {
template <DGCALC::logsize_t logsize /* 6, 7, 8 == 64, 128, 256 */>
class field {
public: // static fields:

    static const std::size_t size = DGCALC::ipow2<std::size_t>(logsize);

private: // fields:

    field<logsize - 1> U;
    field<logsize - 1> L;

public: // ctor:

    field(const field<logsize - 1 > & lower, const field<logsize - 1> & upper = field<logsize - 1>(0)):
        U(upper),
        L(lower)
    {};

    // The base type is >= 64 bits
    field(std::size_t lval = 0, std::size_t uval = 0) {
        U.value(uval);
        L.value(lval);
    }

public: // copy:

    field(const field & fin) {
        this->U = fin.U;
        this->L = fin.L;
    }

    field & operator=(const field & rhs) {
        if (this == &rhs) return *this;
        this->U = rhs.U;
        this->L = rhs.L;
        return *this;
    }

public: // dtor:

    ~field() = default;

public: // artifacts:

    bool operator==(const field & rhs) const {
        if (this == &rhs) return true;
        return (U == rhs.U && L == rhs.L);
    }

    bool operator!=(const field & rhs) const {
        return !(*this == rhs);
    }

    bool operator<(const field & rhs) const {
        if (U < rhs.U) return true;
        return (L < rhs.L);
    }

    bool operator>(const field & rhs) const {
        return (rhs < *this);
    }

    bool operator<=(const field & rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const field & rhs) const {
        return !(*this < rhs);
    }

public: // API:

    field<logsize - 1> u() const {
        return U;
    }

    field<logsize - 1> l() const {
        return L;
    }

    static field ones() {
        return {field<logsize-1>::ones(), field<logsize-1>::ones()};
    }

    static field zeros() {
        return field();
    }

    // Bracket notation
    Byte & operator[](size_t idx) {
        return reinterpret_cast<Bytearray<size/8>*>(this)->operator[](size/8 - 1 - idx);
    }

    const Byte & operator[](size_t idx) const {
        return reinterpret_cast<const Bytearray<size/8>*>(this)->operator[](size/8 - 1 - idx);
    }

    std::size_t signbit() const {
        return U.signbit();
    }

    void set(const std::size_t idx) {
        if (idx < size/2)
            L.set(idx);
        else
            U.set(idx - size/2);
    }

    void clear(const std::size_t idx) {
        if (idx < size/2)
            L.clear(idx);
        else
            U.clear(idx - size/2);
    }

    void toggle(const std::size_t idx) {
        if (idx < size/2)
            L.toggle(idx);
        else
            U.toggle(idx - size/2);
    }

    // extend the field, without modification, to double-size
    field<logsize + 1> extend() const {
        return field<logsize + 1>(*this, field());
    }

    // recursively set the value equal to an explicit unsigned integer
    void value(std::size_t val) {
        U.value(0);
        L.value(val);
    }

    // return the true width of the field, in bits,
    // i.e., size minus # of leading zeros (thus, zero if field is zero, size if MSB != 0).
    std::size_t truewidth_bits() const {
        std::size_t out;
        std::size_t Uw = U.truewidth_bits();
        if (Uw > 0)
            out = Uw + size / 2;
        else out = L.truewidth_bits();
        return out;
    }

    std::size_t popcount() const {
        std::size_t out = U.popcount();
        out += L.popcount();
        return out;
    }

    // shift the field right
    field right_shift(int shamt) const {
        field out;
        if (shamt == 0) {
            out = *this;
        }
        else if (shamt < size) {
            if (shamt >= size / 2) {
                out.U = field<logsize - 1>();
                out.L = U.right_shift(shamt - size / 2);
            }
            else {
                //\\ 0 < shamt < size/2
                out.L = L.right_shift(shamt);
                out.L |= U.left_shift(size / 2 - shamt);
                out.U = U.right_shift(shamt);
            }
        }
        return out;
    }

    // shift the field left
    field left_shift(int shamt) const {
        field out;
        if (shamt == 0) {
            out = *this;
        }
        else if (shamt < size) {
            if (shamt >= size / 2) {
                out.L = field<logsize - 1>();
                out.U = L.left_shift(shamt - size / 2);
            }
            else {
                //\\ 0 < shamt < size/2
                out.U = U.left_shift(shamt);
                out.U |= L.right_shift(size / 2 - shamt);
                out.L = L.left_shift(shamt);
            }
        }
        return out;
    }

    // increment the value by one, with ripple carry
    void inc(bool & x, bool & overflow) {
        L.inc(x, overflow);
        U.inc(overflow, overflow);
    }

    // addition.
    // low-level arithmetic routine.
    field add(const field & ain, bool & overflow) const {
        bool Loverflow;
        field<logsize-1> Lsum = L.add(ain.L, Loverflow);
        field<logsize-1> Usum = U.add(ain.U, overflow);
        // if it already overflows, it will still overflow.
        Usum.inc(Loverflow, overflow);
        return field(Lsum, Usum);
    }

    // addition of half width into the object, destructively
    void add_destructive(const field<logsize - 1> & rhs, bool & overflow) {
        bool Loverflow;
        L = L.add(rhs, Loverflow);
        U.inc(Loverflow, overflow);
    }

    // multiplication, as unsigned integers, keeping whatever can be preserved,
    // with information loss in general.
    // low-level arithmetic routine.
    field multiply_no_guarantee(const field & rhs) const {
        field<logsize + 1> tmp = (*this)*rhs;
        return tmp.l();
    }

    // guaranteed multiplication: spill into double-size width.
    // All bits preserved and returned intact in a double-sized field.
    friend field<logsize + 1> operator*(const field & lhs, const field & rhs) {
        field<logsize + 1> out;
        field<logsize + 1> tmp1, tmp2, tmp3;
        bool overflow;
        // Lower
        out = lhs.L * rhs.L;
        // Crossed
        tmp1 = (lhs.L*rhs.U).right_shift(field<logsize>::size / 4);  /* divide: sic */
        tmp2 = (lhs.L*rhs.L).right_shift(field<logsize>::size / 4);
        // Upper
        tmp3 = (lhs.U*rhs.U).right_shift(field<logsize>::size/2);
        // todo check overflow
        out = out.add(tmp1, overflow);
        out = out.add(tmp2, overflow);
        out = out.add(tmp3, overflow);
        return out;
    }

public: // artifacts:

    field & operator&=(const field & rhs) {
        U &= rhs.U;
        L &= rhs.L;
        return *this;
    }

    field & operator|=(const field & rhs) {
        U |= rhs.U;
        L |= rhs.L;
        return *this;
    }

    field & operator>>=(const int rhs) {
        right_shift(rhs);
        return *this;
    }

    field & operator<<=(const int rhs) {
        left_shift(rhs);
        return *this;
    }

    // operator&
    friend field operator&(const field & lhs, const field & rhs) {
        return {(lhs.L & rhs.L), (lhs.U & rhs.U)};
    }

    // operator|
    friend field operator|(const field & lhs, const field & rhs) {
        return {(lhs.L | rhs.L), (lhs.U | rhs.U)};
    }

    // operator>>
    friend field operator>>(const field & lhs, const int shamt) {
        return lhs.right_shift(shamt);
    }

    // operator<<
    friend field operator<<(const field & lhs, const int shamt) {
        return lhs.left_shift(shamt);
    }

public: // printing

    std::string str_bin() const {
        std::string out = U.str_bin() + " " + L.str_bin();
        return out;
    }

    std::string str_dec() const {
        std::string out = U.str_dec() + "_" + L.str_dec();
        return out;
    }

    std::string str_hex() const {
        std::string out = U.str_hex() + "_" + L.str_hex();
        return out;
    }

    std::string str() const {
        return reinterpret_cast<const Bytearray<size/8>*>(this)->str();
    }

};







// The C++ std::size_t type can be expected to be the architecture address size.
#define base_size (sizeof(std::size_t)*8)
#define base_logsize (DGCALC::ilog2<DGCALC::logsize_t>(base_size))

template<>
class field<base_logsize> {
public: // static fields:

    static const std::size_t size = base_size;

private: // fields:

    std::size_t L;

public: // ctor:

    field(std::size_t val = 0): L(val) {}

public: // copy:

    field(const field & fin) {
        this->L = fin.L;
    }

    field & operator=(const field & rhs) {
        if (this == &rhs) return *this;
        this->L = rhs.L;
        return *this;
    }

public: // dtor:

    ~field() = default;

public: // artifacts:

    bool operator==(const field & rhs) const;

    bool operator!=(const field & rhs) const;

    bool operator<(const field & rhs) const;

    bool operator>(const field & rhs) const;

    bool operator<=(const field & rhs) const;

    bool operator>=(const field & rhs) const;

public: // API:

//    std::size_t u() const;

//    std::size_t l() const;

    static field ones();

    static field zeros();

    // Bracket notation
    Byte & operator[](size_t idx);

    const Byte & operator[](size_t idx) const;

    std::size_t signbit() const;

    void set(std::size_t idx);

    void clear(std::size_t idx);

    void toggle(std::size_t idx);


    field<base_logsize+1> extend() const;

    void value(std::size_t val);

    std::size_t popcount() const;

    // return the true width of the field, in bits,
    // i.e., size minus # of leading zeros (thus, zero if field is zero, size if MSB != 0).
    std::size_t truewidth_bits() const;

    field right_shift(int shamt) const;

    field left_shift(int shamt) const;

    // increment the value by one, with ripple carry
    void inc(bool & x, bool & overflow);

    // addition.
    // low-level arithmetic routine.
    field add(const field & rhs, bool & overflow) const;

    // add_destructive ...I might have to use extend for this instead?

    // multiplication, keeping whatever can be preserved (i.e., the usual way.)
    field multiply_no_guarantee(const field & rhs) const;

    // All bits preserved and returned intact in a double-sized field.
    friend field<base_logsize+1> operator*(const field & lhs, const field & rhs);

public: // artifacts:

    field & operator&=(const field & rhs);

    field & operator|=(const field & rhs);

    field & operator>>=(const int rhs);

    field & operator<<=(const int rhs);

    // operator&
    friend field operator&(const field & lhs, const field & rhs);

    // operator|
    friend field operator|(const field & lhs, const field & rhs);

    // operator>>
    friend field operator>>(const field & lhs, int shamt);

    // operator<<
    friend field operator<<(const field & lhs, int shamt);

public: // printing

    std::string str_bin() const;

    std::string str_dec() const;

    std::string str_hex() const;

    std::string str() const;

};
}

#undef base_size
#undef base_logsize
