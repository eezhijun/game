/**
 * @file utils.c
 * @author eehongzhijun (eehongzhijun@outlook.com)
 * @brief 
 * @version 0.0.1
 * @date 2023-08-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "math.h"
#include "unistd.h"
#include "limits.h"
#include "assert.h"

/* linux lib */
#include "fcntl.h"
#include "termios.h"

#include "utils.h"


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

void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}

char getch(void)
{
    char c;
    system("stty raw");
    c = getchar();
    system("stty sane");
    return c;
}

void clrscr(void)
{
    system("clear");
}

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

char wait_4_key(void)
{
    int pressed;

    while (!kbhit()) {
    }

    pressed = getch();
    //pressed = tolower(pressed);
    return (char)pressed;
}

void set_terminal_attributes(void)
{
    struct termios term;
    tcgetattr(STDOUT_FILENO, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(STDOUT_FILENO, TCSANOW, &term);
}

void restore_terminal_attributes(void)
{
    struct termios term;
    tcgetattr(STDOUT_FILENO, &term);
    term.c_lflag |= ECHO;
    tcsetattr(STDOUT_FILENO, TCSANOW, &term);
}

void hide_cursor(void)
{
    printf("\e[?25l");
}

void show_cursor(void)
{
    printf("\e[?25h");
}

int ffs(int x)
{
    if (x == 0)
        return 0;
    int position = 1;
    while ((x & 1) == 0) {
        x >>= 1;
        position++;
    }
    return position;
}

int fls(int x)
{
    if (x == 0)
        return 0;
    int position = sizeof(x) * CHAR_BIT;
    while ((x & (1 << (position - 1))) == 0) {
        position--;
    }
    return position;
}

int clz(int x)
{
    if (x == 0)
        return sizeof(x) * CHAR_BIT;
    int count = 0;
    while ((x & (1 << (sizeof(x) * CHAR_BIT - 1))) == 0) {
        x <<= 1;
        count++;
    }
    return count;
}

int ctz(int x)
{
    if (x == 0)
        return sizeof(x) * CHAR_BIT;
    int count = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        count++;
    }
    return count;
}

#define DUMP_BYTES 16
#define DUMP_BUFFER_SIZE 512

static char dump_buffer[DUMP_BUFFER_SIZE] = { 0 };
void dump_x(const uint8_t *data, size_t len)
{
    size_t line = len / DUMP_BYTES;
    int offset = 0;
    for (int i = 0; i < line; i++) {
        if (((int)sizeof(dump_buffer) - offset) < 0) {
            break;
        }

        for (int j = i * DUMP_BYTES; j < DUMP_BYTES + DUMP_BYTES * i; j++) {
            offset += snprintf(dump_buffer + offset,
                               sizeof(dump_buffer) - offset, "%02X ", data[j]);
        }
        offset += snprintf(dump_buffer + offset, sizeof(dump_buffer) - offset, "\n");

    }

    if (len > line * DUMP_BYTES) {
        for (int i = line * DUMP_BYTES; i < DUMP_BYTES + line * DUMP_BYTES; i++) {
            if (((int)sizeof(dump_buffer) - offset) < 0) {
                break;
            }

            if (i < len) {
                offset += snprintf(dump_buffer + offset,
                                sizeof(dump_buffer) - offset, "%02X ", data[i]);
            } else {
                offset += snprintf(dump_buffer + offset,
                                sizeof(dump_buffer) - offset, "%02X ", 0);
            }
        }
        offset += snprintf(dump_buffer + offset, sizeof(dump_buffer) - offset, "\n");
    }
    printf("date len: %d\n", len);
    printf("%s", dump_buffer);
}

void print_int(void *elem)
{
    int *num = (int *)elem;
    printf("%d ", *num);
}

void print_float(void *elem)
{
    float *num = (float *)elem;
    printf("%f ", *num);
}

void print_string(void *elem)
{
    char **str = (char **)elem;
    printf("%s\n", *str);
}

void print_arr(void *arr, size_t size, size_t elem_size, void (*print_elem(void *)))
{
    for (size_t i = 0; i < size; i++) {
        print_elem(arr + i * elem_size);
    }
    printf("\n");
}



