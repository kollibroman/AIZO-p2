#ifndef SORTING_H
#define SORTING_H

#include "../Utils/DefinitelyNotADataStructures/DefinitelyNotAVector.h"
#include "../Utils/DefinitelyNotADataStructures/DefinitelyNotATuple.h"

class Sorting
{
public:
     static void QuickSort(DefinitelyNotAVector<Edge>& edges,
                         int low, int high) {
        if (low < high) {
            int pi = partition(edges, low, high);
            QuickSort(edges, low, pi - 1);
            QuickSort(edges, pi + 1, high);
        }
    }

    static int partition(DefinitelyNotAVector<Edge>& edges,
                        int low, int high)
    {
        auto pivot = edges[high].weight;
        int i = low - 1;

        for (int j = low; j < high; j++)
        {
            if (edges[j].weight <= pivot)
            {
                i++;
                MySwap(edges[i], edges[j]);
            }
        }
        MySwap(edges[i + 1], edges[high]);
        return i + 1;
    }

    static void MySwap(Edge& a,
                    Edge& b)
    {
        const Edge temp = a;
        a = b;
        b = temp;
    }
};

#endif //SORTING_H
