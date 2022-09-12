#include "stdio.h"
#include "stdlib.h"
#include "windows.h"
#include "conio.h"
#include "time.h"
#include "debug.h"

#define SNAKE_MAP_H 25
#define SNAKE_MAP_W 75
#define SNAKE_INIT_POS_X SNAKE_MAP_W / 12
#define SNAKE_INIT_POS_Y SNAKE_MAP_H / 2
#define SNAKE_MOVE_SPEED 100 //  ���ڿ������ƶ��ٶ�
#define SNAKE_CURSOR_SIZE 1     // [1-100]
#define SNAKE_INIT_LEN 4

enum snake_add_food {
    SNAKE_FOOD_NONE,
    SNAKE_FOOD_DOLLAR,
};

enum snake_direction {
    SNAKE_DIR_NONE,
    SNAKE_DIR_UP,
    SNAKE_DIR_DOWN,
    SNAKE_DIR_LEFT,
    SNAKE_DIR_RIGHT,
};

enum sanke_status {
    SNAKE_STATUS_NONE,
    SNAKE_STATUS_START,
    SNAKE_STATUS_PAUSE,
    SNAKE_STATUS_KILL_BY_SELF,
    SNAKE_STATUS_KILL_BY_WALL,
    SNAKE_STATUS_ESC,
    SNAKE_STATUS_END,
};

typedef struct {
    int x[SNAKE_MAP_H * SNAKE_MAP_W];   // ����x
    int y[SNAKE_MAP_H * SNAKE_MAP_W];   // ����y
    int len;                            // �߳�
    enum snake_direction dir;           // �߷���
    enum snake_status status;           // �ߵ�״̬
    BOOL iseat;                           // ���Ƿ�Ե�ʳ�1���Ե���
} snake_object_t;

void snake_init(void);
void snake_task(void);
static void snake_gotoxy(int x, int y);
static void snake_draw_map_init(void);
static void snake_param_init(void);
static BOOL snake_is_key_start(void);
static void snake_draw_snake(void);
static void snake_key_scan(void);
static void snake_move(void);
static BOOL snake_is_kill_by_wall(void);
static BOOL snake_is_invalid_food(int x, int y);
static void snake_add_food(void);


static int g_snake_map[SNAKE_MAP_H][SNAKE_MAP_W];   // ��ͼ���飬���ڴ���ߵ�ʳ��
static snake_object_t g_snake;

static void snake_gotoxy(int x, int y)
{
    if (x < 0 || y < 0) {
        return;
    }
    COORD position = { x, y };
    if (!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position)) {
        SNAKE_LOG_ERROR("SetConsoleCursorPosition fail");
    }
}

static void snake_draw_map_init(void)
{
    int i, j;
    for (i = 0; i < SNAKE_MAP_H; i++) {
        for (j = 0; j < SNAKE_MAP_W; j++) {
            if (i == 0 || j == 0 || i == SNAKE_MAP_H - 1 || j == SNAKE_MAP_W - 1) {
                SNAKE_PRINT("#");
            }
            else {
                SNAKE_PRINT(" ");
            }
        }
        SNAKE_PRINT("\n");
    }

    for (i = 0; i < SNAKE_MAP_W; i++) {
        for (j = 0; j < SNAKE_MAP_H; j++) {
            g_snake_map[i][j] = SNAKE_FOOD_NONE;
        }
    }
}

static void snake_param_init(void)
{
    int i;
    g_snake.x[0] = SNAKE_INIT_POS_X;
    g_snake.y[0] = SNAKE_INIT_POS_Y;
    for (i = 1; i < SNAKE_INIT_LEN; i++) {
        g_snake.x[i] = g_snake.x[0] - i;
        g_snake.y[i] = g_snake.y[0];
    }
    g_snake.len = SNAKE_INIT_LEN;
    g_snake.dir = SNAKE_DIR_RIGHT;
    g_snake.status = SNAKE_STATUS_NONE;
    g_snake.iseat = FALSE;
}

static BOOL snake_is_key_start(void)
{
    if (_kbhit() != 0) {
        int in;
        while (!_kbhit() == 0) {
            in = _getch();
        }
        switch (in) {
        case 'w':
        case 'W':
            g_snake.dir = SNAKE_DIR_UP;
            g_snake.status = SNAKE_STATUS_START;
            return TRUE;
        case 's':
        case 'S':
            g_snake.dir = SNAKE_DIR_DOWN;
            g_snake.status = SNAKE_STATUS_START;
            return TRUE;
        case 'd':
        case 'D':
            g_snake.dir = SNAKE_DIR_RIGHT;
            g_snake.status = SNAKE_STATUS_START;
            return TRUE;
        default:
            break;
        }
    }
    return FALSE;
}

void snake_init(void)
{
    CONSOLE_CURSOR_INFO cursor_info = { SNAKE_CURSOR_SIZE, FALSE };
    if (!SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info)) { // ���ع��
        SNAKE_LOG_ERROR("SetConsoleCursorInfo fail");
    }

    snake_gotoxy(0, 0); // ����ʼλ��
    snake_draw_map_init();
    snake_param_init();
    snake_draw_snake();
    srand((unsigned)time(NULL)); // ��ʼ�������������

    // ���ӳ�ʼʳ��
    g_snake_map[SNAKE_MAP_W / 2][SNAKE_MAP_H / 2] = SNAKE_FOOD_DOLLAR;
    snake_gotoxy(SNAKE_MAP_W / 2, SNAKE_MAP_H / 2);
    SNAKE_PRINT("$");

    snake_gotoxy(0, SNAKE_MAP_H);
    SNAKE_LOG_INFO("Snake init success, please press \033[40;33mw/s/d\033[0m to start!");
    snake_gotoxy(0, 0);
    while (!snake_is_key_start()) {

    }

}

static void snake_draw_snake(void)
{
    int i = 0;
    snake_gotoxy(g_snake.x[0], g_snake.y[0]);
    SNAKE_PRINT("@");
    for (i = 1; i < g_snake.len; i++) {
        snake_gotoxy(g_snake.x[i], g_snake.y[i]);
        SNAKE_PRINT("O");
    }
}

static void snake_key_scan(void)
{
    if (_kbhit() != 0) {
        int in;
        while (!_kbhit() == 0) {
            in = _getch();
        }
        switch (in) {
        case 'w':
        case 'W':
            if (g_snake.dir != SNAKE_DIR_DOWN) {
                g_snake.dir = SNAKE_DIR_UP;
                SNAKE_LOG_DEBUG("W");
            }
            break;
        case 's':
        case 'S':
            if (g_snake.dir != SNAKE_DIR_UP) {
                g_snake.dir = SNAKE_DIR_DOWN;
                SNAKE_LOG_DEBUG("S");
            }
            break;
        case 'a':
        case 'A':
            if (g_snake.dir != SNAKE_DIR_RIGHT) {
                g_snake.dir = SNAKE_DIR_LEFT;
                SNAKE_LOG_DEBUG("A");
            }
            break;
        case 'd':
        case 'D':
            if (g_snake.dir != SNAKE_DIR_LEFT) {
                g_snake.dir = SNAKE_DIR_RIGHT;
                SNAKE_LOG_DEBUG("D");
            }
            break;
        case 'p':
        case 'P':
            snake_gotoxy(0, SNAKE_MAP_H + 1);
            system("pause");
            snake_gotoxy(0, SNAKE_MAP_H + 1);
            SNAKE_PRINT("\33[2K\r");
            break;
        default:
            SNAKE_LOG_DEBUG("INVALID KEY");
            break;
        }
    }
}

static void snake_move(void)
{
    int i;
    snake_gotoxy(g_snake.x[g_snake.len - 1], g_snake.y[g_snake.len - 1]);
    SNAKE_PRINT(" ");
    if (g_snake.iseat) {
        g_snake.len++;
        g_snake.iseat = FALSE;
    }
    for (i = g_snake.len - 1; i > 0; i--) {
        g_snake.x[i] = g_snake.x[i - 1];
        g_snake.y[i] = g_snake.y[i - 1];
    }
    switch (g_snake.dir) {
    case SNAKE_DIR_UP:
        g_snake.y[0]--;
        SNAKE_LOG_DEBUG("U-");
        break;
    case SNAKE_DIR_DOWN:
        g_snake.y[0]++;
        SNAKE_LOG_DEBUG("D+");
        break;
    case SNAKE_DIR_LEFT:
        g_snake.x[0]--;
        SNAKE_LOG_DEBUG("L-");
        break;
    case SNAKE_DIR_RIGHT:
        g_snake.x[0]++;
        SNAKE_LOG_DEBUG("R+");
        break;
    default:
        SNAKE_LOG_DEBUG("INVALID DIR");
        break;
    }

}

static BOOL snake_is_kill_by_wall(void)
{
    if (g_snake.x[0] == 0 || g_snake.y[0] == 0 || g_snake.x[0] == SNAKE_MAP_W ||
        g_snake.y[0] == SNAKE_MAP_H) {
        return TRUE;
    }
    return FALSE;
}

static BOOL snake_is_invalid_food(int x, int y)
{
    int i;
    if (g_snake_map[x][y] == SNAKE_FOOD_DOLLAR) {
        return TRUE;
    }
    for (i = 0; i < g_snake.len; i++) {
        if (g_snake.x[i] == x && g_snake.y[i] == y) {
            return TRUE;
        }
    }
    return FALSE;
}

static void snake_add_food(void)
{
    int i, j;
    do {
        i = rand() % (SNAKE_MAP_W - 2) + 1; // ��Χ��1~SNAKE_MAP_W-2
        j = rand() % (SNAKE_MAP_H - 2) + 1; // ��Χ��1~SANKE_MAP_H-2
        SNAKE_LOG_DEBUG("i=%d, j=%d", i, j);
    } while (snake_is_invalid_food(i, j));
    g_snake_map[i][j] = SNAKE_FOOD_DOLLAR;
    snake_gotoxy(i, j);
    SNAKE_PRINT("$");
}

void snake_task(void)
{
    if (snake_is_kill_by_wall()) {
        system("cls");
        snake_gotoxy(SNAKE_MAP_W / 2, SNAKE_MAP_H / 2);
        SNAKE_PRINT("GAME OVER!");
        _getch();
        exit(-1);
    }
    snake_draw_snake();
    Sleep(SNAKE_MOVE_SPEED);
    snake_key_scan();
    snake_move();
    if (g_snake_map[g_snake.x[0]][g_snake.y[0]] == SNAKE_FOOD_DOLLAR) {
        snake_add_food();
        g_snake_map[g_snake.x[0]][g_snake.y[0]] = SNAKE_FOOD_NONE;
        g_snake.iseat = TRUE;
    }
}

int main(int argc, char *argv[])
{
    snake_init();
    while (1) {
        snake_task();
    }
    return 0;
}