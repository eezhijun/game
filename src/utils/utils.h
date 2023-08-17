#ifndef _UTILS_H_
#define _UTILS_H_

#include "stdint.h"
#include "stddef.h"
#include "limits.h"

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

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

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])
#endif

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

#ifndef ROUNDUP
#define ROUNDUP(x) ((x) < 0 ? (int)(x) : (int)((x) + 1))
#endif

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
 * @brief
 *
 * @param num
 * @return double
 */
double roundup(double num);

/**
 * @brief
 *
 * @param num
 * @return double
 */
double rounddown(double num);

/**
 * @brief Limit a value between an upper and lower limit,
 * when the value exceeds the range of the minimum and maximum values,
 * choose a value between the minimum and maximum values to use
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
        type temp = lhs;     \
        lhs = rhs;           \
        rhs = temp;          \
    } while (0)
#endif

/**
 * @brief
 *
 * @param lhs
 * @param rhs
 * @param size
 */
void swap(void *lhs, void *rhs, size_t size);

/**
 * @brief
 *
 * @param lhs
 * @param rhs
 */
void swap_by_temp(int *lhs, int *rhs);

/**
 * @brief
 *
 * @param lhs
 * @param rhs
 */
void swap_by_sum(int *lhs, int *rhs);

/**
 * @brief
 *
 * @param lhs
 * @param rhs
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
 * @brief
 *
 * @return char
 */
char wait_4_key(void);

/* sort */
/**
 * @brief
 *
 * @param arr
 * @param len
 */
void bubble_sort(int arr[], int len);

/**
 * @brief
 *
 * @param pa
 * @param pb
 * @return int
 */
int cmp(const void *pa, const void *pb);

/**
 * @brief
 *
 * @param arr
 * @param len
 */
void print_array(int arr[], int len);

/**
 * @brief
 *
 * @param data
 * @param len
 */
void dump_x(const uint8_t *data, size_t len);

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

#endif
