#ifndef TEST_H
#define TEST_H

#include "ADefines.h"
#include <malloc.h>

template <class T>
class AArray
{
public:
    AArray();
    AArray(size_t capacity);
    ~AArray();
    void add(T element);
    size_t size();
    size_t getCapacity();
    T &operator[](size_t index);
    size_t occupied = 0;
private:
    size_t capacity = 0;
    T *data = nullptr;
};

template <class T>
AArray<T>::AArray()
{
    this->capacity = 0;
    this->occupied = 0;
    this->data = nullptr;
}

template <class T>
AArray<T>::AArray(size_t capacity)
{
    this->capacity = capacity;
    this->occupied = 0;
    this->data = (T *)malloc(capacity * sizeof(T));
}


template <class T>
AArray<T>::~AArray()
{
    free(this->data);
}

template <class T>
void AArray<T>::add(T element)
{
    if(!this->data)
    {
        this->data = (T *)malloc(sizeof(T));
        this->data[this->occupied++] = element;
        this->capacity++;
    }
    else if(this->occupied + 1 <= this->capacity)
    {
        this->data[this->occupied++] = element;
    }
    else
    {
        size_t neededSize = this->occupied + 1;
        size_t defIncrement = this->capacity * 2;
        
        this->capacity = neededSize > defIncrement ? neededSize : defIncrement;
        this->data = (T *)realloc(this->data, this->capacity * sizeof(T));
        this->data[this->occupied++] = element;
    }
}

template <class T>
size_t AArray<T>::size()
{
    return this->occupied;
}

template <class T>
size_t AArray<T>::getCapacity()
{
    return this->capacity;
}

template <class T>
T &AArray<T>::operator[](size_t index)
{
    Assert(index < occupied);
    return this->data[index];
}


#endif
