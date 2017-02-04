#ifndef EBA_BST_NAIVE_HPP
#       define EBA_BST_NAIVE_HPP
#       include "../common/common.hpp"
namespace eba {
        template<
                typename TValue,
                typename TNode = empty_struct,
                bool value_comparator (const TValue&, const TValue&) = default_comparator<TValue>
        > class bst_naive {
                protected:
                struct node:TNode {
                        TValue val;
                        node *ch[2], *par;
                        node(TValue vl, node *pr = 0): val(vl), par(pr) {ch[0] = ch[1] = 0;}
                };
                node *root;
                node* path(TValue vl, bool create = false) {
                        if(!root) {
                                if(create)
                                        return (root = new node(vl));
                                else 
                                        return 0;
                        }
                        for(node *n = root; n; ) {
                                if(!bool(value_comparator(n->val, vl)) &&
                                   !bool(value_comparator(vl, n->val)) )
                                        return n;
                                if(n->ch[bool(value_comparator(n->val, vl))])
                                        n = n->ch[bool(value_comparator(n->val, vl))];
                                else if(create) 
                                        return n->ch[bool(value_comparator(n->val, vl))] = new node(vl, n);
                                else return 0;
                        }
                }
                static node* subtree_minmax(node *nd, bool mx) {
                        for(;nd&&nd->ch[mx]; 
                            nd = nd->ch[mx]);
                        return nd;
                }
                void rotate_node(node *nd, bool r) {
                        node *tmp = nd->ch[!r];
                        nd->ch[!r] = tmp->ch[r];
                        if (nd->ch[!r])
                                tmp->ch[r]->par = nd;
                        tmp->par = nd->par;
                        if (!nd->par) 
                                root = tmp;
                        else 
                                nd->par->ch[nd == nd->par->ch[1]] = tmp;
                        tmp->ch[r] = nd;
                        nd->par = tmp;
                }
                void free_subtree(node *nd) {
                        if(!nd) return;
                        free_subtree(nd->ch[0]);
                        free_subtree(nd->ch[1]);
                        delete nd;
                }
                public:
                bst_naive(): root(0) {}
                ~bst_naive(){ 
                        free_subtree(root);
                }
                class iterator {
                        node *ptr;
                        void mv(bool nxt) {
                                if(!ptr) return;
                                if(ptr->ch[nxt])
                                        ptr = subtree_minmax(ptr->ch[nxt], !nxt);
                                else {
                                        while(ptr->par && ptr->par->ch[nxt] == ptr)
                                                ptr = ptr->par;
                                        ptr = ptr->par;
                                }
                        }
                        public:
                        iterator(node *n): ptr(n) {}
                        void operator++() {
                                mv(true);
                        }
                        void operator--() {
                                mv(false);
                        }
                        TValue& operator*() {
                                return ptr->val;
                        }
                        bool operator ==(const iterator &a) {
                                return ptr == a.ptr;
                        }
                        bool operator !=(const iterator &a) {
                                return ptr != a.ptr;
                        }
                };
                iterator begin() {
                        return iterator(subtree_minmax(root, 0));
                }
                iterator end() {
                        return iterator(0);
                }
                void insert(TValue vl) {
                        path(vl, true);
                }
                bool find(TValue vl) {
                        return bool(path(vl, false));
                }
                void remove(TValue vl) {
                        node *n;
                        if((n = path(vl, false))) {
                                if(n->ch[0] && n->ch[1]) {
                                        node *p = subtree_minmax(n->ch[0], 1);
                                        swap(n->val, p->val);
                                        p->par->ch[p->par->ch[1]==p] = 0;
                                        delete p;
                                }
                                else{
                                        if(n->par) {
                                                node *t = n->ch[0]?n->ch[0] : n->ch[1];
                                                if(t)
                                                        t->par = n->par;
                                                n->par->ch[n->par->ch[1]==n] = t;
                                        }
                                        else { 
                                                root = n->ch[0]?n->ch[0] : n->ch[1] ;
                                                root->par = 0;
                                        }
                                        delete n;
                                }
                        }
                }
        };
}
#endif
// vim: et:ts=8:sw=8
