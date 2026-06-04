#include <bits/stdc++.h>
#include <optional>

#define endl '\n'

class Heap {
private:
    std::vector<int> heap;
public:
    Heap(){};

    void Insert(int val){
        heap.push_back(val);
        int ind = heap.size()-1;
        while (ind > 0){
            int par_ind = (ind-1)/2;
            if (heap[par_ind] > heap[ind]){
                std::swap(heap[par_ind], heap[ind]);
                ind = par_ind;
            } else break;
        }
    }

    std::optional<int> ExtractMin(){
        if (!heap.size()) 
            return std::nullopt;
        int ans = heap[0];
        std::swap(heap[0], heap[heap.size()-1]);
        heap.pop_back();
        int ind = 0, n = heap.size();
        while (ind < n){
            int smallest_ind = ind, l = ind*2+1, r = ind*2+2;
            if (l < n && heap[smallest_ind] > heap[l]) 
                smallest_ind = l;
            if (r < n && heap[smallest_ind] > heap[r]) 
                smallest_ind = r;
            if (smallest_ind == ind) 
                break;
            std::swap(heap[ind], heap[smallest_ind]);
            ind = smallest_ind;
        }
        return ans;
    }
};

class FibonacciHeap {
private:
    struct Node {
        int k, degree;
        Node* par;
        Node* ch;
        Node* l;
        Node* r;

        Node(int key) : k(key), degree(0), par(nullptr), ch(nullptr), l(this), r(this) {}
    };

    Node* mn;

    Node* MergeTrees(Node* x, Node* y){
        if (x->k > y->k) 
            std::swap(x, y);
        if (!x->ch){
            x->ch = y;
            y->l = y;
            y->r = y;
        } else {

            Node* ch = x->ch;
            y->r = ch;
            y->l = ch->l;
            ch->l->r = y;
            ch->l = y;
        }

        y->par = x;
        x->degree++;
        return x;
    }

    void Consolidate(){
        std::vector<Node*> roots;
        Node* cur = mn;
        do {
            roots.push_back(cur);
            cur = cur->r;
        } while (cur != mn);

        for (Node* n : roots) { 
            n->l = n; 
            n->r = n; 
        }
        std::vector<Node*> deg(30, nullptr);
        Node* mn_node = nullptr;

        for (Node* x : roots){

            int d = x->degree;
            while (deg[d]){
                x = MergeTrees(x, deg[d]);
                deg[d++] = nullptr;
            }
            deg[d] = x;
            if (!mn_node || x->k < mn_node->k) 
                mn_node = x;
        }

        mn = nullptr;
        for (Node* x : deg){
            if (!x) 
                continue;
            x->l = x; 
            x->r = x;
            if (!mn){
                mn = x;
            } else {
                x->r = mn->r;
                x->l = mn;
                mn->r->l = x;
                mn->r = x;
            }
        }
        mn = mn_node;
    }

public:
    FibonacciHeap() : mn(nullptr) {}
    FibonacciHeap(Node* ptr) : mn(ptr) {}

    void Insert(int key){
        Node* node = new Node(key);
        if (!mn){ 
            mn = node; 
            return; 
        }

        node->r = mn->r;
        node->l = mn;
        mn->r->l = node;
        mn->r = node;

        if (key < mn->k) 
            mn = node;


    }

    static FibonacciHeap Union(FibonacciHeap&& h1, FibonacciHeap&& h2){
        if (!h1.mn){ 
            FibonacciHeap t(h2.mn); 
            h2.mn = nullptr; 
            return t; 
        }
        if (!h2.mn){ 
            FibonacciHeap t(h1.mn); 
            h1.mn = nullptr; 
            return t; 
        }

        Node* mn1 = h1.mn, *mn2 = h2.mn;
        Node* r = mn1->r, *l = mn2->l;
        mn1->r = mn2;
        mn2->l = mn1;
        l->r = r;     
        r->l = l;

        h1.mn = h2.mn = nullptr;
        return FibonacciHeap(mn1->k < mn2->k ? mn1 : mn2);


    }

    std::optional<int> ExtractMin(){
        if (!mn) 
            return std::nullopt;

        int ans = mn->k;

        if (mn->ch){

            std::vector<Node*> chs;
            Node* ch = mn->ch;
            do { 
                chs.push_back(ch); 
                ch = ch->r; 
            } while (ch != mn->ch);

            for (Node* child : chs){
                child->par = nullptr;
                child->r = mn->r;
                child->l = mn;
                mn->r->l = child;
                mn->r = child;
            }
            mn->ch = nullptr;
        }

        Node* tmp = mn;
        if (mn->r == mn){          
            mn = nullptr;
            delete tmp;
            return ans;
        }
        mn->l->r = mn->r;
        mn->r->l = mn->l;
        mn = mn->r;
        delete tmp;

        Consolidate();

        return ans;
    }
};
