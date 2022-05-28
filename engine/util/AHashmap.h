#ifndef AHASHMAP_H
#define AHASHMAP_H

#include "AHash.h"
#include "AString.h"
#include "ADefines.h"

#include <stdio.h>

// TODO: impove Hashmap code to work out side of visual studio


#include <malloc.h>

template <class T>
class AHashValue
{
public:
    AHashValue() {}
    AHashValue(T value, unsigned long long id, const char *name);
    T value;
    unsigned long long id;
    const char *name;
};

template <class T>
AHashValue<T>::AHashValue(T value, unsigned long long id, const char *name)
{
    this->value = value;
    this->id = id;
    this->name = name; 
}

template <class T>
class AHashmap
{
public:
    AHashmap();
    ~AHashmap();
    void add(AString key, T value);
    void remove(AString key);
    void grow();
    T &operator[](AString key);
private:
    size_t occupied;
    size_t capacity;
    unsigned long long mask;
    AHashValue<T> **values;
};

template <class T>
AHashmap<T>::AHashmap()
{
    capacity = 2;
    occupied = 0;
    mask = capacity - 1;
    values = (AHashValue<T> **)malloc(capacity * sizeof(AHashValue<T> *));
    for(size_t i = 0; i < capacity; ++i)
    {
        values[i] = nullptr;
    }
}

template <class T>
AHashmap<T>::~AHashmap()
{
    for(int i = 0; i < capacity; ++i)
    {
        if(values[i])
        {
            delete values[i];
        }
    }
    free(values);
}

template <class T>
void AHashmap<T>::add(AString key, T value)
{
    unsigned int hashIndex = (unsigned int)(key.getID() & mask);
    if(!values[hashIndex])
    {
        values[hashIndex] = new AHashValue<T>(value, key.getID(), key.get());
        occupied = occupied + 1;
    }
    else
    {
        unsigned int nextIndex = hashIndex + 1; 
        while(values[nextIndex] || nextIndex >= capacity)
        {
            nextIndex = (nextIndex + 1) % capacity;
        }
        values[nextIndex] = new AHashValue<T>(value, key.getID(), key.get());
        occupied = occupied + 1;
    }

    if(((float)occupied / (float)capacity) >= 0.7f)
    {
        grow();
    }
}

template <class T>
void AHashmap<T>::grow()
{
    size_t oldCapacity = capacity;
    capacity = capacity * 2;
    mask = capacity - 1;
    AHashValue<T> **tmpValues = (AHashValue<T> **)malloc(capacity * sizeof(AHashValue<T> *));
    for(size_t i = 0; i < capacity; ++i)
    {
        tmpValues[i] = nullptr;
    }
    for(size_t i = 0; i < oldCapacity; i++)
    {
        AHashValue<T> *hashValue = values[i];
        if(hashValue)
        {
            unsigned int hashIndex = (unsigned int)(hashValue->id & mask);
            if(!tmpValues[hashIndex])
            {
                tmpValues[hashIndex] = new AHashValue<T>(hashValue->value, hashValue->id, hashValue->name);
            }
            else
            { 
                unsigned int nextIndex = hashIndex + 1; 
                while(tmpValues[nextIndex] || nextIndex >= capacity)
                {
                    nextIndex = (nextIndex + 1) % capacity;
                }
                tmpValues[nextIndex] = new AHashValue<T>(hashValue->value, hashValue->id, hashValue->name);
            }
        }
        delete values[i];
        values[i] = nullptr;
    }
    free(values);
    values = tmpValues;
}

template <class T>
T &AHashmap<T>::operator[](AString key)
{
    unsigned int hashIndex = (unsigned int)(key.getID() & mask);
    while(values[hashIndex] && values[hashIndex]->id != key.getID())
    {
        hashIndex = (hashIndex + 1) % capacity;
    }
    if(values[hashIndex])
    {
        return values[hashIndex]->value;
    }
    else
    {
        Assert(!"ERROR: key is not on the hashmap");
        return values[hashIndex]->value;
    }
}

template <class T>
void AHashmap<T>::remove(AString key)
{
    unsigned int hashIndex = (unsigned int)(key.getID() & mask);
    while(values[hashIndex] && values[hashIndex]->id != key.getID())
    {
        hashIndex = (hashIndex + 1) % capacity;
    }

    if(values[hashIndex])
    {
        delete values[hashIndex];
        values[hashIndex] = nullptr;
        occupied = occupied - 1;
    }
    else
    {
        Assert(!"ERROR: key is not on the hashmap");
    }
}

#endif
