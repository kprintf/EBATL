#ifndef EBA_COMMON_HPP
#       define EBA_COMMON_HPP
#       ifndef EBA_NOSTL
#               include <cstddef>
#               include <cstdint>
#               include <algorithm>
#               include <utility>
#       endif
namespace eba {

#       ifdef EBA_NOSTL
        template<bool b> struct ____hack_for_size_t;
        
        template<>
        struct ____hack_for_size_t<1> {
                typedef unsigned long long int type;
        };
        
        template<>
        struct ____hack_for_size_t<0> {
                typedef unsigned int type;
        };

        typedef ____hack_for_size_t<sizeof(void*) == sizeof(unsigned long long int)>::type size_t;


        template<typename T> void swap(T& a, T& b) {
                T tmp;
                tmp = a;
                a = b;
                b = tmp;        
        }

        template<typename T> T max(T a, T b) {
                if(b < a)
                        return a;
                else return b;
        }

#       else
        typedef std::size_t size_t;
        
        template<typename T> void swap(T& a, T& b) {
               std::swap<T>(a,b);
        }
        
        template<typename T> T max(T a, T b) {
                return std::max<T>(a,b);
        }
        
#       endif

        template<typename T> T abs(T a) {
                if(0 < a)
                        return a;
                return -a;
        }

        template<typename T>
        bool default_comparator(const T& a, const T& b) {
                return a < b;
        }

        struct empty_struct {};

        
        void beep() {}
};
#endif
// vim: et:ts=8:sw=8
