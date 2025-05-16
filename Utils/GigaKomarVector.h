#ifndef GIGAKOMARVECTOR_H
#define GIGAKOMARVECTOR_H
#include
#include
#include
#include

template<class T>
class GigaKomarVector {
private:
    T* arr;
    int capacity;
    int current;

public:
    explicit GigaKomarVector(int initial_capacity = 4);
    ~GigaKomarVector();

    T& operator[](int index);
    const T& operator[](int index) const;

    void Push(const T& data);
    void PushData(const T& data, int index);
    void PopData();

    T & GetData(int index);
    const T& GetData(int index) const;
    [[nodiscard]] int GetSize() const;
    [[nodiscard]] int GetCapacity() const;
    [[nodiscard]] bool Empty() const;
    void Clear();
    void Print() const;
};

template<class T>
GigaKomarVector<T>::GigaKomarVector(int initial_capacity)
    : capacity(initial_capacity), current(0)
{
    arr = new T[capacity];
}

template<class T>
GigaKomarVector<T>::~GigaKomarVector()
{
    delete[] arr;
}

template<class T>
T& GigaKomarVector<T>::operator[](int index)
{
    return arr[index];
}

template<class T>
const T& GigaKomarVector<T>::operator[](int index) const
{
    return arr[index];
}

template<class T>
void GigaKomarVector<T>::Push(const T& data)
{
    if (current == capacity)
    {
        T* temp = new T[2 * capacity];
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

template<class T>
void GigaKomarVector<T>::PushData(const T& data, int index)
{
    if (index >= current)
    {
        if (index >= capacity)
        {
            int new_capacity = capacity;
            while (new_capacity <= index)
            {
                new_capacity *= 2;
            }
            T* temp = new T[new_capacity];
            for (int i = 0; i < current; i++)
            {
                temp[i] = arr[i];
            }
            delete[] arr;
            arr = temp;
            capacity = new_capacity;
        }
        current = index + 1;
    }
    arr[index] = data;
}

template<class T>
void GigaKomarVector<T>::PopData()
{
    if (current > 0)
    {
        --current;
    }
}

template<class T>
T & GigaKomarVector<T>::GetData(int index)
{
    return arr[index];
}

template<class T>
const T& GigaKomarVector<T>::GetData(int index) const
{
    return arr[index];
}

template<class T>
int GigaKomarVector<T>::GetSize() const
{
    return current;
}

template<class T>
int GigaKomarVector<T>::GetCapacity() const
{
    return capacity;
}

template<class T>
bool GigaKomarVector<T>::Empty() const
{
    return current == 0;
}

template<class T>
void GigaKomarVector<T>::Clear()
{
    current = 0;
}

template<class T>
void GigaKomarVector<T>::Print() const
{
    for (int i = 0; i < current; i++)
    {
        std::cout << arr[i] << " \n";
    }
}

#endif //GIGAKOMARVECTOR_H
