#include <bits/stdc++.h>

#define endl '\n'


struct Node {
    int val;
    int height;
    Node* left;
    Node* right;

    Node(int key) {
        val = key;
        left = nullptr;
        right = nullptr;
        height = 0;
    }
};

class AVL{
private:
    Node* root;
    long long search_visited, insert_visited, delete_visited, cnt_rotate;

    int getBalance(Node* node){
        return node ? Height(node->left)-Height(node->right) : 0;
    }

    Node* RightRotate(Node* node){
        ++cnt_rotate;
        Node* l = node->left;
        node->left = l->right;
        l->right = node;

        node->height = 1 + std::max(Height(node->left), Height(node->right));
        l->height = 1 + std::max(Height(l->left), Height(l->right));

        return l;
    }

    Node* LeftRotate(Node* node){
        ++cnt_rotate;
        Node* r = node->right;
        node->right = r->left;
        r->left = node;
        
        node->height = 1 + std::max(Height(node->left), Height(node->right));
        r->height = 1 + std::max(Height(r->left), Height(r->right));

        return r;
    }

    Node* Balance(Node* node){
        node->height = 1 + std::max(Height(node->left), Height(node->right));

        int bn = getBalance(node);

        if (bn < -1){
            int br = getBalance(node->right);
            if (br > 0)
                node->right = RightRotate(node->right);
            node = LeftRotate(node);
            
        } else if (bn > 1) {
            int bl = getBalance(node->left);

            if (bl < 0)
                node->left = LeftRotate(node->left);
            node = RightRotate(node);
        }

        return node;
    }

    Node* SearchNode(Node* node, int val){
        if (!node)
            return nullptr;
        ++search_visited;
        if (val < node->val){
            return SearchNode(node->left, val);
        } else if (val > node->val){
            return SearchNode(node->right, val);
        } else {
            return node;
        }
    }

    Node* InsertNode(Node* node, int val){
        ++insert_visited;
        if (!node)
            return new Node(val);
        
        if (val < node->val)
            node->left = InsertNode(node->left, val);
        else if (val > node->val)
            node->right = InsertNode(node->right, val);
        else
            return node;

        node->height = 1 + std::max(Height(node->left), Height(node->right));

        return Balance(node);
    }

    Node* DeleteNode(Node* node, int val){
        ++delete_visited;
        if (!node)
            return node;

        if (val < node->val){
            node->left = DeleteNode(node->left, val);
        } else if (val > node->val) {
            node->right = DeleteNode(node->right, val);
        } else {
            if (node->left && node->right){
                Node* repl = node->right;
                while (repl->left)
                    repl = repl->left;
                node->val = repl->val;
                node->right = DeleteNode(node->right, repl->val);
                return node;
            } else if (node->left){
                Node* tmp = node->left;
                delete node;
                return tmp;
            } else if (node->right){
                Node* tmp = node->right;
                delete node;
                return tmp;
            } else {
                delete node;
                return nullptr;
            }
        }

        node->height = 1 + std::max(Height(node->left), Height(node->right));

        return Balance(node);
    }

public:
    AVL (){
        root = nullptr;
        search_visited = 0;
        insert_visited = 0;
        delete_visited = 0;
        cnt_rotate = 0;

    }

    Node* Search(int val){
        return SearchNode(root, val);
    }

    void Insert(int val){
        root = InsertNode(root, val);
    }

    void Delete(int val){
        root = DeleteNode(root, val);
    }

    Node* Successor(int val){
        Node* node = Search(val);
        if (!node) return nullptr;

        if (node->right){
            Node* cur = node->right;

            while (cur->left){
                cur = cur->left;
            }
            return cur;
            
        }

        Node* cur = root;
        Node* succ = nullptr;

        while (cur){
            if (node->val < cur->val){
                succ = cur;
                cur = cur->left;
            } else if (node->val > cur->val){
                cur = cur->right;
            } else {
                break;
            }
        }
        return succ;

    }

    Node* Predcessor(int val){
        Node* node = Search(val);
        if (!node) return nullptr;

        if (node->left){
            Node* cur = node->left;
            while (cur->right){
                cur = cur->right;
            }

            return cur;
        }

        Node* pred = nullptr;
        Node* cur = root;

        while (cur){
            if (node->val < cur->val){
                cur = cur->left;
            } else if (node->val > cur->val){
                pred = cur;
                cur = cur->right;
            } else {
                break;
            }
        }
        return pred;
            
    }


    void InOrderWalk(Node* node){
        if (node != nullptr){
            InOrderWalk(node->left);
            std::cout << node->val << " ";
            InOrderWalk(node->right);
        }
    }

    Node* GetRoot(){
        return root;
    }

    int Height(Node* node){
        return node ? node->height : 0;
    }

    void NullCnts(){
        search_visited = 0;
        insert_visited = 0;
        delete_visited = 0;
        cnt_rotate = 0;
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
    long long GetRotateCnt(){
        return cnt_rotate;
    }

};
