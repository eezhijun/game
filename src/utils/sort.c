/**
 * @file sort.c
 * @author hongzhijun (eehongzhijun@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-08-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "stdio.h"
#include "stdbool.h"

void bubble_sort(int arr[], int len)
{
    int i, j, temp;
    bool exchanged = true;

    for (i = 0; exchanged && i < len - 1; i++) {
        exchanged = false;
        for (j = 0; j < len - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                exchanged = true;
            }
        }
    }
}

int cmp(const void *pa, const void *pb)
{
    int a = *(int *)pa;
    int b = *(int *)pb;
    return a - b;
}
