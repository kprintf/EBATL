/*TODO applications: joker, multijoker (2 ways), submatrix
 */
#ifndef EBA_AHO_CORASICK_HPP
#       define EBA_AHO_CORASICK_HPP
#       include <bitset>
#       include "../struct/map.hpp"
#       include "../common/common.hpp"
using std::bitset;
namespace eba {
        template <
                typename TString = string,
                typename TChar = char,
                typename TOperator = char(TString, size_t),
                TOperator char_fetch = default_string_fetch<TString>,
                TChar end = '\0',
                typename TBitset = bitset<32>,
                typename TMap = map<TChar, void*>
        > class aho_corasick_automat {
                struct node {
                        TMap v;
                        node *suflink, *par;
                        TBitset found;
                        node(node *p): suflink(NULL), par(p) {}
                };
                typedef node *nodp;
                nodp root, pos;
                struct wavemark{
                        TChar ch;
                        nodp p;
                        wavemark(TChar c, nodp pa=NULL): ch(c), p(pa) {}
                };
                public:
                aho_corasick_automat(): root(NULL){}
                void insert(int no, const TString &str) {
                        int i = 0;
                        if(!root) root = new node(NULL);
                        node *n = root;
                        do {
                                if(!(n->v[char_fetch(str, i)]))
                                        n->v[char_fetch(str, i)] = new node(n);
                                n = nodp(n->v[char_fetch(str, i)]);
                        }
                        while(char_fetch(str, ++i) != end);
                        n->found[no] = 1;
                }
                void build_automat() {
                        vector<wavemark> wave[2], *cur=&wave[0], *nxt=&wave[1];
                        pos = root;
                        cur->push_back(wavemark(end, pos));
                        while(!cur->empty()) {
                                nxt->clear();
                                for(wavemark x: *cur) {
                                        for(auto y: x.p->v) {
                                                nxt->push_back(wavemark(y.first, nodp(y.second)));
                                        }
                                        if(x.p->par) {
                                                nodp y = x.p->par;
                                                while(y->suflink && y != root) {
                                                        if(y->suflink->v[x.ch]) {
                                                                x.p->suflink = nodp(y->suflink->v[x.ch]);
                                                                break;
                                                        }
                                                        y = y->suflink;
                                                }
                                                if(!x.p->suflink && x.p->par != root)
                                                        if(root->v[x.ch])
                                                                x.p->suflink = nodp(root->v[x.ch]);
                                        }
                                }
                                swap(cur, nxt);
                        }
                }
                
                void feed_char(TChar ch) {
                        while(pos && !pos->v[ch] && pos != root) pos = pos->suflink;
                        if(!pos) pos = root;
                        if(pos->v[ch])
                                pos = nodp(pos->v[ch]);
                        else pos = root;
                }

                TBitset& get_state() {
                        return pos->found;
                }
        };

        template <
                typename TCallback,
                typename TStringList,
                int MAX_PATTERNS = 32,
                typename TString = string,
                typename TChar = char,
                typename TOperator = char(&)(TString, size_t),
                TOperator char_fetch = default_string_fetch<TString>,
                TChar end = '\0'
        > void text_find_aho_corasick(TString str, TStringList wut, TCallback find) {
                aho_corasick_automat<
                        TString,
                        TChar,
                        TOperator,
                        char_fetch,
                        end,
                        bitset<MAX_PATTERNS> 
                        > automat;
                int cnt = 0;
                for(TString pattern: wut) automat.insert(cnt++, pattern);
                automat.build_automat();
                for(int i = 0; char_fetch(str, i) != end; i++) {
                        automat.feed_char(char_fetch(str, i));
                        bitset<MAX_PATTERNS> &set = automat.get_state();
                        for(int j = 0; j < cnt; j++) {
                                if(set[j])
                                        find(i, j);
                        }
                }
        }
};
#endif
// vim: et:ts=8:sw=8
