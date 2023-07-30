#ifdef TEST_DEMO

#include "stdio.h"
#include "external.h"


void test_gets(void)
{
    char s[20] = "\0";
    printf("please input str\n");
    fgets(s, 20, stdin);
    printf("output:%s\n", s);
}

int main(void)
{
    printf("TEST MAIN ENTRY\n");
    mainSnakeTest();
    return 0;
}

#endif
