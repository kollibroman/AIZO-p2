#ifndef SORTING_H
#define SORTING_H
#include <algorithm>
#include "../Utils/DefinitelyNotADataStructures/DefinitelyNotAVector.h"
#include "../Utils/DefinitelyNotADataStructures/DefinitelyNotATuple.h"

class Sorting
{
public:
     static void QuickSort(DefinitelyNotAVector<DefinitelyNotATuple<int, int, int>>& edges,
                         int low, int high) {
        if (low < high) {
            int pi = std::ranges::partition(edges, low, high);
            QuickSort(edges, low, pi - 1);
            QuickSort(edges, pi + 1, high);
        }
    }

    static int partition(DefinitelyNotAVector<DefinitelyNotATuple<int, int, int>>& edges,
                        int low, int high) {
        auto pivot = edges[high].third();
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (edges[j].third() <= pivot) {
                i++;
                MySwap(edges[i], edges[j]);
            }
        }
        MySwap(edges[i + 1], edges[high]);
        return i + 1;
    }

    static void MySwap(DefinitelyNotATuple<int, int, int>& a,
                    DefinitelyNotATuple<int, int, int>& b) {
        DefinitelyNotATuple<int, int, int> temp = a;
        a = b;
        b = temp;
    }
};

#endif //SORTING_H
