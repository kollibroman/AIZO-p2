#ifndef GIGAKOMARVECTOR_H
#define GIGAKOMARVECTOR_H

template<class T>
class BigDVector {
private:
    T* arr;
    int capacity;
    int current;

public:
    explicit BigDVector(int initial_capacity = 4);
    ~BigDVector();

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
BigDVector<T>::BigDVector(int initial_capacity)
    : capacity(initial_capacity), current(0)
{
    arr = new T[capacity];
}

template<class T>
BigDVector<T>::~BigDVector()
{
    delete[] arr;
}

template<class T>
T& BigDVector<T>::operator[](int index)
{
    return arr[index];
}

template<class T>
const T& BigDVector<T>::operator[](int index) const
{
    return arr[index];
}

template<class T>
void BigDVector<T>::Push(const T& data)
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
void BigDVector<T>::PushData(const T& data, int index)
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
void BigDVector<T>::PopData()
{
    if (current > 0)
    {
        --current;
    }
}

template<class T>
T & BigDVector<T>::GetData(int index)
{
    return arr[index];
}

template<class T>
const T& BigDVector<T>::GetData(int index) const
{
    return arr[index];
}

template<class T>
int BigDVector<T>::GetSize() const
{
    return current;
}

template<class T>
int BigDVector<T>::GetCapacity() const
{
    return capacity;
}

template<class T>
bool BigDVector<T>::Empty() const
{
    return current == 0;
}

template<class T>
void BigDVector<T>::Clear()
{
    current = 0;
}

template<class T>
void BigDVector<T>::Print() const
{
    for (int i = 0; i < current; i++)
    {
        std::cout << arr[i] << " \n";
    }
}

#endif //GIGAKOMARVECTOR_H
