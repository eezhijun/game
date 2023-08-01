#ifdef TEST_DEMO

#include "stdio.h"
#include "unistd.h"
#include "common.h"
#include "test.h"


void tt_fgets(void)
{
    char s[20] = "\0";
    printf("please input str\n");
    fgets(s, 20, stdin);
    printf("output:%s\n", s);
}

void tt_gotoxy(void)
{
    gotoxy(50, 10);
    printf("cursor end\n");
    sleep(5);
}

void tt_getchar(void)
{
    char c;

    printf("please enter a char\n");
    c = getchar();
    printf("c=%c\n", c);
}

void tt_getch(void)
{
    char c;

    printf("please enter a char\n");
    c = getch();
    printf("c=%c\n", c);
}

int main(void)
{
    printf("TEST MAIN ENTRY\n");
    test();
    // tt_getchar();
    // tt_getch();
    return 0;
}

#endif
