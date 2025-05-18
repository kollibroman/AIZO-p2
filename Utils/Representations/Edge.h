#ifndef EDGE_H
#define EDGE_H

struct Edge {
    int source;
    int destination;
    int weight;

    Edge(int s, int d, int w) : source(s), destination(d), weight(w) {}

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};


#endif //EDGE_H
