#include <bits/stdc++.h>

#define endl '\n'


struct Node {
    int val;
    Node* left;
    Node* right;
    Node* parent;

    Node(int key) {
        val = key;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

class BST{
private:
    Node* root;
    long long search_visited, insert_visited, delete_visited;

    void IOW(Node* node){
        if (node != nullptr){
            IOW(node->left);
            std::cout << node->val << " ";
            IOW(node->right);
        }
    }

    void Swap(Node* u, Node* v){
        if (u->parent == nullptr){
            root = v;
        } else if (u == u->parent->left){
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        if (v != nullptr)
            v->parent = u->parent;
    }
public:
    BST (){
        root = nullptr;
        search_visited = 0;
        insert_visited = 0;
        delete_visited = 0;
    }

    void Insert(int val){
        insert_visited = 0;
        if (root == nullptr){
            root = new Node(val);
            ++insert_visited;
            return;
        }

        Node* cur = root;

        while (cur != nullptr){
            ++insert_visited;
            if (val < cur->val){

                if (cur->left == nullptr){
                    Node* node = new Node(val);
                    node->parent = cur;
                    cur->left = node;
                    break;
                } else {
                    cur = cur->left;
                }
            }
            if (val > cur->val){
                if (cur->right == nullptr){
                    Node* node = new Node(val);
                    node->parent = cur;
                    cur->right = node;
                    break;
                } else {
                    cur = cur->right;
                }
            }
            if (val == cur->val){
                return;
            }

        }


    }

    Node* Search(int val){
        search_visited = 0;
        Node* cur = root;

        while (cur != nullptr){
            ++search_visited;
            if (val == cur->val)
                return cur;

            if (val < cur->val)
                cur = cur->left;
            else
                cur = cur->right;
        }
        return nullptr;

    }

    Node* Predecessor(Node* cur){
        if (cur == nullptr) 
            return nullptr;
        if (cur->left != nullptr){
            cur = cur->left;
            while (cur->right != nullptr)
                cur = cur->right;
            return cur;
        }

        Node* p = cur->parent;
        while (p != nullptr && cur == p->left){
            cur = p;
            p = p->parent;
        }

        return p;
        
    }

    Node* Successor(Node* cur){
        if (cur->right != nullptr){
            cur = cur->right;

            while (cur->left != nullptr){
                ++delete_visited;
                cur = cur->left;
            }

            return cur;
        }

        Node* p = cur->parent;

        while (p != nullptr && cur == p->right){
            ++delete_visited;
            cur = p;
            p = p->parent;
        }

        return p;
    }

    void InorderWalk(){
        IOW(root);
    }

    void Delete(int val){
        Node* cur = Search(val);

        delete_visited = search_visited;

        if (cur == nullptr)
            return;
        
        if (cur->left == nullptr){
            ++delete_visited;
            Swap(cur, cur->right);
        } else if (cur->right == nullptr){
            ++delete_visited;
            Swap(cur, cur->left);
        } else {
            ++delete_visited;
            Node* node = Successor(cur);
            
            if (node->parent != cur){
                Swap(node, node->right);
                node->right = cur->right;
                if (node->right != nullptr)
                    node->right->parent = node;
            }

            Swap(cur, node);
            node->left = cur->left;
            node->left->parent = node;
            
        }
        
        

        delete cur;
        
    }

    Node* GetRoot(){
        return root;
    }

    int Height(Node* node){
        if (node == nullptr)
            return 0;

        return 1 + std::max(Height(node->left), Height(node->right));
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

};
