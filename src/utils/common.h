#ifndef _COMMON_H_
#define _COMMON_H_


#define UNUSED(x) (void)(x)

/* max */
#define MAX(a, b) \
({ \
    typeof(a) _a = (a); \
    typeof(b) _b = (b); \
    _a > _b ? _a : _b; \
})

#define MIN(a, b) \
({ \
    typeof(a) _a = (a); \
    typeof(b) _b = (b); \
    _a < _b ? _a : _b; \
})

int max(int a, int b);
int min(int a, int b);

/* swap */
#define SWAP(type, lhs, rhs) \
do \
{ \
    type temp = lhs; \
    lhs = rhs; \
    rhs = temp; \
} while(0)

void swap(void* lhs, void* rhs, size_t size);
void swap_by_temp(int* lhs, int *rhs);
void swap_by_sum(int* lhs, int* rhs);
void swap_by_xor(int *lhs, int *rhs);

/* keyboard */
//Controls (arrow keys for Ubuntu)
#define UP_ARROW (char)'A' //Originally I used constants but borland started giving me errors, so I changed to #define - I do realize that is not the best way.
#define LEFT_ARROW (char)'D'
#define RIGHT_ARROW (char)'C'
#define DOWN_ARROW (char)'B'

#define ENTER_KEY 10

void gotoxy(int x,int y);
char getch(void);
void clrscr(void);
int kbhit(void);
char waitForAnyKey(void);

/* sort */
void bubble_sort (int arr[], int len);
int cmp(const void* pa, const void* pb);
void print_array(int arr[], int len);

#endif
