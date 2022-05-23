#ifndef ADEFINES_H
#define ADEFINES_H

#define Kilobyte(value) (value * 1024LL)
#define Megabyte(value) (value * 1024LL * 1024LL)
#define Gigabyte(value) (value * 1024LL * 1024LL * 1024LL)
#define Terabyte(value) (value * 1024LL * 1024LL * 1024LL * 1024LL)

#define Assert(condition) if(!(condition)) { *(unsigned int*)0 = 0;}
#define ArrayCount(array) (sizeof(array)/sizeof((array)[0]))

#define PI 3.14159265359f

// SSE2
#define M(a, i) ((float *)&(a))[i]
#define Mi(a, i) ((int *)&(a))[i]
#define Mu(a, i) ((unsigned int *)&(a))[i]

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#endif
