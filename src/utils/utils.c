#include "stdio.h"
#include "math.h"

/* function template */
// #define MAXT(T) T max_##T(T a, T b) {return (a > b) ? a : b;}

// MAXT(int)
// MAXT(float)
// MAXT(double)

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int min(int a, int b)
{
    return (a > b) ? b : a;
}

int clamp(int value, int min, int max)
{
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}

double roundup(double num)
{
    if (num >= 0) {
        return ceil(num);
    } else {
        return floor(num);
    }
}

double rounddown(double num)
{
    if (num >= 0) {
        return floor(num);
    } else {
        return ceil(num);
    }
}
