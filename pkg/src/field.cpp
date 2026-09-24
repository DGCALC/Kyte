//
// Created by Lucius Schoenbaum on 3/18/22.
//

#include <kyte/field.h>
#include <kyte/convert.h>

// The C++ std::size_t type can be expected to be the architecture address size.
#define base_size (sizeof(std::size_t)*8)
#define base_logsize (DGCALC::ilog2<DGCALC::logsize_t>(base_size))

namespace Kyte {

// public: // artifacts:

bool field<base_logsize>::operator==(const field<base_logsize> & rhs) const {
    if (this == &rhs) return true;
    return (L == rhs.L);
}

bool field<base_logsize>::operator!=(const field<base_logsize> & rhs) const {
    return !(*this == rhs);
}

bool field<base_logsize>::operator<(const field<base_logsize> & rhs) const {
    return (L < rhs.L);
}

bool field<base_logsize>::operator>(const field<base_logsize> & rhs) const {
    return (rhs < *this);
}

bool field<base_logsize>::operator<=(const field<base_logsize> & rhs) const {
    return !(rhs < *this);
}

bool field<base_logsize>::operator>=(const field<base_logsize> & rhs) const {
    return !(*this < rhs);
}

// public: // API:

//    std::size_t u() const {
//        return L & base_umask;
//    }

//    std::size_t l() const {
//        return L & base_lmask;
//    }

field<base_logsize> field<base_logsize>::ones() {
    field<base_logsize> out(-1);
    return out;
}

field<base_logsize> field<base_logsize>::zeros() {
    return {};
}

Byte & field<base_logsize>::operator[](size_t idx) {
    return reinterpret_cast<Bytearray<sizeof(std::size_t)>*>(this)->operator[](sizeof(std::size_t) - 1 - idx);
}

const Byte & field<base_logsize>::operator[](size_t idx) const {
    return reinterpret_cast<const Bytearray<sizeof(std::size_t)>*>(this)->operator[](sizeof(std::size_t) - 1 - idx);
}

std::size_t field<base_logsize>::signbit() const {
    return DGCALC::get_bit(L, sizeof(std::size_t) - 1);
}

void field<base_logsize>::set(const std::size_t idx) {
    DGCALC::set_bit<std::size_t>(L, idx);
}

void field<base_logsize>::clear(const std::size_t idx) {
    DGCALC::clear_bit<std::size_t>(L, idx);
}

void field<base_logsize>::toggle(std::size_t idx) {
    DGCALC::toggle_bit<std::size_t>(L, idx);
}

std::size_t field<base_logsize>::popcount() const {
    return DGCALC::popcount(L);
}

field<base_logsize+1> field<base_logsize>::extend() const {
    return {L};
}

void field<base_logsize>::value(std::size_t val) {
    L = val;
}

std::size_t field<base_logsize>::truewidth_bits() const {
    std::size_t out = base_size;
    while (--out > 0)
        if (DGCALC::get_bit(L, out))
            break;
    return out;
}

field<base_logsize> field<base_logsize>::right_shift(int shamt) const {
    return {L >> shamt};
}

field<base_logsize> field<base_logsize>::left_shift(int shamt) const {
    return {L << shamt};
}

// increment the value by one, with ripple carry
void field<base_logsize>::inc(bool & x, bool & overflow) {
    if (x) ++L;
    overflow = (L != 0);
}

// addition.
// low-level arithmetic routine.
field<base_logsize> field<base_logsize>::add(const field<base_logsize> & rhs, bool & overflow) const {
    std::size_t out = rhs.L + L;
    overflow = (out < L);
    return {out};
}

// add_destructive

field<base_logsize> field<base_logsize>::multiply_no_guarantee(const field<base_logsize> & rhs) const {
    return {L*rhs.L};
}

// so often, stack overflow:
// https://stackoverflow.com/questions/28868367/getting-the-high-part-of-64-bit-integer-multiplication
field<base_logsize+1> operator*(const field<base_logsize> & lhs, const field<base_logsize> & rhs) {
    return {lhs.L*rhs.L, static_cast<std::size_t>(((unsigned __int128)lhs.L * (unsigned __int128)rhs.L) >> base_size)};
}

// public: // artifacts:

field<base_logsize> & field<base_logsize>::operator&=(const field<base_logsize> & rhs) {
    L &= rhs.L;
    return *this;
}

field<base_logsize> & field<base_logsize>::operator|=(const field<base_logsize> & rhs) {
    L |= rhs.L;
    return *this;
}

field<base_logsize> & field<base_logsize>::operator>>=(const int rhs) {
    right_shift(rhs);
    return *this;
}

field<base_logsize> & field<base_logsize>::operator<<=(const int rhs) {
    left_shift(rhs);
    return *this;
}

// operator&
field<base_logsize> operator&(const field<base_logsize> & lhs, const field<base_logsize> & rhs) {
    return {lhs.L & rhs.L};
}

// operator|
field<base_logsize> operator|(const field<base_logsize> & lhs, const field<base_logsize> & rhs) {
    return {lhs.L | rhs.L};
}

// operator>>
field<base_logsize> operator>>(const field<base_logsize> & lhs, const int shamt) {
    return {lhs.L >> shamt};
}

// operator<<
field<base_logsize> operator<<(const field<base_logsize> & lhs, const int shamt) {
    return {lhs.L << shamt};
}

// public: // printing

std::string field<base_logsize>::str_bin() const {
    std::string out = Kyte::Convert::tobin(static_cast<uint64_t>(L));
    return out;
}

std::string field<base_logsize>::str_dec() const {
    return std::to_string(L);
}

std::string field<base_logsize>::str_hex() const {
    return Kyte::Convert::tohex(static_cast<uint64_t>(L));
}

std::string field<base_logsize>::str() const {
    return reinterpret_cast<const Bytearray<sizeof(std::size_t)>*>(this)->str();
}

}


#undef base_size
#undef base_logsize