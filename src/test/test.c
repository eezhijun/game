#ifdef TEST_DEMO

#include "stdio.h"
#include "unistd.h"
#include "assert.h"
#include "stdint.h"
#include "stdlib.h"
#include "time.h"

#include "ringbuffer.h"

#include "utils.h"

extern int snake_test(void);
extern int stack_test(void);
extern int queue_test(void);

/* ringbuffer test */
void tt_ringbuffer(void)
{
    int i, cnt;
    char tmp;
    char tmp_arr[50];

    /* Create and initialize ring buffer */
    ring_buffer_t ring_buffer;
    char buf_arr[128];
    ring_buffer_init(&ring_buffer, buf_arr, sizeof(buf_arr));

    /* Add elements to buffer; one at a time */
    for (i = 0; i < 100; i++) {
        ring_buffer_queue(&ring_buffer, i);
    }

    /* Verify size */
    assert(ring_buffer_num_items(&ring_buffer) == 100);

    /* Peek third element */
    cnt = ring_buffer_peek(&ring_buffer, &tmp, 3);
    /* Assert byte returned */
    assert(cnt == 1);
    /* Assert contents */
    assert(tmp == 3);

    /* Dequeue all elements */
    for (cnt = 0; ring_buffer_dequeue(&ring_buffer, &tmp) > 0; cnt++) {
        /* Do something with buf... */
        assert(tmp == cnt);
        printf("Read: %d\n", tmp);
    }
    printf("\n===============\n");

    /* Add array */
    ring_buffer_queue_arr(&ring_buffer, "Hello, Ring Buffer!", 20);

    /* Is buffer empty? */
    assert(!ring_buffer_is_empty(&ring_buffer));

    /* Dequeue all elements */
    while (ring_buffer_dequeue(&ring_buffer, &tmp) > 0) {
        /* Print contents */
        printf("Read: %c\n", tmp);
    }

    /* Add new array */
    ring_buffer_queue_arr(&ring_buffer, "Hello again, Ring Buffer!", 26);

    /* Dequeue array in two parts */
    printf("Read:\n");
    cnt = ring_buffer_dequeue_arr(&ring_buffer, tmp_arr, 13);
    printf("%d\n", cnt);
    assert(cnt == 13);
    /* Add \0 termination before printing */
    tmp_arr[13] = '\0';
    printf("%s\n", tmp_arr);
    /* Dequeue remaining */
    cnt = ring_buffer_dequeue_arr(&ring_buffer, tmp_arr, 13);
    assert(cnt == 13);
    printf("%s", tmp_arr);

    printf("\n===============\n");

    /* Overfill buffer */
    for (i = 0; i < 1000; i++) {
        ring_buffer_queue(&ring_buffer, (i % 127));
    }

    /* Is buffer full? */
    if (ring_buffer_is_full(&ring_buffer)) {
        cnt = ring_buffer_num_items(&ring_buffer);
        printf("Buffer is full and contains %d bytes\n", cnt);
    }

    /* Dequeue all elements */
    while (ring_buffer_dequeue(&ring_buffer, &tmp) > 0) {
        /* Print contents */
        printf("Read: 0x%02x\n", tmp);
    }
}

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

    // 设置随机数种子，使用当前时间作为种子
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

int main(void)
{
    printf("TEST MAIN ENTRY\n");

    // snake_test();
    // tt_ringbuffer();
    // char key=waitForAnyKey();
    // tt_getchar();
    // tt_getch();
    // stack_test();
    // queue_test();
    // tt_hton();
    tt_dump_bytes();

    return 0;
}

#endif
