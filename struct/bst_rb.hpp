#ifndef EBA_BST_RB_H
#       define EBA_BST_RB_H
#       include "bst_naive.hpp"
namespace eba {
        struct bst_rb_node_addon {
                bool red;
                bst_rb_node_addon(): red(false) {}
        };
        template<
                typename TValue,
                bool value_comparator (const TValue&, const TValue&) = default_comparator<TValue>
        > class bst_rb :  public bst_naive<TValue, bst_rb_node_addon, value_comparator>{
                typedef bst_naive<TValue, bst_rb_node_addon, value_comparator> par;
                typedef typename par::node node;
                inline bool red(node *n) {
                        if(n)
                                return n->red;
                        return false;
                }
                public:
                void insert(TValue value) {
                        node *nd = par::path(value, true);
                        if(nd) {
                                nd->red = true;
                                for(node *tmp; red(nd->par); ) {
                                        bool t = nd->par == nd->par->par->ch[0];
                                        tmp = nd->par->par->ch[t];
                                        if (red(tmp)) {
                                                nd->par->red = tmp->red = false;
                                                nd->par->par->red = true;
                                                nd = nd->par->par;
                                        }
                                        else {
                                                if (nd == nd->par->ch[t]) {
                                                    nd = nd->par;
                                                    par::rotate_node(nd, !t);
                                                }
                                                nd->par->red = false;
                                                nd->par->par->red = true;
                                                par::rotate_node(nd->par->par,t);
                                        }
                                }
                                par::root->red = false;
                        }
                }
                void remove(TValue value) {
                        node *nd = par::path(value, false), *y = nd, *z;
                        if (!nd) return;
                        bool tmp = y->red;
                        if (nd->ch[0] && nd->ch[1]) {
                                y = par::subtree_minmax(nd->ch[1],0);
                                tmp = y->red;
                                z = y->ch[1];
                                if (y->par == nd) {
                                        if(z)
                                                z->par = y;
                                } else {
                                        transplant(y, y->ch[1]);
                                        y->ch[1] = nd->ch[1];
                                        y->ch[1]->par = y;
                                }
                                transplant(nd, y);
                                y->ch[0] = nd->ch[0];
                                y->ch[0]->par = y;
                                y->red = nd->red;
                        } else {
                            z = (nd->ch[0])? nd->ch[1] : nd->ch[0];
                            transplant(nd, z);
                        }
                        node *del = nd;
                        if (!tmp) {
                                node *tmp; 
                                while (nd->par && !nd->red) {
                                        bool t = nd == nd->par->ch[0];
                                        tmp = nd->par->ch[t];
                                        if (red(tmp)) {
                                                tmp->red = false;
                                                nd->par->red = true;
                                                par::rotate_node(nd->par, !t);
                                                tmp = nd->par->ch[t];
                                        }
                                        if (tmp && !red(tmp->ch[0]) && !red(tmp->ch[1])) {
                                                tmp->red = true;
                                                nd = nd->par;
                                        } else {
                                                if (tmp && !red(tmp->ch[t])) {
                                                        tmp->ch[!t]->red = false;
                                                        tmp->red = true;
                                                        par::rotate_node(tmp,t);
                                                        tmp = nd->par->ch[t];
                                                }
                                                if(tmp) {
                                                        tmp->red = red(nd->par);
                                                        if(tmp->ch[t])
                                                                tmp->ch[t]->red = false;
                                                }
                                                nd->par->red = false;
                                                if(tmp)
                                                        par::rotate_node(nd->par,!t);
                                                nd = par::root;
                                        }
                                }
                                nd->red = false;
                        }
                        delete del;
                }
                void transplant(node *u, node *v) {
                    if (!u->par) {
                            par::root = v;
                    }
                    else {
                        u->par->ch[u == u->par->ch[1]] = v;
                    }
                    if(v && u)
                            v->par = u->par;
                }
        };
};
#endif
// vim: et:ts=8:sw=8
