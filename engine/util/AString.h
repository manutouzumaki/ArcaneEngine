#ifndef ASTRING_H
#define ASTRING_H

void StringCopy(char *dest, int size, const char *src);
int StringLength(const char *string);
bool StringCompare(const char *A, const char *B, int Size);

class AString
{
public:
    AString() {}
    AString(const char *string);
    const char *get();
    unsigned long long getID();
    int length();
    bool operator==(AString &string);
private:
    const char *string;
    unsigned long long id;
};

#endif
