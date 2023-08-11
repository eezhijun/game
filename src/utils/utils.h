#ifndef _UTILS_H_
#define _UTILS_H_

#include "stdint.h"

#ifndef UNUSED
#define UNUSED(x) (void)(x)
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

// TODO
// ROUNDUP ROUNDDOWN
// __attribute__((section( name )))
// #define CONTAINER_OF(ptr, type, member) ((type *)((char *)ptr - offsetof(type, member)))
// endian int32/uint32 reverse uint8 point


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
 * @brief dump a piece of data
 *
 * @param data
 * @param len
 */
void dump_bytes(const uint8_t *data, int len);
#endif