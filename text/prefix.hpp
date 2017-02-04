#ifndef EBA_PREFIX_HPP 
#       define EBA_PREFIX_HPP
#       ifdef EBA_NOSTL
#               error NOSTL mode is unimplemented yet
#       else
#       include <vector>
using std::vector;
#       endif
#       include "../common/string.hpp"
#       include "../common/common.hpp"
namespace eba {

        /* calculate values of prefix function on string, write in vector
         */
        template <
                typename TString,
                typename TChar,
                typename TOperator,
                TOperator char_fetch
        > void calc_prefix(vector<size_t> &dst, TString &inp) {
                dst.resize(inp.size()+1);
                for(size_t i = 1, j; i < inp.size(); i++) {
                        for(j = dst[i-1]; 
                            j && char_fetch(inp, i) != char_fetch(inp, j); 
                            j = dst[j-1]);
                        dst[i] = j + int(char_fetch(inp, j)==char_fetch(inp, i));
                }
        }
}
#endif
// vim: et:ts=8:sw=8
