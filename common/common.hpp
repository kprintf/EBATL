#ifndef EBA_COMMON_HPP
#       define EBA_COMMON_HPP
#       include <cstddef>
#       include <cstdint>
#       include <algorithm>
#       include <utility>
#       include <vector>
namespace eba {
        using std::size_t; 
        using std::swap; 
        using std::max; 
        using std::min; 
        using std::abs; 
        using std::vector;

        template<typename T>
        bool default_comparator(const T& a, const T& b) {
                return a < b;
        }

        struct empty_struct {};

        void beep() {}
};
#endif
// vim: et:ts=8:sw=8
