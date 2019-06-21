#ifndef NODE_H
#define NODE_H

#include <vector>

using namespace std;

template<typename T> class BTree;

template <typename T>
class Node {
    typedef Node<T> node;

    unsigned int capacity;
    unsigned int size;
    vector<unsigned int> keys;
    vector<Node<T>*> children;
    bool isLeaf;

    public: 
        Node(unsigned int capacity, bool isLeaf = true) : capacity(capacity), isLeaf(isLeaf) {
            keys.resize(capacity - 1);
            children.resize(capacity);
            size = 0;
        }

        bool search(int k){
            int i = 0;
            while(i < size && k > keys[i])
                i++;

            if(keys[i] == k)
                return true;

            if(isLeaf)
                return false;

            return children[i]->search(k);
        }

        void insert(unsigned int k, T data){
            int idx = size;
            if(isLeaf){
                while(idx > 0 && k < keys[idx - 1]){
                    keys[idx] = keys[idx - 1];
                    idx--;
                }

                keys[idx] = k;
                size++;
            }
            else{
                while(idx > 0 && k < keys[idx - 1]){
                    idx--;
                }

                if(children[idx]->size == capacity - 1){
                    splitChild(idx, children[idx]);
                    if(k > keys[idx]){
                        idx++;
                    }
                }

                children[idx]->insert(k, data);
            }
        }

        void splitChild(int idx, node* toSplit){
            node* newNode = new node(capacity, toSplit->isLeaf);
            int m = (capacity - 1)/2;

            newNode->size = m - (capacity%2);
            
            for(int i = 0; i < newNode->size; i++){
                newNode->keys[i] = toSplit->keys[m + 1 + i];
            }

            if(toSplit->isLeaf == false){
                for(int i = 0; i <= newNode->size; i++){
                    newNode->children[i] = toSplit->children[m + 1 + i];
                }
            }

            toSplit->size = m;

            for(int i = size + 1; i > idx + 1; i--){
                children[i] = children[i-1];
            }

            children[idx + 1] = newNode;

            for(int i = size; i > idx; i--){
                keys[i] = keys[i-1];
            }

            keys[idx] = toSplit->keys[m];
            size++;
        }

        void remove(int k){
            int idx = 0;
            while(idx < size && k > keys[idx]){
                idx++;
            }

            if(idx < size && keys[idx] == k){
                if(isLeaf){
                    for(int i = idx; i < size - 1; i++){
                        keys[i] = keys[i + 1];
                    }
                    size--;
                }
                else{
                    if(children[idx]->size >= (capacity + 1)/2){
                        int newKey = getPreviousKey(idx);
                        keys[idx] = newKey;
                        children[idx]->remove(newKey);
                    }
                    else if(children[idx + 1]->size >= (capacity + 1)/2){
                        int newKey = getNextKey(idx);
                        keys[idx] = newKey;
                        children[idx + 1]->remove(newKey);
                    }
                    else{
                        mergeNodes(idx);
                        children[idx]->remove(k);
                    }
                }

            }
            else{
                if(children[idx]->size < (capacity + 1)/2){
                   fillNode(idx);
                }

                if(idx > size){
                   children[idx - 1]->remove(k);
                }
                else{
                   children[idx]->remove(k);
                }
            }
        }

        int getPreviousKey(int idx){
            node* current = children[idx];

            while(current->isLeaf == false){
                current = current->children[0];
            }

            return current->keys[0];
        }

        int getNextKey(int idx){
            node* current = children[idx + 1];

            while(current->isLeaf == false){
                current = current->children[current->size];
            }

            return current->keys[current->size - 1];
        }

        void fillNode(int idx){
            int m = (capacity - 1)/2;

            if(idx && children[idx - 1]->size > m){
                borrowPrev(idx);
            }

            else if(idx != size && children[idx + 1]->size > m){
                borrowNext(idx);
            }

            else{
                if(idx == size){
                    mergeNodes(idx - 1);
                }
                else{
                    mergeNodes(idx);
                }
            }
        }

        void borrowPrev(int idx){
            int m = (capacity - 1)/2;

            node* current = children[idx];
            node* toBorrow = children[idx - 1];

            for(int i = current->size; i > 0; i--){
                current->keys[i] = current->keys[i - 1];
            }

            if(current->isLeaf == false){
                for(int i = current->size + 1; i > 0; i--){
                    current->children[i] = current->children[i - 1];
                }
            }

            current->keys[0] = keys[idx - 1];
            
            if(toBorrow->isLeaf == false){
                current->children[0] = toBorrow->children[toBorrow->size];
            }

            keys[idx - 1] = toBorrow->keys[toBorrow->size - 1];

            current->size++;
            toBorrow->size--;
        }
            
        void borrowNext(int idx){
            int m = (capacity - 1)/2;

            node* current = children[idx];
            node* toBorrow = children[idx + 1];

            current->keys[current->size] = keys[idx];
            
            if(toBorrow->isLeaf == false){
                current->children[current->size + 1] = toBorrow->children[0];
            }

            keys[idx] = toBorrow->keys[0];

            for(int i = 1; i < toBorrow->size; i++){
                toBorrow->keys[i - 1] = toBorrow->keys[i];
            }

            if(current->isLeaf == false){
                for(int i = 1; i < toBorrow->size + 1; i++){
                    toBorrow->children[i - 1] = toBorrow->children[i];
                }
            }

            current->size++;
            toBorrow->size--;
        }

        void mergeNodes(int idx){
            int m = (capacity - 1)/2;

            node* current = children[idx];
            node* toMerge = children[idx + 1];

            current->keys[m] = keys[idx];

            for(int i = 0; i < toMerge->size; i++){
                current->keys[m + 1 + i] = toMerge->keys[i];
            }

            if(toMerge->isLeaf == false){
                for(int i = 0; i <= toMerge->size; i++){
                    current->children[m + 1 + i] = toMerge->children[i];
                }
            }

            for(int i = idx; i < size - 1; i++){
                keys[i] = keys[i + 1];
            }

            for(int i = idx + 1; i < size; i++){
                children[i] = children[i + 1];
            }

            current->size += 1 + toMerge->size;
            size--;
            delete toMerge;
        }

        void print(){
            if(isLeaf){
                for(int i = 0; i < size; i++){
                    cout<<keys[i]<<" ";
                }
            }
            else{
                for(int i = 0; i < size; i++){
                    children[i]->print();
                    cout<<keys[i]<<" ";
                }
                children[size]->print();
            }
        }

    friend class BTree<T>; 
};

#endif
