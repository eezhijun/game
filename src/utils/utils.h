/**
 * @file utils.h
 * @author hongzhijun (eehongzhijun@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-08-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include "stdint.h"
#include "stddef.h"
#include "limits.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#define STR(s) #s

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT "_WIN32"
#else
#define ENVIRONMENT "_WIN64"
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT "__GNUC__64"
#else
#define ENVIRONMENT "__GNUC__32"
#endif
#endif

#define COUNT_DIGITS(num)       \
    ({                          \
        int count = 0;          \
        int temp = num;         \
        if (temp == 0) {        \
            count = 1;          \
        } else {                \
            while (temp != 0) { \
                temp /= 10;     \
                count++;        \
            }                   \
        }                       \
        count;                  \
    })

#ifndef FFS
#define FFS(x) ((x) ? __builtin_ffs(x) : 0)
#endif

#ifndef FLS
#define FLS(x) ((x) ? (sizeof(x) * CHAR_BIT - __builtin_clz(x)) : 0)
#endif

#ifndef CLZ
#define CLZ(x) ((x) ? __builtin_clz(x) : sizeof(x) * CHAR_BIT)
#endif

#ifndef CTZ
#define CTZ(x) ((x) ? __builtin_ctz(x) : sizeof(x) * CHAR_BIT)
#endif

#define linkly(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

#ifndef BIT
#define BIT(x) (1u << (x))
#endif

#ifndef BIT64
#define BIT64(x) ((uint64_t)1u << (x))
#endif

// calculate the total size of the array
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])
#endif

// calculate the total size of the 2D array
#ifndef XARRAY_SIZE
#define XARRAY_SIZE(a) (sizeof(a) / sizeof(a[0][0]))
#endif

// calculate the number of rows in the 2D array
#ifndef ROW_SIZE
#define ROW_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

// calculate the number of columns in the 2D array
#ifndef COL_SIZE
#define COL_SIZE(a) (sizeof(a[0]) / sizeof(a[0][0]))
#endif

#ifndef PRINT_ARRAY
#define PRINT_ARRAY(arr, size, format)      \
    do {                                    \
        for (size_t i = 0; i < size; i++) { \
            printf(format, arr[i]);         \
        }                                   \
    } while (0)
#endif

#ifndef PRINT_2DARRAY
#define PRINT_2DARRAY(arr, row, col, format)   \
    do {                                       \
        for (size_t i = 0; i < row; i++) {     \
            for (size_t j = 0; j < col; j++) { \
                printf(format, arr[i][j]);     \
            }                                  \
            printf("\n");                      \
        }                                      \
    } while (0)
#endif

/*
    Limit a value between an upper and lower limit.
    When the value exceeds the range of the minimum and maximum values,
    select a value between the minimum and maximum values.
*/
#ifndef CLAMP
#define CLAMP(value, min, max)                                  \
    ({                                                          \
        typeof(value) _value = (value);                         \
        typeof(min) _min = (min);                               \
        typeof(max) _max = (max);                               \
        _value < _min ? _min : (_value > _max ? _max : _value); \
    })
#endif

#ifndef HTONL
#define HTONL(x)                                                        \
    (((x & 0xFF) << 24) | ((x & 0xFF00) << 8) | ((x & 0xFF0000) >> 8) | \
     ((x & 0xFF000000) >> 24))
#endif

#ifndef HTONS
#define HTONS(x) (((x & 0xFF) << 8) | ((x & 0xFF00) >> 8))
#endif

#ifndef NTOHL
#define NTOHL(x) HTONL(x)
#endif

#ifndef NTOHS
#define NTOHS(x) HTONS(x)
#endif

/* max */
#ifndef MAX
#define MAX(a, b)           \
    ({                      \
        typeof(a) _a = (a); \
        typeof(b) _b = (b); \
        _a > _b ? _a : _b;  \
    })
#endif

#ifndef MIN
#define MIN(a, b)           \
    ({                      \
        typeof(a) _a = (a); \
        typeof(b) _b = (b); \
        _a < _b ? _a : _b;  \
    })
#endif

// Round a value up
#ifndef ROUNDUP
#define ROUNDUP(x) ((x) < 0 ? (int)(x) : (int)((x) + 1))
#endif

// Round a value down
#ifndef ROUNDDOWN
#define ROUNDDOWN(x) ((x) < 0 ? (int)((x)-1) : (int)(x))
#endif

#ifndef CONTAINER_OF
#define CONTAINER_OF(ptr, type, member) \
    ((type *)((char *)(ptr)-offsetof(type, member)))
#endif

/**
 * @brief
 *
 * @param a
 * @param b
 * @return int
 */
int max(int a, int b);

/**
 * @brief
 *
 * @param a
 * @param b
 * @return int
 */
int min(int a, int b);

/**
 * @brief round up
 * roundup(5.3) = 6
 *
 * @param num
 * @return double
 */
double roundup(double num);

/**
 * @brief round down
 * rounddown(5.3) 5
 *
 * @param num
 * @return double
 */
double rounddown(double num);

/**
 * @brief Limit a value between an upper and lower limit,
 * when the value exceeds the range of the minimum and maximum values,
 * choose a value between the minimum and maximum values to use
 * min=1, max=5
 * value=2, clamp(2, 1, 5)=2
 * value=6, clamp(6, 1, 5)=5
 *
 * @param value
 * @param min
 * @param max
 * @return int
 */
int clamp(int value, int min, int max);

/* swap */
#ifndef SWAP
#define SWAP(type, lhs, rhs) \
    do {                     \
        type temp = *lhs;    \
        *lhs = *rhs;         \
        *rhs = temp;         \
    } while (0)
#endif

/**
 * @brief
 *
 * @param lhs left hand side
 * @param rhs right hand side
 * @param size need to malloc swap value type size
 */
void swap(void *lhs, void *rhs, size_t size);

/**
 * @brief
 *
 * @param lhs left hand side
 * @param rhs right hand side
 */
void swap_by_temp(int *lhs, int *rhs);

/**
 * @brief
 *
 * @param lhs left hand side
 * @param rhs right hand side
 */
void swap_by_sum(int *lhs, int *rhs);

/**
 * @brief xor means logical operation
 *
 * @param lhs left hand side
 * @param rhs right hand side
 */
void swap_by_xor(int *lhs, int *rhs);

/* keyboard */
#define UP_ARROW (char)'A'
#define LEFT_ARROW (char)'D'
#define RIGHT_ARROW (char)'C'
#define DOWN_ARROW (char)'B'

#define ENTER_KEY 10
#define EXIT_BUTTON 27 //ESC
#define PAUSE_BUTTON 112 //P

#ifndef BELL
#define BELL()        \
    do {              \
        printf("\7"); \
    } while (0)
#endif

/**
 * @brief cursor move
 *
 * @param x
 * @param y
 * @see http://ubuntuforums.org/showthread.php?t=549023
 */
void gotoxy(int x, int y);

/**
 * @brief get char
 *
 * @return char
 * @see http://www.experts-exchange.com/Programming/Languages/C/Q_10119844.html - posted by jos
 */
char getch(void);

/**
 * @brief clear screen
 *
 */
void clrscr(void);

/**
 * @brief key check
 *
 * @return int
 * @see http://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html
 */
int kbhit(void);

/**
 * @brief wait key check
 *
 * @return char
 */
char wait_4_key(void);

/**
 * @brief
 *
 */
void set_terminal_attributes(void);

/**
 * @brief
 *
 */
void restore_terminal_attributes(void);

/**
 * @brief show cursor
 *
 */
void show_cursor(void);

/**
 * @brief hide cursor
 *
 */
void hide_cursor(void);

/* sort */
/**
 * @brief bubble sort
 *
 * @param arr
 * @param len
 */
void bubble_sort(int arr[], int len);

/**
 * @brief quick sort
 *
 * @param pa
 * @param pb
 * @return int
 */
int cmp(const void *pa, const void *pb);

/**
 * @brief
 *
 * @param elem
 */
void print_int(void *elem);
void print_float(void *elem);
void print_string(void *elem);
void print_arr(void *arr, size_t size, size_t elem_size,
               void(*print_elem(void *)));

/**
 * @brief Find First One, used to find the position (index) of
 * the first set (value 1) starting from the lowest bit in an integer or bit field,
 * that is, to find the position of the rightmost 1
 *
 * @param x
 * @return int
 */
int ffs(int x);

/**
 * @brief Find Last Set, Used to find the position (index) of
 * the last set bit (value 1) starting from the highest bit in an integer or bit field,
 * that is, to find the position of the leftmost 1.
 *
 * @param x
 * @return int
 */
int fls(int x);

/**
 * @brief Count Leading Zeros, used to count the number of
 * consecutive 0s starting from the highest bit in the binary representation of an integer
 *
 * @param x
 * @return int
 */
int clz(int x);

/**
 * @brief Count Trailing Zeros, used to count the number of
 * consecutive zeros starting from the lowest bit in the binary representation of an integer
 *
 * @param x
 * @return int
 */
int ctz(int x);

/**
 * @brief int to string
 *
 * @param num
 * @param str
 * @return char*
 */
char *int2string(int num, char *str);

/**
 * @brief string to int
 *
 * @param str
 * @return int
 */
int string2int(char *str);

/**
 * @brief reverse string
 *
 * @param s source string
 * @param l left char postion
 * @param r right char postion
 */
void reverse(char *s, int l, int r);

/**
 * @brief hex to dec
 *
 * @param hex
 * @return int
 */
int hex2dec(char hex[]);

/**
 * @brief dec to hex
 *
 * @param dec
 * @return char*
 */
char *dec2hex(int dec);

/**
 * @brief
 *
 * @param num
 * @return int
 */
int count_digits(int num);

/**
 * @brief
 *
 * @param num
 */
void printf_bin(int num);

typedef unsigned long Uint;

/**
 * @brief Setting a bit
 *
 * @param number
 * @param n
 * @return Uint
 */
static inline Uint bit_set(Uint number, Uint n)
{
    return number | ((Uint)1 << n);
}

/**
 * @brief Clearing a bit
 *
 * @param number
 * @param n
 * @return Uint
 */
static inline Uint bit_clear(Uint number, Uint n)
{
    return number & ~((Uint)1 << n);
}

/**
 * @brief Toggling a bit
 *
 * @param number
 * @param n
 * @return Uint
 */
static inline Uint bit_toggle(Uint number, Uint n)
{
    return number ^ ((Uint)1 << n);
}

/**
 * @brief Checking a bit
 *
 * @param number
 * @param n
 * @return true
 * @return false
 */
static inline bool bit_check(Uint number, Uint n)
{
    return (number >> n) & (Uint)1;
}

/**
 * @brief Changing the nth bit to x
 *
 * @param number
 * @param n
 * @param x
 * @return Uint
 */
static inline Uint bit_set_to(Uint number, Uint n, bool x)
{
    return (number & ~((Uint)1 << n)) | ((Uint)x << n);
}

#ifdef __cplusplus
}
#endif

#endif
