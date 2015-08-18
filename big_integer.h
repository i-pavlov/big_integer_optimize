#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include "so_vector.h"
#include <string>

struct big_integer {
    big_integer();
    big_integer(big_integer const& other);
    big_integer(unsigned int a);
    big_integer(int a);
    big_integer(long long a);
    explicit big_integer(std::string const& str);
    ~big_integer();
    
    big_integer& operator = (big_integer const& other);
    
    big_integer& operator += (big_integer const& rhs);
    big_integer& operator -= (big_integer const& rhs);
    big_integer& operator *= (big_integer const& rhs);
    big_integer& operator /= (int const& rhs);
    big_integer& operator /= (big_integer const& rhs);
    big_integer& operator %= (big_integer const& rhs);
    
    big_integer& operator &= (big_integer const& rhs);
    big_integer& operator |= (big_integer const& rhs);
    big_integer& operator ^= (big_integer const& rhs);
    
    big_integer& operator <<= (int rhs);
    big_integer& operator >>= (int rhs);
    
    big_integer operator + () const;
    big_integer operator - () const;
    big_integer operator ~ () const;
    
    big_integer& operator ++ ();
    big_integer operator ++ (int);
    
    big_integer& operator -- ();
    big_integer operator -- (int);
    
    friend bool operator == (big_integer const& a, big_integer const& b);
    friend bool operator != (big_integer const& a, big_integer const& b);
    friend bool operator < (big_integer const& a, big_integer const& b);
    friend bool operator > (big_integer const& a, big_integer const& b);
    friend bool operator <= (big_integer const& a, big_integer const& b);
    friend bool operator >= (big_integer const& a, big_integer const& b);
    
    friend std::string to_string(big_integer const& a);
    
    char sign;
    so_vector elements;
    void __delete_zeroes();
    void __to_additional_code();
    void __binary_operation(uint32_t (*func)(uint32_t, uint32_t),
                            big_integer const& rhs,
                            char need_sign);
    static const uint32_t base = 4294967295U;
    static const uint32_t blen = 32;
};

big_integer operator + (big_integer a, big_integer const& b);
big_integer operator - (big_integer a, big_integer const& b);

big_integer operator * (big_integer a, big_integer const& b);
big_integer operator / (big_integer a, int const& b);
big_integer operator / (big_integer a, big_integer const& b);
big_integer operator % (big_integer a, big_integer const& b);

big_integer operator & (big_integer a, big_integer const& b);
big_integer operator | (big_integer a, big_integer const& b);
big_integer operator ^ (big_integer a, big_integer const& b);

big_integer operator << (big_integer a, int b);
big_integer operator >> (big_integer a, int b);

bool operator == (big_integer const& a, big_integer const& b);
bool operator != (big_integer const& a, big_integer const& b);
bool operator < (big_integer const& a, big_integer const& b);
bool operator > (big_integer const& a, big_integer const& b);
bool operator <= (big_integer const& a, big_integer const& b);
bool operator >= (big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& a);
std::ostream& operator << (std::ostream& s, big_integer const& a);

#endif // BIG_INTEGER_H
