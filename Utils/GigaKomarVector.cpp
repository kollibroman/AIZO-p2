#include "GigaKomarVector.h"
#include <iostream>

template<class T>
class GigaKomarVector<T>
{
    T *arr;
    int capacity;
    T current;

public:
    GigaKomarVector::GigaKomarVector()
    {
        arr = new int[4];

        //Best size/performance ratio
        capacity = 4;
        current = 0;
    };

    void GigaKomarVector::Push(T data)
    {
        if (current == capacity)
        {
            T *temp = new T[2 * capacity];

            for (int i = 0; i < capacity; i++)
            {
                temp[i] = arr[i];
            }

            delete[] arr;
            capacity *= 2;

            arr = temp;
        }

        arr[current] = data;
        ++current;
    }

    void GigaKomarVector::PushData(T data, int index)
    {
        if (index == capacity)
        {
            Push(index);
        }

        else
        {
            arr[index] = data;
        }
    }
    void GigaKomarVector::PopData()
    {
        --current;
    }

    T* GigaKomarVector::GetData(int index)
    {
        if (index < current)
        {
            return &arr[index];
        }

        return nullptr;
    }

    int GigaKomarVector::GetSize()
    {
        return current;
    }

    void GigaKomarVector::Print()
    {
        for (int i = 0; i < current; i++)
        {
            std::cout << arr[i] << " \n";
        }
    }
};