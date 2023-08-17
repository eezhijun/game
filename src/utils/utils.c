#include "stdio.h"
#include "stdlib.h"
#include "string.h"
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

void swap(void *lhs, void *rhs, size_t size)
{
    if (lhs == NULL || rhs == NULL || size == 0) {
        return;
    }
    void *t = malloc(size);
    memcpy(t, lhs, size);
    memcpy(lhs, rhs, size);
    memcpy(rhs, t, size);
    free(t);
}

void swap_by_temp(int *lhs, int *rhs)
{
    if (lhs == NULL || rhs == NULL) {
        return;
    }
    int temp = *lhs;
    *lhs = *rhs;
    *rhs = temp;
}

void swap_by_sum(int *lhs, int *rhs)
{
    if (lhs == NULL || rhs == NULL) {
        return;
    }
    *lhs = *lhs + *rhs;
    *rhs = *lhs - *rhs;
    *lhs = *lhs - *rhs;
}

void swap_by_xor(int *lhs, int *rhs)
{
    if (lhs == NULL || rhs == NULL) {
        return;
    }
    *lhs = *lhs ^ *rhs;
    *rhs = *lhs ^ *rhs;
    *lhs = *lhs ^ *rhs;
}
