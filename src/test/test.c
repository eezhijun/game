#ifdef TEST_DEMO

#include "stdio.h"
#include "unistd.h"
#include "assert.h"

#include "ringbuffer.h"

#include "common.h"


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
    for(i = 0; i < 100; i++) {
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
    for(cnt = 0; ring_buffer_dequeue(&ring_buffer, &tmp) > 0; cnt++) {
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
    while(ring_buffer_dequeue(&ring_buffer, &tmp) > 0) {
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
    for(i = 0; i < 1000; i++) {
        ring_buffer_queue(&ring_buffer, (i % 127));
    }

    /* Is buffer full? */
    if(ring_buffer_is_full(&ring_buffer)) {
        cnt = ring_buffer_num_items(&ring_buffer);
        printf("Buffer is full and contains %d bytes\n", cnt);
    }

    /* Dequeue all elements */
    while(ring_buffer_dequeue(&ring_buffer, &tmp) > 0) {
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
    if (getch() == '\033') // if the first value is esc
    {
        getch(); // skip the [
        switch(getch()) // the real value
        {
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
        }
    }
}


int main(void)
{
    printf("TEST MAIN ENTRY\n");

    // snake_test();
    // tt_ringbuffer();
    // char key=waitForAnyKey();
    // tt_getchar();
    // tt_getch();
    stack_test();
    queue_test();

    return 0;
}

#endif
