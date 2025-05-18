#ifndef BINARYHEAP_H
#define BINARYHEAP_H

#include "DefinitelyNotADataStructures/DefinitelyNotAVector.h"

template<typename T>
class BinaryHeap {
private:
    struct HeapNode {
        T value;
        int vertex;
        int key;

        HeapNode(T v, int vtx, int k) : value(v), vertex(vtx), key(k) {}

        bool operator<(const HeapNode& other) const {
            return value < other.value;
        }
    };

    DefinitelyNotAVector<HeapNode> heap;
    DefinitelyNotAVector<int> position; // Maps vertex to its position in heap

    void swap(int i, int j) {
        HeapNode temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;

        position[heap[i].vertex] = i;
        position[heap[j].vertex] = j;
    }

    void heapify(int idx) {
        int smallest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;

        if (left < heap.GetSize() && heap[left] < heap[smallest])
            smallest = left;

        if (right < heap.GetSize() && heap[right] < heap[smallest])
            smallest = right;

        if (smallest != idx) {
            swap(idx, smallest);
            heapify(smallest);
        }
    }

    void bubbleUp(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (heap[idx] < heap[parent]) {
                swap(idx, parent);
                idx = parent;
            } else {
                break;
            }
        }
    }

public:
    explicit BinaryHeap(int capacity) : position(capacity, -1) {}

    void insert(T value, int vertex, int key) {
        heap.Push(HeapNode(value, vertex, key));
        position[vertex] = heap.GetSize() - 1;
        bubbleUp(heap.GetSize() - 1);
    }

    HeapNode extractMin() {
        if (heap.GetSize() <= 0) {
            return HeapNode(T(), -1, -1);
        }

        HeapNode min = heap[0];
        position[min.vertex] = -1;

        heap[0] = heap[heap.GetSize() - 1];
        position[heap[0].vertex] = 0;
        heap.Pop();

        if (heap.GetSize() > 0) {
            heapify(0);
        }

        return min;
    }

    void decreaseKey(int vertex, T newValue) {
        int idx = position[vertex];
        if (idx != -1 && newValue < heap[idx].value) {
            heap[idx].value = newValue;
            bubbleUp(idx);
        }
    }

    bool contains(int vertex) const {
        return position[vertex] != -1;
    }

    bool empty() const {
        return heap.GetSize() == 0;
    }
};


#endif //BINARYHEAP_H
