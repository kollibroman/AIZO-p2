#ifndef UNIONFIND_H
#define UNIONFIND_H

#include "DefinitelyNotAVector.h"

class UnionFind {
private:
    DefinitelyNotAVector<int> parent;
    DefinitelyNotAVector<int> rank;
    int count; // Number of disjoint sets

public:
    explicit UnionFind(int size) : parent(size), rank(size, 0), count(size) {
        // Initialize each element as its own set
        for (int i = 0; i < size; i++) {
            parent[i] = i;
        }
    }

    // Find set of vertex i with path compression
    int find(int i) {
        if (parent[i] != i) {
            parent[i] = find(parent[i]); // Path compression
        }
        return parent[i];
    }

    // Union by rank
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);

        if (root_i != root_j) {
            // Attach smaller rank tree under root of higher rank tree
            if (rank[root_i] < rank[root_j]) {
                parent[root_i] = root_j;
            } else if (rank[root_i] > rank[root_j]) {
                parent[root_j] = root_i;
            } else {
                // If ranks are same, make one as root and increment its rank
                parent[root_j] = root_i;
                rank[root_i]++;
            }
            count--; // Decrease count of disjoint sets
        }
    }

    // Check if two elements are in same set
    bool connected(int i, int j) {
        return find(i) == find(j);
    }

    // Get number of disjoint sets
    int getCount() const {
        return count;
    }
};


#endif //UNIONFIND_H
