#include <bits/stdc++.h>

#include "btree.h"

using namespace std;

int main(int argc, char *argv[]) {
    BTree<int> btree(6);

    for(int i = 0; i < 50; i++)
        btree.insert(i, i);

    btree.print();

    for(int i = 0; i < 50; i++)
        btree.remove(i), btree.print();

    return EXIT_SUCCESS;
}
