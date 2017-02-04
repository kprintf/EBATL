#ifndef EBA_KMP_HPP
#       define EBA_KMP_HPP
#       include "prefix.hpp"
namespace eba {

        /* Knuth-Morris-Pratt version: online mode, prefix based. sp/f are omitted.
         * */
        template <
                typename TCallback,
                typename TString = string,
                typename TChar = char,
                typename TOperator = char(TString, size_t),
                TOperator char_fetch = default_string_fetch,
                TChar end = '\0'
        > void text_find_kmp_prefix(TString str, TString wut, TCallback find) {
                vector<size_t> ps;
                calc_prefix<TString, TChar, TOperator, char_fetch>(ps,wut);
                for(size_t i = 0, pi=0; char_fetch(str, i) != end; i++) {
                        for(; pi && str[i]!=wut[pi]; pi = ps[pi-1]);
                        if(char_fetch(str, i) == char_fetch(wut, pi)) pi++;
                        if(pi == ps.size()) {
                                find(i-ps.size());
                                pi = ps[pi-1];
                        }
                }
        }
}
#endif
// vim: et:ts=8:sw=8
