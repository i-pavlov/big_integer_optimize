#include "big_integer.h"
#include "so_vector.h"
#include <iostream>
#include <algorithm>

/* Private functions */

const big_integer ZERO = 0;

void big_integer::__delete_zeroes() {
    while (elements.size() > 1 && !elements.back())
        elements.pop_back();
    if (!elements.back())
        sign = 0;
}

void big_integer::__to_additional_code() {
    for (size_t i = 0; i < elements.size(); ++i)
        elements[i] = ~elements[i];
    --(*this);
}

void big_integer::__binary_operation(uint32_t (*func)(uint32_t, uint32_t), big_integer const &rhs, char need_sign) {
    if (!sign || !rhs.sign)
        *this = ZERO;
    big_integer result = big_integer(rhs);
    if (result.elements.size() > elements.size())
        elements.resize(result.elements.size());
    if (elements.size() > result.elements.size())
        result.elements.resize(elements.size());
    
    
    if (sign == -1)
        __to_additional_code();
    if (result.sign == -1)
        result.__to_additional_code();
    
    for (size_t i = 0; i < elements.size(); ++i)
        elements[i] = (*func)(elements[i], result.elements[i]);
    sign = need_sign;
    if (need_sign == -1)
        __to_additional_code();
    
    __delete_zeroes();
}

big_integer::big_integer() {
    *this = ZERO;
}

big_integer::big_integer(big_integer const &other) {
    sign = other.sign;
    elements = other.elements;
//    for(size_t i = 0; i < other.elements.size(); ++i)
//        elements.push_back(other.elements[i]);
}

big_integer::big_integer(unsigned int a) {
    elements.clear();
    sign = (char) (a ? 1 : 0);
    elements.push_back(a);
}

big_integer::big_integer(int a) {
    elements.clear();
    sign = (char) ((char) (a == 0) ? 0 : (a > 0) ? 1 : -1);
    elements.push_back((unsigned int const &) abs(a));
}

big_integer::big_integer(long long a) {
    elements.clear();
    if (!a) {
        sign = 0;
        elements.push_back(0);
        return;
    }
    sign = (char) (a > 0 ? 1 : 0);
    if (a < 0)
        a = -a;
    elements.push_back((unsigned int const &) (a & (1LL * base)));
    elements.push_back((unsigned int const &) (a >> blen));
    __delete_zeroes();
}

big_integer::big_integer(std::string const &str) {
    *this = ZERO;
    char result_sign = 1;
    if (str.size() > 0) {
        size_t index = 0;
        if (str[index] == '-')
            result_sign = -1, index++;
        if (index == str.size())
            throw std::runtime_error("Invalid string in input");
        bool is_zero = true;
        for (; index < str.size(); ++index) {
            char current = str[index];
            if (isdigit(current)) {
                if (current != '0')
                    is_zero = false;
                *this *= 10;
                *this += (current - '0');
            }
            else
                throw std::runtime_error("Invalid string in input");
        }
        sign = (char) (is_zero ? 0 : result_sign);
    }
    else
        throw std::runtime_error("Invalid string in input");
}

big_integer::~big_integer() {
    sign = 0;
    elements.clear();
}

big_integer &big_integer::operator=(big_integer const &other) {
    sign = other.sign;
    if(&elements != &other.elements)
    {
        elements.clear();
        for(size_t i = 0; i < other.elements.size(); ++i)
            elements.push_back(other.elements[i]);
    }
    return *this;
}

big_integer &big_integer::operator+=(big_integer const &rhs) {
    if (!sign) {
        (*this) = rhs;
        return *this;
    }
    
    if (sign * rhs.sign >= 0) {
        bool overflow = 0;
        for (size_t i = 0; i < std::max(elements.size(), rhs.elements.size()) || overflow; ++i) {
            if (i == elements.size())
                elements.push_back(0);
            int64_t digit = (i < rhs.elements.size() ? rhs.elements[i] : 0);
            int64_t current = elements[i] + digit + overflow;
            overflow = current > (int64_t) base;
            elements[i] = (uint32_t) (overflow ? current - base - 1 : current);
        }
    }
    else {
        sign = rhs.sign;
        if ((*this) == rhs)
            return *this = ZERO;
        if (rhs.sign == -1)
            sign = 1, *this -= -rhs;
        else
            *this = rhs - (*this);
    }
    
    __delete_zeroes();
    return *this;
}

big_integer &big_integer::operator-=(big_integer const &rhs) {
    if (!rhs.sign)
        return *this;
    if (!sign)
        return *this = -rhs;
    
    if (sign == 1 && rhs.sign == -1)
        return (*this) += (-rhs);
    if (sign == -1 && rhs.sign == 1)
        return *this = -(-(*this) + rhs);
    
    if ((*this) == rhs)
        return *this = ZERO;
    
    if ((sign == 1 && *this >= rhs) || (sign == -1 && *this <= rhs)) {
        bool overflow = 0;
        for (size_t i = 0; i < rhs.elements.size() || overflow; ++i) {
            int64_t digit = (i < rhs.elements.size() ? rhs.elements[i] : 0);
            int64_t current = elements[i] - digit - overflow;
            overflow = current < 0;
            elements[i] = (uint32_t) (overflow ? current + base + 1 : current);
        }
        __delete_zeroes();
        return *this;
    }
    return *this = sign == 1 ? -(rhs - (*this)) : (-rhs) + (*this);
}

big_integer &big_integer::operator*=(big_integer const &rhs) {
    
    if (!sign || !rhs.sign)
        return *this = ZERO;
    sign = sign * rhs.sign;
    
    std::vector<uint32_t> ans(elements.size() + rhs.elements.size());
    for (size_t i = 0; i < elements.size(); ++i) {
        uint32_t carry = 0;
        for (size_t j = 0; j < rhs.elements.size() || carry; ++j) {
            uint32_t rdigit = (j < rhs.elements.size() ? rhs.elements[j] : 0);
            uint64_t current = ans[i + j] + 1ULL * elements[i] * rdigit + carry;
            ans[i + j] = (uint32_t) (current & (1ULL * base));
            carry = (uint32_t) (current >> blen);
        }
    }
    
    elements.resize(ans.size());
    for (size_t i = 0; i < ans.size(); ++i)
        elements[i] = ans[i];
    __delete_zeroes();
    return *this;
}

big_integer &big_integer::operator/=(int const &rhs) {
    if (!sign)
        return *this = ZERO;
    int32_t r = rhs;
    
    int32_t carry = 0;
    sign = (char) (((sign == -1) ^ (r < 0)) ? -1 : 1);
    if (r < 0)
        r = -r;
    for (int32_t i = (int) elements.size() - 1; i >= 0; --i) {
        int64_t current = elements[i] + carry * (1LL * base + 1);
        lldiv_t result = lldiv(current, r);
        elements[i] = (uint32_t) result.quot;
        carry = (int32_t) result.rem;
    }
    
    __delete_zeroes();
    return *this;
}

big_integer &big_integer::operator/=(big_integer const &rhs) {
    if (!rhs.sign)
        throw std::runtime_error("Division by zero!");
    if (!sign)
        return *this = ZERO;
    if (rhs.elements.size() == 1) {
        return *this = (*this / (rhs.sign * rhs.elements[0]));
    }
    big_integer r = big_integer(rhs);
    char need_sign = r.sign * sign;
    sign = 1;
    r.sign = 1;
    if ((*this) < r)
        return *this = ZERO;
    
    int32_t n = (int32_t) r.elements.size();
    int32_t m = (int32_t) (elements.size() - n);
    if (r.elements[n - 1] <= (base >> 1)) {
        int64_t tmp = (1LL * base + 1) / (r.elements[n - 1] + 1);
        *this *= tmp;
        r *= tmp;
        n = (int32_t) r.elements.size();
        m = (int32_t) (elements.size() - n);
    }
    
    std::vector<uint32_t> result((uint64_t) (m + 1));
    big_integer y = r << (m * blen);
    if (*this >= y) {
        result[m] = 1;
        *this -= y;
    }
    
    for (int j = m - 1; j >= 0; --j) {
        if ((int) elements.size() <= n + j)
            break;
        
        int32_t len = (int32_t) y.elements.size();
        for (size_t i = r.elements.size(); i > 0; --i)
            y.elements[len - i - 1] = y.elements[len - i];
        y.elements.pop_back();
        
        uint64_t quot = (elements[n + j] * (1ULL * base + 1) + elements[n + j - 1]) / r.elements[n - 1];
        quot = std::min(quot, 1ULL * base);
        uint32_t res = (uint32_t) quot;
        
        *this -= y * res;
        
        while (sign == -1) {
            *this += y;
            --res;
        }
        result[j] = (uint32_t) res;
    }
    
    big_integer res;
    res.sign = need_sign;
    res.elements = result;
    res.__delete_zeroes();
    return *this = res;
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    if (!sign)
        return *this;
    
    *this -= (*this / rhs) * rhs;
    __delete_zeroes();
    return *this;
}

uint32_t and_func(uint32_t x, uint32_t y) {
    return x & y;
}

uint32_t or_func(uint32_t x, uint32_t y) {
    return x | y;
}

uint32_t xor_func(uint32_t x, uint32_t y) {
    return x ^ y;
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
    __binary_operation(and_func, rhs, (char) and_func((uint32_t) this->sign, (uint32_t) rhs.sign));
    return *this;
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
    __binary_operation(or_func, rhs, (char) or_func((uint32_t) this->sign, (uint32_t) rhs.sign));
    return *this;
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    char need_sign = (char) (((sign == -1) ^ (rhs.sign == -1)) ? -1 : 1);
    __binary_operation(xor_func, rhs, need_sign);
    return *this;
}

big_integer &big_integer::operator<<=(int rhs) {
    if (rhs < 0)
        throw std::runtime_error("Cannot shift to negative value");
    
    if (sign == -1)
        __to_additional_code();
    
    div_t result = div(rhs, blen);
    if (result.rem) {
        int64_t carry = 0;
        int64_t BASE = base;
        for (size_t i = 0; i < elements.size() || carry; ++i) {
            if (i == elements.size())
                elements.push_back(0);
            int64_t current = ((1LL * elements[i]) << result.rem) + carry;
            elements[i] = (uint32_t) (current & BASE);
            carry = current >> blen;
        }
    }
    
    uint32_t new_size = (uint32_t) (elements.size() + result.quot);
    elements.resize(new_size);
    for (int32_t i = new_size - 1; i >= result.quot; --i)
        elements[i] = elements[i - result.quot];
    for (int32_t i = result.quot - 1; i >= 0; --i)
        elements[i] = 0;
    
    if (sign == -1)
        __to_additional_code();
    
    __delete_zeroes();
    return *this;
}

big_integer &big_integer::operator>>=(int rhs) {
    if (rhs < 0)
        throw std::runtime_error("Cannot shift to negative value");
    
    if (sign == -1)
        __to_additional_code();
    
    div_t result = div(rhs, blen);
    
    int32_t new_size = (int32_t) (elements.size() - result.quot);
    for (size_t i = (size_t) result.quot; i < elements.size(); ++i)
        elements[i - result.quot] = elements[i];
    for (size_t i = elements.size() - result.quot; i < elements.size(); ++i)
        elements[i] = 0;
    elements.resize((unsigned long) new_size);
    
    if (result.rem) {
        int64_t carry = 0;
        if (sign == -1)
            carry = base;
        int64_t BASE = base;
        int power = blen - result.rem;
        for (int i = (int) elements.size() - 1; i >= 0; --i) {
            int64_t shl = (1LL * elements[i]) >> result.rem;
            int64_t current = shl + (carry << power);
            carry = elements[i] - (shl << result.rem);
            elements[i] = (unsigned int) (current & BASE);
        }
    }
    
    if (sign == -1)
        __to_additional_code();
    
    __delete_zeroes();
    return *this;
}

big_integer big_integer::operator-() const {
    big_integer r = big_integer(*this);
    r.sign = -r.sign;
    return r;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator~() const {
    big_integer r = big_integer(*this);
    r = -r - 1;
    r.__delete_zeroes();
    return r;
}

big_integer &big_integer::operator++() {
    *this += big_integer(1);
    return *this;
}

big_integer big_integer::operator++(int) {
    big_integer r = *this;
    ++(*this);
    return r;
}

big_integer &big_integer::operator--() {
    *this -= big_integer(1);
    return *this;
}

big_integer big_integer::operator--(int) {
    big_integer r = *this;
    --(*this);
    return r;
}

big_integer operator+(big_integer a, big_integer const &b) {
    return a += b;
}

big_integer operator-(big_integer a, big_integer const &b) {
    return a -= b;
}


big_integer operator*(big_integer a, big_integer const &b) {
    return a *= b;
}

big_integer operator/(big_integer a, int const &b) {
    return a /= b;
}

big_integer operator/(big_integer a, big_integer const &b) {
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const &b) {
    return a %= b;
}

big_integer operator&(big_integer a, big_integer const &b) {
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const &b) {
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const &b) {
    return a ^= b;
}

big_integer operator<<(big_integer a, int b) {
    return a <<= b;
}

big_integer operator>>(big_integer a, int b) {
    return a >>= b;
}

bool operator==(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign || a.elements.size() != b.elements.size())
        return false;
    
    for (size_t i = 0; i < a.elements.size(); ++i)
        if (a.elements[i] != b.elements[i])
            return false;
    return true;
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return !(a == b);
}

bool operator<(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign)
        return a.sign < b.sign;
    if (a.elements.size() != b.elements.size())
        return a.sign == 1 ? a.elements.size() < b.elements.size() : a.elements.size() > b.elements.size();
    
    for (int i = (int) a.elements.size() - 1; i >= 0; --i)
        if (a.elements[i] != b.elements[i])
            return a.sign == 1 ? a.elements[i] < b.elements[i] : a.elements[i] > b.elements[i];
    return false;
}

bool operator<=(big_integer const &a, big_integer const &b) {
    return a == b ? true : a < b;
}

bool operator>(big_integer const &a, big_integer const &b) {
    return !(a <= b);
}

bool operator>=(big_integer const &a, big_integer const &b) {
    return !(a < b);
}

std::string to_string(big_integer const &a) {
    if (!a.elements.size())
        throw std::runtime_error("Cannot output this number: no constructors were called");
    if (!a.sign)
        return "0";
    
    std::string number = "";
    char sign = a.sign;
    big_integer b = big_integer(a);
    big_integer divisor = 10;
    while (b.sign) {
        big_integer last_digit = b % divisor;
        number += char('0' + last_digit.elements[0]);
        b /= divisor;
    }
    if (sign == -1)
        number += '-';
    
    reverse(number.begin(), number.end());
    return number;
}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    return s << to_string(a);
}


