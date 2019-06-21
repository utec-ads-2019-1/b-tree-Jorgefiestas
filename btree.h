#ifndef BTREE_H
#define BTREE_H

#include "node.h"

template <typename T>
class BTree {
    private:
        typedef Node<T> node;

        node* root;
        unsigned int degree;

    public:
        BTree(unsigned int degree) : degree(degree), root(nullptr) {};

        T search(int k) { 
            if(root)
                return root->search(k);
            return false;
        } 

        bool insert(int k, T data) {
            if(search(k))
                return false;

            if(!root){
                root = new node(degree);
                root->keys[0] = k;
                root->size = 1;
            }

            else if(root->size == degree - 1){
                node *newRoot = new node(degree, false);
                newRoot->children[0] = root;
                newRoot->splitChild(0, root);

                if(k < newRoot->keys[0]){
                    newRoot->children[0]->insert(k, data);
                }
                else{
                    newRoot->children[1]->insert(k, data);
                }

                root = newRoot;
            }
            else{
                root->insert(k, data);
            }

            return true;
        }

        bool remove(int k) {
            if(!search(k)){
                return false;
            }

            root->remove(k);

            if(root->size == 0){
                node* oldRoot = root;
                if(root->isLeaf){
                    root = nullptr;
                }
                else{
                    root = root->children[0];
                }
                delete oldRoot;
            }

            return true;
        }

        void print() {
            if(root){
                root->print();
            }
            cout<<endl;
        }

        ~BTree(){};
};

#endif
