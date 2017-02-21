#include "tstree.h"
#include <queue>
#include <cstdlib>

size_t strCmp(std::string& text, std::string& pattern, size_t stPosText, size_t stPosPat, int& res){    //res == 1 - pattern is more than text
    size_t eq = stPosPat;
    size_t i = stPosText, j = stPosPat;
    for (; i < text.length() && j < pattern.length(); i++, j++){
        if (pattern[j] > text[i]){
            res = 1;
            return eq;
        }
        else if (pattern[j] < text[i]){
            res = -1;
            return eq;
        }
        else {
            eq++;
            if (j + 1 == pattern.length()){
                res = 0;
                return eq;
            }
        }
    }
    res = 1;
    return eq;
}


TSTree::TNode::TNode() {}

TSTree::TNode::~TNode(){
    for (size_t i = 0; i < Transition.size(); i++){
        delete Transition[i].second;
    }
}

size_t TSTree::Append(size_t pos, size_t stState){
    bool skip = false;
    bool toLink = false;
    TNode* nToLink;
    TNode* node = Root;
//    size_t& curEnd = fin;
    size_t retState = 0;
    size_t saveLB = 0;
    for (size_t i = stState; i < pos; i++){
        bool added = false;
        for (size_t j = 0; j < node->Transition.size(); j++){
            size_t lNB = i;
            if (node != Root){
                lNB = saveLB;
            }
            size_t lSB = node->Transition[j].first.first;
            size_t rSB;
            if (node->Transition[j].second->Transition.size() == 0){
                rSB = *node->Transition[j].first.second.Leaf;
            }
            else {
                rSB = node->Transition[j].first.second.Inner;
            }
            if (added){
                break;
            }
            if (Text[lNB] != Text[lSB]){
                continue;
            }
            added = true;
            bool leaf = false;
            //bool searched = false;
            while (pos - 1 - lNB > rSB - lSB){     //way searching
                //size_t k = j;
                    //searched = true;
                    //j = 0;
                    if (leaf){
                        break;
                    }
                    size_t k = 0;
                    lNB += rSB - lSB + 1;
                    node = node->Transition[j].second;
                    while (k < node->Transition.size()){
                        size_t indNewL = node->Transition[k].first.first;
                        size_t compInd = lNB;
                        if (Text[compInd] == Text[indNewL]){
                            lSB = node->Transition[k].first.first;
                            if (node->Transition[k].second->Transition.size() == 0){
                                rSB = *node->Transition[k].first.second.Leaf;
                                //j = k;
                                leaf = true;
                                //break;
                            }
                            else {
                                rSB = node->Transition[k].first.second.Inner;
                            }
                            j = k;
                            break;
                        }
                        k++;
                    }

            }
            if (pos - 1 - lNB == rSB - lSB){
                if (j > node->Transition.size() - 1){
                    TNode* newLeaf = new TNode();           //rule 2
                    newLeaf->Link = Root;
                    newLeaf->SufInd = i + 1;
                    TRB rightB;
                    rightB.Leaf = &fin;
                    std::pair<size_t, TRB> newSegm(i, rightB);
                    std::pair<std::pair<size_t, TRB>, TNode*> lShell(newSegm,newLeaf);
                    Insert(*node, lShell);
                    node = node->Link;
                    if (node != Root){
                        saveLB = lNB;
                    }
                    continue;
                }
                if (node->Transition[j].second->SufInd == 0){
                    if (toLink){
                        nToLink->Link = node->Transition[j].second;
                        toLink = false;
                    }
                    bool r3 = false;
                    TNode* link = node->Transition[j].second;
                    for (size_t k = 0; k < link->Transition.size(); k++){    //searching for rule 3
                        size_t compInd = link->Transition[k].first.first;
                        if (Text[compInd] == Text[pos]){
                            r3 = true;
                            break;
                        }
                    }
                    if (r3){        //rule 3
                        retState = i - 1;
                        skip = true;
                        node = node->Link;
                        if (node != Root){
                            saveLB = lNB;
                        }
                        continue;
                    }
                    TNode* newLeaf = new TNode();           //rule 2
                    newLeaf->Link = Root;
                    newLeaf->SufInd = i + 1;
                    TRB rightB;
                    rightB.Leaf = &fin;
                    std::pair<size_t, TRB> newSegm(pos, rightB);
                    std::pair<std::pair<size_t, TRB>, TNode*> lShell(newSegm,newLeaf);
                    Insert(*node->Transition[j].second, lShell);
                    node = node->Link;
                    if (node != Root){
                        saveLB = lNB;
                    }
                }
                /*else {                      //rule 1
                    node->Transition[j].first.second = pos;
                    node = node->Link;
                }*/
                else {
                    node = node->Link;
                    if (node != Root){
                        saveLB = lNB;
                    }
                    continue;
                }
                continue;
            }
            else {      //rules 2,3
                size_t length = lSB + (pos - lNB);
                if (Text[pos] == Text[length]){      //rule3
                    retState = i - 1;
                    skip = true;
                    if (toLink){
                        TNode* newNode = new TNode();
                        newNode->SufInd = 0;
                        size_t oldLB = node->Transition[j].first.first;
                        node->Transition[j].first.first = rSB - lSB + 1;
                        newNode->Transition.push_back(node->Transition[j]);
                        node->Transition[j].first.first = oldLB;
                        node->Transition[j].first.second.Inner = rSB - lSB;
                        node->Transition[j].second = newNode;
                        nToLink->Link = newNode;
                        nToLink = newNode;
                    }
                    node = node->Link;
                    if (node != Root){
                        saveLB = lNB;
                    }
                    continue;
                }
                TNode* newLeaf = new TNode();               //rule2
                newLeaf->SufInd = i + 1;
                newLeaf->Link = Root;
                TRB rightB;
                rightB.Leaf = &fin;
                std::pair<size_t, TRB> newSegm(pos, rightB);
                TNode* newNode = new TNode();
                newNode->SufInd = 0;
                newNode->Transition.push_back(std::pair<std::pair<size_t, TRB>, TNode*>(newSegm,newLeaf));
                //size_t oldLB = node->Transition[j].first.first;
                node->Transition[j].first.first = length;
                Insert(*newNode,node->Transition[j]);
                node->Transition[j].first.first = lSB;
                node->Transition[j].first.second.Inner = length - 1;
                node->Transition[j].second = newNode;
                if (toLink){
                    nToLink->Link = newNode;
                }
                toLink = true;
                nToLink = newNode;
                node = node->Link;
                if (node != Root){
                    saveLB = lNB;
                }
            }
        }
        if (skip){
            return retState;
        }
    }
    if (toLink){
        nToLink->Link = Root;
    }
    if (skip){
        return retState;
    }
    for (size_t i = 0; i < Root->Transition.size(); i++){       //searching for 'pos' symbol alone (rule 3)
        if (Text[pos] == Text[Root->Transition[i].first.first]){
            return pos - 1;
        }
    }
    TNode* newLeaf = new TNode();
    newLeaf->Link = Root;
    newLeaf->SufInd = pos + 1;
    TRB rightB;
    rightB.Leaf = &fin;
    std::pair<size_t,TRB> newSegm(pos,rightB);
    std::pair<std::pair<size_t, TRB>, TNode*> lShell(newSegm,newLeaf);
    Insert(*Root, lShell);
    return 0;
}

void TSTree::Insert(TNode& node, std::pair<std::pair<size_t, TRB>, TNode*>& newTrans){
    node.Transition.push_back(newTrans);
    size_t i = node.Transition.size() - 1;
    char cmp = Text[node.Transition[i - 1].first.first];
    char added = Text[node.Transition[i].first.first];
    for (; cmp > added && i > 0; i--){
        std::pair<std::pair<size_t, TRB>, TNode*> temp = node.Transition[i];
        node.Transition[i] = node.Transition[i - 1];
        node.Transition[i - 1] = temp;
        if (i == 1){
            break;
        }
        cmp = Text[node.Transition[i - 2].first.first];
    }
}

size_t TSTree::GetLcp(size_t ind1, size_t ind2){
    if (ind2 < ind1){
        return GetLcp(ind2, ind1);
    }
    if (ind1 == ind2){
        return -1;
    }
    size_t result = Lcp[ind1];
    for (size_t i = ind1 + 1; i < ind2; i++){
        if (Lcp[i] < result){
            result = Lcp[i];
        }
    }
    return result;
}

void TSTree::InDpth(size_t& pos, size_t& k, TNode* node, std::vector<size_t>& sArr, size_t* prevStrDpth, size_t strDpth, bool first){
    if (node->SufInd == 0){
        for (size_t i = 0; i < node->Transition.size(); i++){
            size_t newStrDpth;
            if (node->Transition[i].second->SufInd == 0){
                size_t sec = node->Transition[i].first.second.Inner;
                newStrDpth = sec - node->Transition[i].first.first + 1;
            }
            else {
                newStrDpth = 0;
            }
            InDpth(pos, k, node->Transition[i].second, sArr, prevStrDpth, strDpth + newStrDpth, first);
            *prevStrDpth = strDpth;
            if (first){
                first = false;
            }
        }
    }
    else {
        sArr[pos++] = node->SufInd;
        if (pos != 1){
            if (first){
                Lcp[k++] = 0;
            }
            else {
                if (*prevStrDpth < strDpth){
                    Lcp[k++] = *prevStrDpth;
                }
                else {
                    Lcp[k++] = strDpth;
                }
            }
        }
    }
}

TSTree::TSTree(){
    Root = new TSTree::TNode();
    Root->SufInd = 0;
    Root->Link = Root;
    fin = 0;
}

TSTree::TSTree(std::string& text){
    Text = text;
    Root = new TSTree::TNode();
    Root->SufInd = 0;
    Root->Link = Root;
    TNode* firstL = new TNode();
    firstL->SufInd = 1;
    fin = 0;
//    size_t& curEnd = fin;
    TRB rightB;
    rightB.Leaf = &fin;
    std::pair<size_t, TRB> firstInd(0,rightB);
    std::pair<std::pair<size_t, TRB>, TNode*> first(firstInd,firstL);
    Root->Transition.push_back(first);
    Root->Transition[0].second->Link = Root;
    size_t stState = 0;
    for (size_t i = 1; i < Text.length(); i++){
        stState = Append(i, stState);
        fin++;
    }
    Text += Terminator;
    Append(Text.length() - 1, 0);
    Text.pop_back();
}

void TSTree::Array(std::vector<size_t>& sArr){
    Text += Terminator;
    if (sArr.size() < Text.length() - 1){
        sArr.resize(Text.length() - 1);
    }
    if (sArr.size() == 1){
        sArr[0] = 1;
        Text.pop_back();
        return;
    }
    Lcp.resize(sArr.size() - 1);
    size_t pos = 0;
    size_t k = 0;
    for (size_t i = 0; i < Root->Transition.size(); i++){
        if (Text[Root->Transition[i].first.first] == Terminator){
            continue;
        }
        size_t prevStrDpth = 0;
        size_t strDpth;
        if (Root->Transition[i].second->SufInd == 0){
            size_t sec = Root->Transition[i].first.second.Inner;
            strDpth = sec - Root->Transition[i].first.first + 1;
        }
        else {
            strDpth = 0;
        }
        InDpth(pos, k, Root->Transition[i].second, sArr, &prevStrDpth, strDpth, true);
    }
    Text.pop_back();
}

std::pair<int,int> TSTree::ArrSearch(std::vector<size_t>& sArr, std::string& pattern){
    std::pair<int,int> result(-1,-1);
    if (sArr.size() == 1){
        if (Text == pattern){
            result.first = 0;
            result.second = 0;
        }
        return result;
    }
    int lB = 0;
    int rB = sArr.size() - 1;
    int cmpInd = (rB - lB) / 2;
    size_t l = 0;
    size_t r = 0;
    size_t mlr = 0;
    while (lB < rB){
        if (l == r){
            int res = 0;
            size_t temp = strCmp(Text, pattern, sArr[cmpInd] - 1 + mlr, mlr, res);
            if (res == 1){
                l = temp;
                mlr = std::min(l, r);
                lB = cmpInd + 1;
                cmpInd = lB + (rB - lB) / 2;
            }
            else if (res == -1){
                r = temp;
                mlr = std::min(l, r);
                rB = cmpInd;
                cmpInd = lB + (rB - lB) / 2;
            }
            else {
                if (result.second == -1){
                    result.second = cmpInd;
                }
                result.first = cmpInd;
                r = temp;
                mlr = std::min(l, r);
                rB = cmpInd;
                cmpInd = lB + (rB - lB) / 2;
            }
        }
        else {
            size_t lLcp = GetLcp(lB, cmpInd);
            if (lLcp == (size_t)-1){
                lLcp = sArr.size() - sArr[cmpInd];
            }
            size_t rLcp = GetLcp(cmpInd, rB);
            if (rLcp == (size_t)-1){
                rLcp = sArr.size() - sArr[cmpInd];
            }
            if (l > r){
                if (lLcp > l){
                    lB = cmpInd + 1;
                    cmpInd = lB + (rB - lB) / 2;
                }
                else if (lLcp < l){
                    r = lLcp;
                    rB = cmpInd;
                    mlr = std::min(l, r);
                    cmpInd = lB + (rB - lB) / 2;
                }
                else {
                    int res = 0;
                    size_t temp = strCmp(Text, pattern, sArr[cmpInd] - 1 + mlr, mlr, res);
                    if (res == 1){
                        l = temp;
                        mlr = std::min(l, r);
                        lB = cmpInd + 1;
                        cmpInd = lB + (rB - lB) / 2;
                    }
                    else if (res == -1){
                        r = temp;
                        mlr = std::min(l, r);
                        rB = cmpInd;
                        cmpInd = lB + (rB - lB) / 2;
                    }
                    else {
                        if (result.second == -1){
                            result.second = cmpInd;
                        }
                        result.first = cmpInd;
                        r = temp;
                        mlr = std::min(l, r);
                        rB = cmpInd;
                        cmpInd = lB + (rB - lB) / 2;
                    }
                }
            }
            else {
                if (rLcp > r){
                    rB = cmpInd;
                    cmpInd = lB + (rB - lB) / 2;
                }
                else if (rLcp < r){
                    l = rLcp;
                    lB = cmpInd + 1;
                    mlr = std::min(l, r);
                    cmpInd = lB + (rB - lB) / 2;
                }
                else {
                    int res = 0;
                    size_t temp = strCmp(Text, pattern, sArr[cmpInd] - 1 + mlr, mlr, res);
                    if (res == 1){
                        l = temp;
                        mlr = std::min(l, r);
                        lB = cmpInd + 1;
                        cmpInd = lB + (rB - lB) / 2;
                    }
                    else if (res == -1){
                        r = temp;
                        mlr = std::min(l, r);
                        rB = cmpInd;
                        cmpInd = lB + (rB - lB) / 2;
                    }
                    else {
                        if (result.second == -1){
                            result.second = cmpInd;
                        }
                        result.first = cmpInd;
                        r = temp;
                        mlr = std::min(l, r);
                        rB = cmpInd;
                        cmpInd = lB + (rB - lB) / 2;
                    }
                }
            }
        }
    }
    int res = 0;
    size_t temp = strCmp(Text, pattern, sArr[cmpInd] - 1 + mlr, mlr, res);
    if (res == 0){
        if (result.second == -1){
            result.second = cmpInd;
        }
        result.first = cmpInd;
        r = temp;
        mlr = std::min(l, r);
        rB = cmpInd;
        cmpInd = lB + (rB - lB) / 2;
    }
    if (result.second != -1){
        cmpInd = result.second;
        res = 0;
        while ((size_t)cmpInd < sArr.size() - 1){
            strCmp(Text, pattern, sArr[cmpInd + 1] - 1, 0, res);
            if (res == 0){
                result.second++;
                cmpInd++;
                continue;
            }
            break;
        }
    }
    return result;
}

TSTree::~TSTree(){
    delete Root;
}
