#pragma once

#include <string>
#include <vector>
#include <utility>

class TSTree{
    union TRB{
        size_t Inner;
        size_t* Leaf;
    };
    class TNode{
    public:
        std::vector<std::pair<std::pair<size_t, TRB>, TNode*> > Transition;
        size_t SufInd;
        TNode* Link;

        TNode();

        ~TNode();
    };
private:
    static const char Terminator = '$';
    size_t fin;
    std::string Text;
    TNode* Root;
    std::vector<size_t> Lcp;
    size_t Append(size_t pos, size_t stState);
    void Insert(TNode& node, std::pair<std::pair<size_t, TRB>, TNode*>& newTrans);
    size_t GetLcp(size_t ind1, size_t ind2);
    void InDpth(size_t& pos, size_t& k, TNode* node, std::vector<size_t>& sArr, size_t* prevStrDpth, size_t strDpth, bool first);
public:
    TSTree();
    TSTree(std::string& text);   //Ukkonen's algo. It makes an implicit tree, then makes it
                                //explicit. One must not append any terminator to string before
                                //passing it to constructor.
    void Array(std::vector<size_t>& sArr);
    std::pair<int,int> ArrSearch(std::vector<size_t>& sArr, std::string& pattern);

    ~TSTree();
};
