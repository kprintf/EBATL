#ifndef INT_FLEXIBLE_HPP
#       define INT_FLEXIBLE_HPP
#       include "../common/common.hpp"
#       include <iostream>
#       include <iomanip>
#       include <cstdlib>
using std::atoi;
namespace eba {
        enum int10u_flexible_exception {
                UNDRFLOW,
                WRONGSUB,
                DIV_ZERO,
                ZEROPOW0
        };
        class int10u_flexible {
                vector < uint32_t > data;
                static const uint64_t BASE = 1000000000;
                static const int32_t DIGITS = 9;

                void increment (size_t seg = 0) {
                        for (; seg < data.size () && data[seg] == BASE; data[seg++] = 0);
                        if (seg == data.size ()) data.push_back (1);
                        else data[seg]++;
                }
                void decrement (int seg = 0) {
                        for (;seg < int (data.size ()) && data[seg] == 0; seg++); 
                        if (seg == int (data.size ())) 
                                throw int10u_flexible_exception (UNDRFLOW);
                        else    for (data[seg--]--; seg >= 0 && data[seg] == 0; seg--) 
                                        data[seg] = BASE - 1;
                }
                void ntrn_sub (uint64_t sub, size_t i = 0) {
                        if (i >= data.size ()) 
                                throw int10u_flexible_exception (UNDRFLOW);
                        uint64_t tmp = data[i];
                        if (tmp < sub) {
                                uint64_t x = (sub - tmp + BASE - 1) / BASE;
                                ntrn_sub (x, i + 1);
                                tmp += BASE * x;
                        }
                        tmp -= sub;
                        data[i] = tmp;
                }
                void ntrn_add (uint64_t add, size_t i = 0) {
                        if (data.size () <= i) 
                                data.resize (i + 1);
                        uint64_t sum = data[i] + add;
                        if (sum >= BASE) {
                                ntrn_add (sum / BASE, i + 1);
                                sum %= BASE;
                        }
                        data[i] = sum;
                }
                void ntrn_mul (uint64_t mul) {
                        mul--;
                        if (!mul) return;
                        vector < uint32_t > orig (data);
                        for (size_t i = 0, sz = orig.size (); i < sz; i++) {
                                uint64_t x = orig[i] * mul;
                                ntrn_add (x, i);
                        }
                }
                void fast_div2 () {
                        for (size_t i = 0; i < data.size (); i++) {
                                data[i] /= 2;
                                if (i + 1 < data.size () && data[i + 1] % 2) {
                                        data[i] += BASE / 2;
                                }
                        }
                }
                void head_fix () {
                        while (!data.empty () && data.back () == 0) {
                                data.pop_back ();
                        }
                }
          public:
                int10u_flexible (uint64_t i = 0) {
                        do    data.push_back (i % BASE);
                        while (i /= BASE);
                }
                int10u_flexible (const int10u_flexible & a) {
                        data = a.data;
                        head_fix ();
                }
                explicit operator          bool () const {
                        return !data.empty () && !(data.size () == 1 && data[0] == 0);
                } 
                bool operator < (const int10u_flexible & a) {
                        if (data.size () != a.data.size ()) {
                                return data.size () < a.data.size ();
                        }
                        if (data.empty ()) {
                                return true;
                        }
                        int i;
                        for (i = data.size () - 1; i && data[i] == a.data[i]; i--);
                        return data[i] < a.data[i];
                }
                bool operator > (const int10u_flexible & a) {
                        if (data.size () != a.data.size ()) {
                                return data.size () > a.data.size ();
                        }
                        if (data.empty ()) {
                                return false;
                        }
                        int i;
                        for (i = data.size () - 1; i && data[i] == a.data[i]; i--);
                        return data[i] > a.data[i];
                }
                bool operator == (const int10u_flexible & a) {
                        if (data.size () != a.data.size ()) {
                                return false;
                        }
                        if (data.empty ()) {
                                return false;
                        }
                        int i;
                        for (i = data.size () - 1; i && data[i] == a.data[i]; i--);
                        return data[i] == a.data[i];
                }
                bool operator != (const int10u_flexible & a) {
                        if (data.size () != a.data.size ()) {
                                return true;
                        }
                        if (data.empty ()) {
                                return true;
                        }
                        int i;
                        for (i = data.size () - 1; i && data[i] == a.data[i]; i--);
                        return data[i] != a.data[i];
                }
                bool operator >= (const int10u_flexible & a) {
                        if (data.size () != a.data.size ()) {
                                return data.size () > a.data.size ();
                        }
                        if (data.empty ()) {
                                return false;
                        }
                        int i;
                        for (i = data.size () - 1; i && data[i] == a.data[i]; i--);
                        return data[i] >= a.data[i];
                }
                bool operator <= (const int10u_flexible & a) {
                        if (data.size () != a.data.size ()) {
                                return data.size () < a.data.size ();
                        }
                        if (data.empty ()) {
                                return true;
                        }
                        int i;
                        for (i = data.size () - 1; i && data[i] == a.data[i]; i--);
                        return data[i] <= a.data[i];
                }
                int10u_flexible & operator += (const int10u_flexible & a) {
                        if (a.data.size () > data.size ()) {
                                data.resize (a.data.size ());
                        }
                        for (size_t i = 0; i < a.data.size (); i++) {
                                ntrn_add (a.data[i], i);
                        }
                        head_fix ();
                        return *this;
                }
                int10u_flexible & operator -= (const int10u_flexible & a) {
                        if (a.data.size () > data.size ()) {
                                throw int10u_flexible_exception (WRONGSUB);
                        }
                        for (size_t i = 0; i < a.data.size (); i++) {
                                ntrn_sub (a.data[i], i);
                        }
                        head_fix ();
                        return *this;
                }
                int10u_flexible & operator *= (const int10u_flexible & a) {
                        int10u_flexible cp (*this), cp_a (a);
                        data.assign (cp.data.size () + a.data.size () + 1, 0);
                        for (size_t i = 0; i < cp.data.size (); i++) {
                                if (cp.data[i]) {
                                        for (size_t j = 0; j < cp_a.data.size (); j++)
                                                ntrn_add (uint64_t (cp.data[i]) *
                                                                         uint64_t (cp_a.data[j]), i + j);
                                }
                        }
                        head_fix ();
                        return *this;
                }
                int10u_flexible & operator ^= (const int10u_flexible & p) {
                        if (!bool (*this) && !bool (p)) {
                                throw int10u_flexible_exception (ZEROPOW0);
                        }
                        int10u_flexible a (*this), pow (p);
                        data.clear ();
                        data.push_back (1);
                        while (bool (pow)) {
                                if (pow.data[0] % 2) {
                                        *this *= a;
                                        pow.decrement (0);
                                }
                                else {
                                        a *= a;
                                        pow.fast_div2 ();
                                }
                                pow.head_fix ();
                        }
                        return *this;
                }
                int10u_flexible & operator /= (const int10u_flexible & a) {
                        if (!bool (a)) throw int10u_flexible_exception (DIV_ZERO);
                        if ((a.data.size () == 1 && a.data[0] == 1) || !bool (*this)) return *this;
                        if (a.data.size () > data.size ()) {
                                data.clear (), data.push_back (0);
                        }
                        else {
                                int10u_flexible u (*this), v (a);
                                int n = a.data.size (), m = data.size ();
                                uint64_t d = BASE / uint64_t (a.data[n - 1] + 1), q_, r_;
                                u.ntrn_mul (d);
                                v.ntrn_mul (d);
                                u.data.push_back (0);
                                v.data.push_back (0);
                                data.assign (data.size () - a.data.size () + 1, 0);
                                for (int j = m - n; j >= 0; j--) {
                                        q_ = (u.data[n + j] * BASE + (u.data[n + j - 1]))
                                                / v.data[n - 1];
                                        r_ = (u.data[n + j] * BASE + (u.data[n + j - 1]))
                                                % v.data[n - 1];
                                        while (r_ < BASE &&
                                                   (q_ == BASE || q_ * (n > 1 ? v.data[n - 2] : 0) >
                                                        r_ * BASE + (n + j > 1 ? u.data[n + j - 2] : 0))) {
                                                q_--;
                                                r_ += v.data[n - 1];
                                        }
                                        try {
                                                for (int k = n - 1; k >= 0; k--) {
                                                        u.ntrn_sub (q_ * v.data[k], j + k);
                                                }
                                        }
                                        catch (int10u_flexible_exception e) {
                                                if (e == UNDRFLOW) {
                                                        q_--;
                                                        for (int k = 0; k < n; k++) {
                                                                u.ntrn_add (v.data[k], j + k);
                                                        }
                                                }
                                        }
                                        data[j] = q_;
                                }
                        }
                        head_fix ();
                        return *this;
                }
                int10u_flexible & operator++ () {
                        increment ();
                        return *this;
                }
                int10u_flexible & operator-- () {
                        decrement ();
                        return *this;
                }
                int10u_flexible operator++ (int) {
                        int10u_flexible copy (*this);
                        ++*this;
                        return copy;
                }
                int10u_flexible operator-- (int) {
                        int10u_flexible copy (*this);
                        --*this;
                        return copy;
                }
                int10u_flexible operator+ (const int10u_flexible & a) {
                        int10u_flexible copy (*this);
                        copy += a;
                        return copy;
                }
                int10u_flexible operator- (const int10u_flexible & a) {
                        int10u_flexible copy (*this);
                        copy -= a;
                        return copy;
                }
                int10u_flexible operator/ (const int10u_flexible & a) {
                        int10u_flexible copy (*this);
                        copy /= a;
                        return copy;
                }
                int10u_flexible operator* (const int10u_flexible & a) {
                        int10u_flexible copy (*this);
                        copy *= a;
                        return copy;
                }
                int10u_flexible operator^ (const int10u_flexible & a) {
                        int10u_flexible copy (*this);
                        copy ^= a;
                        return copy;
                }
                friend std::ostream & operator << (std::ostream & a, const int10u_flexible & b);
                friend std::istream & operator >> (std::istream & a, int10u_flexible & b);
        };

        std::ostream & operator << (std::ostream & a, const int10u_flexible & b) {
                a << (bool (b) ? b.data[b.data.size () - 1] : 0);
                for (int sz = b.data.size () - 2; sz >= 0; sz--) 
                        a << std::setw (int10u_flexible::DIGITS) << std::setfill ('0') << b.data[sz];
                return a;
        }

        std::istream & operator >> (std::istream & a, int10u_flexible & b) {
                std::string str;
                a >> str;
                int ind = str.size (), secondInd;
                b.data.resize ((str.size () + int10u_flexible::DIGITS - 1) /
                                           int10u_flexible::DIGITS);
                secondInd = 0;
                do {
                        ind = max (0, ind - int10u_flexible::DIGITS);
                        b.data[secondInd++] =::atoi (&str[ind]);
                        str[ind] = 0;
                }
                while (ind);
                b.head_fix ();
                return a;
        }
}
#endif
// vim: et:ts=8:sw=8
