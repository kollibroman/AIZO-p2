#ifndef GIGAKOMARVECTOR_H
#define GIGAKOMARVECTOR_H

template<class T>
class GigaKomarVector<T>
{
    T *arr;
    int capacity;
    int current;

public:
    GigaKomarVector();

    void Push(T data);
    void PushData(T data, int index);
    void PopData();
    T* GetData(int index);
    int  GetSize();
    void Print();
};

#endif //GIGAKOMARVECTOR_H
