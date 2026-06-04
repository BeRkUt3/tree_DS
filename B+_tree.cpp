#include <bits/stdc++.h>
#include <optional>

#define endl '\n'

struct Data {
    char bytes[76];
};

struct Node {
    int n;
    std::vector<unsigned int> keys;
    std::vector<Data> data;
    std::vector<Node*> p; 
    bool is_leaf;
    Node* prev;
    Node* next;

    Node(int order, bool isleaf){
        n = 0;
        is_leaf = isleaf;
        keys.resize(order-1);
        prev = nullptr;
        next = nullptr;
        if (is_leaf){
            data.resize(order-1);
        } else {
            p.resize(order);
        }
    }

};


class BPlus{
private:
    const static int m_leaf = 204, m = 2048;
    Node* root;

    long long search_visited = 0, insert_visited = 0, delete_visited = 0, disk_w = 0, disk_r = 0;


public:
    BPlus(){
        root = new Node(m_leaf, true);
    }
    std::optional<Data> Search(unsigned int k){
        if (!root)
            return std::nullopt;
        search_visited++;
        Node* cur = root;
        while (!cur->is_leaf){
            int ind = 0;
            while (ind < cur->n && k >= cur->keys[ind])
                ++ind;
            cur = cur->p[ind];
            search_visited++;
            disk_r++;
        }
        for (int i = 0; i < cur->n; ++i){
            if (cur->keys[i] == k)
                return cur->data[i];
        }
        return std::nullopt;
    }

    void Insert(unsigned int k, Data d){
        insert_visited++;
        Node* prev = nullptr;
        Node* cur = root;

        while (!cur->is_leaf){
            insert_visited++;
            if (cur->n == m-1){
                int mid = cur->n / 2;
                unsigned int k_mid = cur->keys[mid];
                Node* l = new Node(m, false);
                Node* r = new Node(m, false);

                l->n = mid;
                for (int i = 0; i < mid; ++i){
                    l->keys[i] = cur->keys[i];
                    l->p[i] = cur->p[i];
                }
                l->p[mid] = cur->p[mid];

                r->n = cur->n - mid - 1;
                for (int i = mid+1; i < cur->n; ++i){
                    r->keys[i-mid - 1] = cur->keys[i];
                    r->p[i-mid - 1] = cur->p[i];
                }
                r->p[r->n] = cur->p[cur->n];

                if (!prev){
                    root = new Node(m, false);
                    root->keys[0] = k_mid;
                    root->p[0] = l;
                    root->p[1] = r;
                    root->n = 1;
                } else {
                    int ind = 0;
                    while (ind < prev->n && k_mid >= prev->keys[ind])
                        ++ind;
                    for (int i = prev->n; i > ind; --i)
                        prev->keys[i] = prev->keys[i-1];
                    prev->keys[ind] = k_mid;
                    for (int i = prev->n; i > ind; --i)
                        prev->p[i+1] = prev->p[i];
                    prev->p[ind] = l;
                    prev->p[ind+1] = r;
                    prev->n++;
                }

                disk_w += 3;
                int ind = 0;
                while (ind < cur->n && k >= cur->keys[ind])
                    ++ind;
                cur = cur->p[ind];
                prev = (ind <= mid) ? l : r;
            } else {
                int ind = 0;
                while (ind < cur->n && k >= cur->keys[ind])
                    ++ind;
                prev = cur;
                cur = cur->p[ind];
                
            }
            disk_r++;
        }
        insert_visited++;
        if (cur->n == m_leaf-1){
            int mid = cur->n / 2;
            Node* l = new Node(m_leaf, true);
            Node* r = new Node(m_leaf, true);

            l->n = mid;
            for (int i = 0; i < mid; ++i){
                l->keys[i] = cur->keys[i];
                l->data[i] = cur->data[i];
            }

            r->n = cur->n - mid;
            for (int i = mid; i < cur->n; ++i){
                r->keys[i - mid] = cur->keys[i];
                r->data[i - mid] = cur->data[i];
            }

      
            l->prev = cur->prev;
            l->next = r;
            r->prev = l;
            r->next = cur->next;
            if (cur->prev) cur->prev->next = l;
            if (cur->next) cur->next->prev = r;

            unsigned int key = r->keys[0]; 

            if (!prev){
                root = new Node(m, false);
                root->keys[0] = key;
                root->p[0] = l;
                root->p[1] = r;
                root->n = 1;
            } else {
                int ind = 0;
                while (ind < prev->n && key >= prev->keys[ind])
                    ++ind;
                for (int i = prev->n; i > ind; --i)
                    prev->keys[i] = prev->keys[i-1];
                prev->keys[ind] = key;
                for (int i = prev->n; i > ind; --i)
                    prev->p[i+1] = prev->p[i];
                prev->p[ind] = l;
                prev->p[ind+1] = r;
                prev->n++;
            }
            disk_w += 3;
            int ind = 0;
            while (ind < cur->n && k >= cur->keys[ind])
                ++ind;
            if (ind < mid){
                for (int i = l->n; i > ind; --i){
                    l->keys[i] = l->keys[i-1];
                    l->data[i] = l->data[i-1];
                }
                l->keys[ind] = k;
                l->data[ind] = d;
                l->n++;
            } else {
                ind -= mid;
                for (int i = r->n; i > ind; --i){
                    r->keys[i] = r->keys[i-1];
                    r->data[i] = r->data[i-1];
                }
                r->keys[ind] = k;
                r->data[ind] = d;
                r->n++;
            }
            disk_r++;

        } else {
            int ind = 0;
            while (ind < cur->n && k >= cur->keys[ind])
                ++ind;
            for (int i = cur->n; i > ind; --i){
                cur->keys[i] = cur->keys[i-1];
                cur->data[i] = cur->data[i-1];
            }
            cur->keys[ind] = k;
            cur->data[ind] = d;
            cur->n++;
            disk_w++;
        }
    }

    void Delete(unsigned int k){
        if (!root)
            return;
        const int mn = (m + 1) / 2 - 1;         
        const int mn_leaf = (m_leaf + 1) / 2 - 1; 
        Node* prev = nullptr;
        Node* cur  = root;
        delete_visited++;
        while (!cur->is_leaf){
            delete_visited++;
            int ind = 0;
            while (ind < cur->n && k >= cur->keys[ind])
                ++ind;

            Node* ch = cur->p[ind];
            int cmn = ch->is_leaf ? mn_leaf : mn;
            Node* next = ch;
            disk_r++;
            if (ch->n == cmn){
                bool bor = false;

                if (ind > 0 && cur->p[ind-1]->n > cmn){
                    Node* l = cur->p[ind-1];
                    if (ch->is_leaf){
                        for (int i = ch->n; i > 0; --i){
                            ch->keys[i] = ch->keys[i-1];
                            ch->data[i] = ch->data[i-1];
                        }
                        ch->keys[0] = l->keys[l->n-1];
                        ch->data[0] = l->data[l->n-1];
                        l->n--;
                        ch->n++;
                        cur->keys[ind-1] = ch->keys[0];
                    } else {
                        for (int i = ch->n; i > 0; --i)
                            ch->keys[i] = ch->keys[i-1];
                        for (int i = ch->n+1; i > 0; --i)
                            ch->p[i] = ch->p[i-1];
                        ch->keys[0] = cur->keys[ind-1];
                        ch->p[0] = l->p[l->n];
                        ch->n++;
                        cur->keys[ind-1] = l->keys[l->n-1];
                        l->n--;
                    }
                    disk_r++;
                    disk_w += 3;
                    bor = true;
                } else if (ind < cur->n && cur->p[ind+1]->n > cmn){
                    Node* r = cur->p[ind+1];
                    if (ch->is_leaf){
                        ch->keys[ch->n] = r->keys[0];
                        ch->data[ch->n] = r->data[0];
                        ch->n++;
                        for (int i = 0; i < r->n-1; ++i){
                            r->keys[i] = r->keys[i+1];
                            r->data[i] = r->data[i+1];
                        }
                        r->n--;
                        cur->keys[ind] = r->keys[0];
                    } else {
                        ch->keys[ch->n] = cur->keys[ind];
                        ch->p[ch->n+1] = r->p[0];
                        ch->n++;
                        cur->keys[ind] = r->keys[0];
                        for (int i = 0; i < r->n-1; ++i)
                            r->keys[i] = r->keys[i+1];
                        for (int i = 0; i < r->n; ++i)
                            r->p[i] = r->p[i+1];
                        r->n--;
                    }
                    disk_w += 3;
                    bor = true;
                }

                if (!bor){
                    if (ind > 0){
                        Node* l = cur->p[ind-1];
                        if (ch->is_leaf){
                            for (int i = 0; i < ch->n; ++i){
                                l->keys[l->n+i] = ch->keys[i];
                                l->data[l->n+i] = ch->data[i];
                            }
                            l->n += ch->n;
                            l->next = ch->next;
                            if (ch->next)
                                ch->next->prev = l;
                        } else {
                            l->keys[l->n] = cur->keys[ind-1];
                            l->p[l->n+1] = ch->p[0];
                            l->n++;
                            for (int i = 0; i < ch->n; ++i){
                                l->keys[l->n+i] = ch->keys[i];
                                l->p[l->n+i+1] = ch->p[i+1];
                            }
                            l->n += ch->n;
                        }
                        for (int i = ind-1; i < cur->n-1; ++i){
                            cur->keys[i] = cur->keys[i+1];
                            cur->p[i+1] = cur->p[i+2];
                        }
                        cur->n--;
                        disk_r++;
                        disk_w += 2;
                        delete ch;
                        next = l;
                    } else {
                        Node* r = cur->p[ind+1];
                        if (ch->is_leaf){
                            for (int i = 0; i < r->n; ++i){
                                ch->keys[ch->n+i] = r->keys[i];
                                ch->data[ch->n+i] = r->data[i];
                            }
                            ch->n += r->n;
                            ch->next = r->next;
                            if (r->next)
                                r->next->prev = ch;
                        } else {
                            ch->keys[ch->n] = cur->keys[ind];
                            ch->p[ch->n+1] = r->p[0];
                            ch->n++;
                            for (int i = 0; i < r->n; ++i){
                                ch->keys[ch->n+i] = r->keys[i];
                                ch->p[ch->n+i+1] = r->p[i+1];
                            }
                            ch->n += r->n;
                        }
                        for (int i = ind; i < cur->n-1; ++i){
                            cur->keys[i] = cur->keys[i+1];
                            cur->p[i+1] = cur->p[i+2];
                        }
                        disk_r++;
                        disk_w += 2;
                        cur->n--;
                        delete r;
                        next = ch;
                    }


                    if (cur == root && cur->n == 0){
                        disk_w++;
                        root = next;
                        delete cur;
                        cur  = root;
                        prev = nullptr;
                        continue;
                    }
                }

                prev = cur;
                cur  = next;
            } else {
                prev = cur;
                cur  = ch;
            }
        }

        delete_visited++;
        int ind = 0;
        while (ind < cur->n && cur->keys[ind] != k)
            ++ind;
        if (ind == cur->n)
            return;

        for (int i = ind; i < cur->n-1; ++i){
            cur->keys[i] = cur->keys[i+1];
            cur->data[i] = cur->data[i+1];
        }
        cur->n--;
        disk_w++;
    }
    void NullCnts(){
        search_visited = 0;
        insert_visited = 0;
        delete_visited = 0;
        disk_r = 0;
        disk_w = 0;
    }
    int Height(){
        int h = 1;
        Node* cur = root;
        while (!cur->is_leaf){
            ++h;
            cur = cur->p[0];
        }
        return h;
    }
    long long GetInsertCnt(){
        return insert_visited;
    }
    long long GetSearchCnt(){
        return search_visited;
    }
    long long GetDeleteCnt(){
        return delete_visited;
    }
    long long GetDskWCnt(){
        return disk_w;
    }
    long long GetDskRCnt(){
        return disk_r;
    }
};
