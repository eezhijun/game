#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "unistd.h"
#include "fcntl.h"
#include "stdlib.h"
#include "termios.h"
#include "limits.h"

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
