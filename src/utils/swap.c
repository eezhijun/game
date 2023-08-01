#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void swap(void* lhs, void* rhs, size_t size)
{
    if (lhs == NULL || rhs == NULL || size == 0)
    {
        return;
    }
    void *t = malloc(size);
    memcpy(t, lhs, size);
    memcpy(lhs, rhs, size);
    memcpy(rhs, t, size);
    free(t);
}

void swap_by_temp(int* lhs, int *rhs)
{
    if (lhs == NULL || rhs == NULL)
    {
        return;
    }
    int temp = *lhs;
    *lhs = *rhs;
    *rhs = temp;
}

void swap_by_sum(int* lhs, int* rhs)
{
    if (lhs == NULL || rhs == NULL)
    {
        return;
    }
    *lhs = *lhs + *rhs;
    *rhs = *lhs - *rhs;
    *lhs = *lhs - *rhs;
}

void swap_by_xor(int *lhs, int *rhs)
{
    if (lhs == NULL || rhs == NULL)
    {
        return;
    }
    *lhs = *lhs ^ *rhs;
    *rhs = *lhs ^ *rhs;
    *lhs = *lhs ^ *rhs;
}
