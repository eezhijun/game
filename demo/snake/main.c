#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "time.h"
#include "debug.h"
#include "log.h"

#define SNAKE_MAP_H 25
#define SNAKE_MAP_W 75
#define SNAKE_INIT_POS_X SNAKE_MAP_W / 12
#define SNAKE_INIT_POS_Y SNAKE_MAP_H / 2
#define SNAKE_MOVE_SPEED 100 //  用于控制蛇移动速度
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

// enum sanke_status {
//     SNAKE_STATUS_NONE,
//     SNAKE_STATUS_START,
//     SNAKE_STATUS_PAUSE,
//     SNAKE_STATUS_KILL_BY_SELF,
//     SNAKE_STATUS_KILL_BY_WALL,
//     SNAKE_STATUS_ESC,
//     SNAKE_STATUS_END,
// };

typedef struct {
    int x[SNAKE_MAP_H * SNAKE_MAP_W];   // 蛇身x
    int y[SNAKE_MAP_H * SNAKE_MAP_W];   // 蛇身y
    int len;                            // 蛇长
    enum snake_direction dir;           // 蛇方向
    // enum snake_status status;           // 蛇的状态
    bool iseat;                           // 蛇是否吃到食物，1：吃到了
} snake_object_t;
/*
void snake_init(void);
void snake_task(void);
static void snake_gotoxy(int x, int y);
static void snake_draw_map_init(void);
static void snake_param_init(void);
static bool snake_is_key_start(void);
static void snake_draw_snake(void);
static void snake_key_scan(void);
static void snake_move(void);
static bool snake_is_kill_by_wall(void);
static bool snake_is_invalid_food(int x, int y);
static void snake_add_food(void);


static int g_snake_map[SNAKE_MAP_H][SNAKE_MAP_W];   // 地图数组，用于存放蛇的食物
static snake_object_t g_snake;

static void snake_gotoxy(int x, int y)
{
    if (x < 0 || y < 0) {
        return;
    }
    COORD position = {x, y};
    if (!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position)) {
        log_error("SetConsoleCursorPosition fail");
    }
}

static void snake_draw_map_init(void)
{
    int i, j;
    for (i = 0; i < SNAKE_MAP_H; i++) {
        for (j = 0; j < SNAKE_MAP_W; j++) {
            if(i == 0 || j == 0 || i == SNAKE_MAP_H - 1 || j == SNAKE_MAP_W - 1) {
                SNAKE_PRINT("#");
            } else {
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

static bool snake_is_key_start(void)
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
    CONSOLE_CURSOR_INFO cursor_info = {SNAKE_CURSOR_SIZE, FALSE};
    if (!SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info)) { // 隐藏光标
        log_error("SetConsoleCursorInfo fail");
    }

    snake_gotoxy(0, 0); // 光标初始位置
    snake_draw_map_init();
    snake_param_init();
    snake_draw_snake();
    srand((unsigned)time(NULL)); // 初始化随机数发生器

    // 添加初始食物
    g_snake_map[SNAKE_MAP_W / 2][SNAKE_MAP_H / 2] = SNAKE_FOOD_DOLLAR;
    snake_gotoxy(SNAKE_MAP_W / 2, SNAKE_MAP_H / 2);
    SNAKE_PRINT("$");

    snake_gotoxy(0, SNAKE_MAP_H);
    log_info("Snake init success, please press a/s/d/w to start!");
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
                    log_trace("W");
                }
                break;
            case 's':
            case 'S':
                if (g_snake.dir != SNAKE_DIR_UP) {
                    g_snake.dir = SNAKE_DIR_DOWN;
                    log_trace("S");
                }
                break;
            case 'a':
            case 'A':
                if (g_snake.dir != SNAKE_DIR_RIGHT) {
                    g_snake.dir = SNAKE_DIR_LEFT;
                    log_trace("A");
                }
                break;
            case 'd':
            case 'D':
                if (g_snake.dir != SNAKE_DIR_LEFT) {
                    g_snake.dir = SNAKE_DIR_RIGHT;
                    log_trace("D");
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
                log_trace("INVALID KEY");
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
            log_trace("U-");
            break;
        case SNAKE_DIR_DOWN:
            g_snake.y[0]++;
            log_trace("D+");
            break;
        case SNAKE_DIR_LEFT:
            g_snake.x[0]--;
            log_trace("L-");
            break;
        case SNAKE_DIR_RIGHT:
            g_snake.x[0]++;
            log_trace("R+");
            break;
        default:
            log_trace("INVALID DIR");
            break;
    }

}

static bool snake_is_kill_by_wall(void)
{
    if (g_snake.x[0] == 0 || g_snake.y[0] == 0 || g_snake.x[0] == SNAKE_MAP_W ||
        g_snake.y[0] == SNAKE_MAP_H) {
        return TRUE;
    }
    return FALSE;
}

static bool snake_is_invalid_food(int x, int y)
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
        i = rand() % (SNAKE_MAP_W - 2) + 1; // 范围：1~SNAKE_MAP_W-2
        j = rand() % (SNAKE_MAP_H - 2) + 1; // 范围：1~SANKE_MAP_H-2
        log_trace("i=%d, j=%d", i, j);
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
*/
#ifdef SNAKE_DEMO
int main(int argc, char *argv[])
{
    log_set_level(LOG_INFO);
    // snake_init();
    while (1) {
        // snake_task();
    }
    return 0;
}
#endif
