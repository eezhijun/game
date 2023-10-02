#include "stdio.h"
#include "unistd.h"
#include "assert.h"
#include "stdint.h"
#include "stdlib.h"
#include "time.h"

#include "utils.h"

/* keyboard test */
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

/* https://stackoverflow.com/questions/10463201/getch-and-arrow-codes/11432632#11432632 */
void tt_getch(void)
{
    // if the first value is esc
    if (getch() == '\033') {
        getch(); // skip the [
        // the real value
        switch (getch()) {
        case 'A':
            assert(0);
            // code for arrow up
            break;
        case 'B':
            // code for arrow down
            break;
        case 'C':
            // code for arrow right
            break;
        case 'D':
            // code for arrow left
            break;
        default:
            break;
        }
    }
}

void tt_hton(void)
{
    uint32_t original32 = 0x12345678;
    uint16_t original16 = 0x1234;

    uint32_t network32 = HTONL(original32);
    uint16_t network16 = HTONS(original16);

    uint32_t host32 = NTOHL(network32);
    uint16_t host16 = NTOHS(network16);

    printf("Original 32-bit value: 0x%08X\n", original32);
    printf("Network 32-bit value: 0x%08X\n", network32);
    printf("Host 32-bit value: 0x%08X\n", host32);

    printf("Original 16-bit value: 0x%04X\n", original16);
    printf("Network 16-bit value: 0x%04X\n", network16);
    printf("Host 16-bit value: 0x%04X\n", host16);
}

#define ARRAY_SIZEX 300
#define MIN_VALUE 1
#define MAX_VALUE 100

void tt_dump_bytes(void)
{
    uint8_t random_numbers[ARRAY_SIZEX];
    int i;

    // Set the random number seed, using the current time as the seed
    srand(time(NULL));

    for (i = 0; i < ARRAY_SIZEX; i++) {
        random_numbers[i] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
    }

    printf("Generated random numbers:\n");
    for (i = 0; i < ARRAY_SIZEX; i++) {
        // 0-15 16-31

        printf("%02X ", random_numbers[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
            // assert(0);
        }
    }

    printf("\n-----------------------------------------------------------\n");
    dump_x(random_numbers, ARRAY_SIZE(random_numbers));
}

void tt_roundx(void)
{
    double value = 7.3;
    double result = ROUNDUP(value);
    printf("Original value: %.2f\n", value);
    printf("Rounded up value: %.2f\n", result);

    value = 7.8;
    result = ROUNDDOWN(value);
    printf("Original value: %.2f\n", value);
    printf("Rounded down value: %.2f\n", result);
}

struct Person {
    char name[20];
    int age;
    float height;
};

void tt_container_of(void)
{
    struct Person person;
    struct Person *person_ptr = &person;

    person_ptr->age = 30;

    // Using the CONTAINER_OF macro to get the address of the whole struct
    struct Person *recovered_person_ptr =
        CONTAINER_OF(&person_ptr->age, struct Person, age);

    printf("Recovered person's age: %d\n", recovered_person_ptr->age);
}

void tt_bit(void)
{
    int num = 0b00101000;

    printf("ffs: %d\n", ffs(num));
    printf("fls: %d\n", fls(num));
    printf("clz: %d\n", clz(num));
    printf("ctz: %d\n", ctz(num));
}

void tt_clock(void)
{
    clock_t start_time, end_time;
    double cpu_time_used;

    start_time = clock();

    // The block of code whose execution time is to be measured
    for (int i = 0; i < 1000000; i++) {
        // some calculation or operation
    }

    end_time = clock();

    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("execute time:%f s\n", cpu_time_used);
}

void tt_cursor(void)
{
    gotoxy(1, 1);
    hide_cursor();

    wait_4_key();
    show_cursor();
}

void tt_string2int(void)
{
    char str[10] = { 'a', 'b', 'c', '1', '2', '3', '\0', 'x', 'y' };

    int num = string2int(&str[3]);
    printf("num=%d\n", num);
}

#ifdef TEST_DEMO
int main(void)
{
    printf("TEST MAIN ENTRY\n");

    return 0;
}
#endif
