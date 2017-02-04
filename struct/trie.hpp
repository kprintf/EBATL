/*TODO: tst as alternative to map<> addiction*/
#ifndef EBA_TRIE_HPP
#       define EBA_TRIE_HPP
#       include "../common/string.hpp"
#       include "../common/common.hpp"
#       include "map.hpp"
namespace eba {
        template <
                typename TString = string,
                typename TChar = char,
                typename TNodeValue = empty_struct,
                typename TMap = map<TChar, void*>,
                typename TOperator = char(*)(TString, size_t),
                TOperator char_fetch = default_string_fetch,
                TChar end = '\0'
        > class trie {
                private:
                struct node {
                        TMap v;
                        TNodeValue val;
                };
                typedef node *nodp;
                nodp root;
                size_t cnt;
                public:
                trie(): root(NULL), cnt(0) {}
                bool find(const TString &str, TNodeValue **res = NULL) {
                        int i = 0;
                        node *n = root;
                        if(!n) return false;
                        do if(!(n=nodp(n->v[char_fetch(str, i)]))) return false;
                        while(n && char_fetch(str, i++) != end);
                        if(res) *res = &n->val;
                        return true;
                }
                size_t size() const{
                        return cnt;
                }
                void insert(const TString &str) {
                        int i = 0;
                        if(!root) root = new node;
                        node *n = root;
                        do {
                                if(!(n->v[char_fetch(str, i)]))
                                        n->v[char_fetch(str, i)] = new node;
                                n = nodp(n->v[char_fetch(str, i)]);
                        }
                        while(n && char_fetch(str, i++) != end);
                        cnt++;
                }
                void remove(const TString &str) {
                        int i = 0, ni = 0;
                        node *n = root, *del = root;
                        if(!n) return ;
                        do {
                                if(!n)
                                        return;
                                if(!nodp(n->v[char_fetch(str, i)]))
                                        return;
                                if(nodp(n->v[char_fetch(str, i)])->v.size() < 2 && n->v.size() >= 2)
                                        del = n, ni = i;
                        }
                        while((n=nodp(n->v[char_fetch(str, i)])) && char_fetch(str, i++) != end);
                        i = ni;
                        n = nodp(del->v[char_fetch(str, i)]);
                        del->v.erase(char_fetch(str, i++));
                        do {
                                del = n;
                                n = nodp(n->v[char_fetch(str, i)]);
                                delete del;
                        }
                        while(n&&(n=nodp(n->v[char_fetch(str, i)])) && char_fetch(str, i++) != end) ;
                        cnt--;
                }
        };
};
#endif
// vim: et:ts=8:sw=8
