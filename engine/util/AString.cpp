#include "AString.h"
#include "AHash.h"

int StringLength(const char *string)
{
    int counter = 0;
    char *element = (char *)string;
    while(*element++)
    {
        ++counter;
    }
    return counter;
}

bool StringCompare(const char *A, const char *B, int Size)
{
    bool Result = true;
    for(int Index = 0;
        Index < Size;
        ++Index)
    {
        if(*A++ != *B++)
        {
            Result = false;
            break;
        } 
    }
    return Result;
}

AString::AString(const char *string)
{
    id = AHash::murMur64A(string, StringLength(string), 123);
    this->string = string;

}
const char *AString::get()
{
    return string;
}
unsigned long long AString::getID()
{
    return id;
}
int AString::length()
{
    return StringLength(string);    
}

bool AString::operator==(AString &string)
{
    return (id == string.id);
}
