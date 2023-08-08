#include "stdio.h"

/* function template */
// #define MAXT(T) T max_##T(T a, T b) {return (a > b) ? a : b;}

// MAXT(int)
// MAXT(float)
// MAXT(double)

/**
 * @brief cal max
 *
 * @param a
 * @param b
 * @return int
 */
int max(int a, int b)
{
    return (a > b) ? a : b;
}

/**
 * @brief cal min
 *
 * @param a
 * @param b
 * @return int
 */
int min(int a, int b)
{
    return (a > b) ? b : a;
}
