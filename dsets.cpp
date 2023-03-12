/* Your code here! */
#include "dsets.h"

void DisjointSets::addelements(int num) {
    for (int i = 0; i < num; i++)
        data_.push_back(-1);
}

int DisjointSets::find(int elem) {
    if (data_[elem] < 0)
        return elem;
    else
        data_[elem] = find(data_[elem]);
    return (data_[elem]);
}

void DisjointSets::setunion(int a, int b) {
    int rootA, rootB, newSize;

    while (data_[a] >= 0)
        a = data_[a];
         
    while (data_[b] >= 0)
        b = data_[b];
    
    rootA = a;
    rootB = b;
    newSize = data_[rootA] + data_[rootB];

    if (rootA != rootB) {
        if (data_[rootA] <= data_[rootB]) {
            data_[rootB] = a;
            data_[rootA] = newSize;
        }
        else {
            data_[rootA] = b;
            data_[rootB] = newSize;
        }
    }
}

int DisjointSets::size(int elem) {
    int root = find(elem);
	return data_[root] * -1;
}