/**
 * @file snake.c
 * @author eehongzhijun (eehongzhijun@outlook.com)
 * @brief 
 * @version 0.0.1
 * @date 2023-08-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifdef SNAKE_DEMO

#include "ctype.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "stdbool.h"
#include "string.h"
#include "assert.h"
#include "unistd.h"
#include "stdint.h"

#include "utils.h"

#define SNAKE_ARRAY_SIZE 310

#define SNAKE_CONSOLE_WIDTH 80
#define SNAKE_CONSOLE_HEIGHT 24

/* snake init */
#define SNAKE_LEN_INIT 4
#define SNAKE_LEN_MAX 30
#define SNAKE_POS_X 8
#define SNAKE_POS_Y 12
#define SNAKE_FOOD_POS_X 64
#define SNAKE_FOOD_POS_Y 12

#define SNAKE_SPEED_CLOCK(x) \
    clock() + CLOCKS_PER_SEC - (x) * (CLOCKS_PER_SEC / 10)

/* score file array size */
#define SNAKE_SCORE_FILE_ROW_LEN 6
#define SNAKE_SCORE_FILE_COL_LEN 30

/* score file array str offset*/
#define SNAKE_SCORE_FILE_SCORE_OFFSET 7
#define SNAKE_SCORE_FILE_NAME_OFFSET 17

/* score file array score len && name len*/
#define SNAKE_SCORE_FILE_SCORE_LEN 5
#define SNAKE_SCORE_FILE_NAME_LEN 10

#define SNAKE_HEAD 'X'
#define SNAKE_BODY '#'
#define SNAKE_WALL '#'
#define SNAKE_FOOD '*'
#define SNAKE_BLANK ' '

static const char *tips_str[] = {
    "Tips",
    "",
    "Use the following arrow keys to direct the snake to the food: ",
    "Right Arrow",
    "Left Arrow",
    "Top Arrow",
    "Bottom Arrow",
    "",
    "P & Esc pauses the game.",
    "Press any key to continue...",
};

static const char *info_bar_str[] = {
    "Score: ",
    "Speed: ",
    "Coder  : HZJ",
    "Version: 0.5",
};

// Ascii art reference:
// http://www.chris.com/ascii/index.php?art=animals/reptiles/snakes
static const char *wel_art_str[] = {
    "\n",
    "\t\t    _________         _________                 \n",
    "\t\t   /         \\       /         \\              \n",
    "\t\t  /  /~~~~~\\  \\     /  /~~~~~\\  \\           \n",
    "\t\t  |  |     |  |     |  |     |  |               \n",
    "\t\t  |  |     |  |     |  |     |  |               \n",
    "\t\t  |  |     |  |     |  |     |  |         /     \n",
    "\t\t  |  |     |  |     |  |     |  |       //	     \n",
    "\t\t (o  o)    \\  \\_____/  /     \\  \\_____/ /   \n",
    "\t\t  \\__/      \\         /       \\        /     \n",
    "\t\t    |        ~~~~~~~~~         ~~~~~~~~         \n",
    "\t\t    ^                                           \n",
    "\t      Welcome To The Snake Game!                  \n",
    "\t      Press Any Key To Continue...                \n",
    "\n",
};

// http://www.network-science.de/ascii/ <- Ascii Art Gen
static const char *game_over_scr_str[] = {
    "..######......###....##.....##.########\n",
    ".##....##....##.##...###...###.##......\n",
    ".##.........##...##..####.####.##......\n",
    ".##...####.##.....##.##.###.##.######..\n",
    ".##....##..#########.##.....##.##......\n",
    ".##....##..##.....##.##.....##.##......\n",
    "..######...##.....##.##.....##.########\n",
    "..#######..##.....##.########.########..####\n",
    ".##.....##.##.....##.##.......##.....##.####\n",
    ".##.....##.##.....##.##.......##.....##.####\n",
    ".##.....##.##.....##.######...########...##.\n",
    ".##.....##..##...##..##.......##...##.......\n",
    ".##.....##...##.##...##.......##....##..####\n",
    "..#######.....###....########.##.....##.####\n",
};

static const char *you_win_scr_str[] = {
    ".##....##..#######..##.....##....##......##.####.##....##.####\n",
    "..##..##..##.....##.##.....##....##..##..##..##..###...##.####\n",
    "...####...##.....##.##.....##....##..##..##..##..####..##.####\n",
    "....##....##.....##.##.....##....##..##..##..##..##.##.##..##.\n",
    "....##....##.....##.##.....##....##..##..##..##..##..####.....\n",
    "....##....##.....##.##.....##....##..##..##..##..##...###.####\n",
    "....##.....#######...#######......###..###..####.##....##.####\n",
};

static char score_file_arr[SNAKE_SCORE_FILE_ROW_LEN][SNAKE_SCORE_FILE_COL_LEN] = {
    { "Rank   Score     Name      \n" }, { "1      0         EMPTY     \n" },
    { "2      0         EMPTY     \n" }, { "3      0         EMPTY     \n" },
    { "4      0         EMPTY     \n" }, { "5      0         EMPTY     \n" },
};

typedef struct {
    uint8_t index;
    char str[255];
} snake_menu_t;

static const snake_menu_t menu[] = {
    {.index = 0, .str = {"New Game"}},
    {.index = 1, .str = {"Tips"}},
    {.index = 2, .str = {"Exit"}},
    {.index = 3, .str = {"Test"}},
};
typedef enum {
    SNAKE_DIR_NONE,
    SNAKE_DIR_UP = UP_ARROW,
    SNAKE_DIR_DOWN = DOWN_ARROW,
    SNAKE_DIR_LEFT = LEFT_ARROW,
    SNAKE_DIR_RIGHT = RIGHT_ARROW,
} SNAKE_DIR;

typedef enum {
    SNAKE_GAME_OVER_NONE,
    SNAKE_GAME_OVER_BY_WALL,
    SNAKE_GAME_OVER_BY_SNAKE,
    SNAKE_GAME_OVER_BY_YOU_WIN,
} SNAKE_GAME_OVER;

typedef struct {
    /* snake x y*/
    int32_t sx[SNAKE_LEN_MAX];
    int32_t sy[SNAKE_LEN_MAX];

    /* snake len */
    uint16_t slen;

    /* food x y*/
    int32_t fx;
    int32_t fy;

    /* snake move speed */
    uint16_t speed;

    /* snake win score */
    uint16_t score;

    /* game is over */
    SNAKE_GAME_OVER game_over;

    /* snake dir */
    SNAKE_DIR dir;
} snake_object_t;

static snake_object_t g_snake = { .sx = { 0 },
                                  .sy = { 0 },
                                  .slen = 0,
                                  .fx = 0,
                                  .fy = 0,
                                  .speed = 0,
                                  .score = 0,
                                  .game_over = SNAKE_GAME_OVER_NONE,
                                  .dir = SNAKE_DIR_NONE };

snake_object_t *snake_get_snake_object(void)
{
    return &g_snake;
}

void snake_create_high_scores(void)
{
    FILE *fp = NULL;
    uint32_t i;

    fp = fopen("highscores", "w+");
    if (fp == NULL) {
        perror("failed to create highscores");
        exit(EXIT_FAILURE);
    }

    /*
        Rank   Score     Name      
        1      0         EMPTY     
        2      0         EMPTY     
        3      0         EMPTY     
        4      0         EMPTY     
        5      0         EMPTY     
    */

    for (i = 0; i < ARRAY_SIZE(score_file_arr); i++) {
        fprintf(fp, "%s", score_file_arr[i]);
    }
    fclose(fp);
}

FILE *snake_open_high_scores(FILE *fp)
{
    char buf[30] = { 0 };
    uint32_t i = 0;

    fp = fopen("highscores", "r");
    if (fp == NULL) {
        snake_create_high_scores();
        fp = fopen("highscores", "r");
        if (fp == NULL) {
            perror("failed to pen highscores");
            exit(EXIT_FAILURE);
        }
    } else {
        while (fgets(buf, ARRAY_SIZE(buf), fp) != NULL) {
            memcpy(score_file_arr[i], buf, ARRAY_SIZE(buf));
            i++;
        }
    }
    return fp;
}

int snake_get_lowest_score(void)
{
    FILE *fp = NULL;
    uint16_t lowest_score = 0;

    chdir("demo/snake");
    fp = snake_open_high_scores(fp);
    fclose(fp);

    lowest_score = string2int(&score_file_arr[SNAKE_SCORE_FILE_ROW_LEN - 1]
                                             [SNAKE_SCORE_FILE_SCORE_OFFSET]);

    return lowest_score;
}

void snake_input_score(snake_object_t *snake)
{
    FILE *fp = NULL;
    char user_name[SNAKE_SCORE_FILE_NAME_LEN] = { 0 };
    char score_str[SNAKE_SCORE_FILE_SCORE_LEN] = { 0 };
    uint16_t score_int = 0;
    uint32_t i, j;

    clrscr();
    gotoxy(10, 5);
    printf("Only maintain the 5 top score rankings");
    gotoxy(10, 6);
    printf("Please enter your name: ");
    fgets(user_name, ARRAY_SIZE(user_name), stdin);

    /*
        Rank   Score     Name      
        1      0         EMPTY     
        2      0         EMPTY     
        3      0         EMPTY     
        4      0         EMPTY     
        5      0         EMPTY     
    */

    /* only maintain the 5 top score rankings */
    for (i = 1; i < ARRAY_SIZE(score_file_arr); i++) {
        score_int = (uint16_t)string2int(
            &score_file_arr[i][SNAKE_SCORE_FILE_SCORE_OFFSET]);
        if (snake->score > score_int) {
            for (j = ARRAY_SIZE(score_file_arr) - 1; j > i; j--) {
                memcpy(&score_file_arr[j][SNAKE_SCORE_FILE_SCORE_OFFSET],
                       &score_file_arr[j - 1][SNAKE_SCORE_FILE_SCORE_OFFSET],
                       SNAKE_SCORE_FILE_COL_LEN -
                           SNAKE_SCORE_FILE_SCORE_OFFSET);
            }

            sprintf(score_str, "%d", snake->score);
            strncpy(&score_file_arr[i][SNAKE_SCORE_FILE_SCORE_OFFSET],
                    score_str, strlen(score_str));
            memset(&score_file_arr[i][SNAKE_SCORE_FILE_NAME_OFFSET], ' ',
                   SNAKE_SCORE_FILE_NAME_LEN);
            strncpy(&score_file_arr[i][SNAKE_SCORE_FILE_NAME_OFFSET], user_name,
                    strlen(user_name) - 1);
            break;
        }
    }

    fp = fopen("highscores", "w+");
    for (i = 0; i < ARRAY_SIZE(score_file_arr); i++) {
        fprintf(fp, "%s", score_file_arr[i]);
    }
    fclose(fp);
}

void snake_you_win_screen(void)
{
    int32_t x = 6, y = 7;
    int32_t i;

    for (i = 0; i < ARRAY_SIZE(you_win_scr_str); i++) {
        gotoxy(x, y);
        y++;
        printf("%s", you_win_scr_str[i]);
    }

    wait_4_key();
    clrscr();
}

void snake_game_overs_screen(void)
{
    int32_t x = 17, y = 3;
    int32_t i;

    for (i = 0; i < ARRAY_SIZE(game_over_scr_str); i++) {
        gotoxy(x, y);
        y++;
        printf("%s", game_over_scr_str[i]);
    }
    wait_4_key();
    clrscr();
}

void snake_refresh_info_bar(snake_object_t *snake)
{
    gotoxy(5, SNAKE_CONSOLE_HEIGHT + 2);
    printf("%s%d", info_bar_str[0], snake->score);

    gotoxy(5, SNAKE_CONSOLE_HEIGHT + 3);
    printf("%s%d", info_bar_str[1], snake->speed);

    gotoxy(SNAKE_CONSOLE_WIDTH - 15, SNAKE_CONSOLE_HEIGHT + 2);
    printf("%s", info_bar_str[2]);

    gotoxy(SNAKE_CONSOLE_WIDTH - 15, SNAKE_CONSOLE_HEIGHT + 3);
    printf("%s", info_bar_str[3]);
    gotoxy(0, 0);
}

void snake_pause_menu(void)
{
    gotoxy(SNAKE_CONSOLE_WIDTH / 2, SNAKE_CONSOLE_HEIGHT / 2);
    printf("PAUSE");

    wait_4_key();
    gotoxy(SNAKE_CONSOLE_WIDTH / 2, SNAKE_CONSOLE_HEIGHT / 2);
    printf("            ");
}

void snake_enviroment_init(int console_width, int console_height)
{
    int32_t x = 1, y = 1;

    clrscr();

    for (; x < console_width; x++) {
        gotoxy(x, y);
        printf("%c", SNAKE_WALL);

        gotoxy(x, console_height);
        printf("%c", SNAKE_WALL);
    }

    x = 1;
    for (; y < console_height + 1; y++) {
        gotoxy(x, y);
        printf("%c", SNAKE_WALL);

        gotoxy(console_width, y);
        printf("%c", SNAKE_WALL);
    }
}

void snake_len_init(snake_object_t *snake)
{
    snake->slen = SNAKE_LEN_INIT;
}

void snake_body_init(snake_object_t *snake)
{
    int32_t i;
    int32_t x, y;

    /*
        Make sure the snake head is at index 0 of the array
        3 2 1 0
        # # # X
    */
    snake->sx[0] = SNAKE_POS_X;
    snake->sy[0] = SNAKE_POS_Y;

    x = snake->sx[0];
    y = snake->sy[0];

    for (i = 1; i < snake->slen; i++) {
        snake->sx[i] = x - i;
        snake->sy[i] = y;
    }

    gotoxy(snake->sx[0], snake->sy[0]);
    printf("%c", SNAKE_HEAD);

    for (i = 1; i < snake->slen; i++) {
        gotoxy(snake->sx[i], snake->sy[i]);
        printf("%c", SNAKE_BODY);
    }
    gotoxy(0, 0);
}

void snake_food_init(snake_object_t *snake)
{
    snake->fx = SNAKE_FOOD_POS_X;
    snake->fy = SNAKE_FOOD_POS_Y;
    gotoxy(snake->fx, snake->fy);
    printf("%c", SNAKE_FOOD);
}

void snake_speed_init(snake_object_t *snake)
{
    uint16_t speed;
    clrscr();

    do {
        gotoxy(10, 5);
        printf("Select the game speed between 1 and 9.");
        speed = wait_4_key() - '0';
    } while (speed < 1 || speed > 9);
    snake->speed = speed;
}

void snake_move(snake_object_t *snake)
{
    // clear snake tail cursor
    gotoxy(snake->sx[snake->slen - 1], snake->sy[snake->slen - 1]);
    printf("%c", SNAKE_BLANK);

    // update snake pos
    for (int i = snake->slen - 1; i > 0; i--) {
        snake->sx[i] = snake->sx[i - 1];
        snake->sy[i] = snake->sy[i - 1];
    }
    switch (snake->dir) {
    case SNAKE_DIR_UP:
        snake->sy[0]--;
        break;
    case SNAKE_DIR_DOWN:
        snake->sy[0]++;
        break;
    case SNAKE_DIR_RIGHT:
        snake->sx[0]++;
        break;
    case SNAKE_DIR_LEFT:
        snake->sx[0]--;
        break;
    default:
        break;
    }

    // update snake head cursor
    gotoxy(snake->sx[0], snake->sy[0]);
    printf("%c", SNAKE_HEAD);

    // update snake body cursor
    for (int i = 1; i < snake->slen; i++) {
        gotoxy(snake->sx[i], snake->sy[i]);
        printf("%c", SNAKE_BODY);
    }

    // update cursor origin
    gotoxy(0, 0);
}

bool snake_is_eat_food(snake_object_t *snake)
{
    if (snake->sx[0] == snake->fx && snake->sy[0] == snake->fy) {
        snake->fx = 0;
        snake->fy = 0;
        BELL();
        return true; // eat
    }
    return false;
}

void snake_food(snake_object_t *snake)
{
    /* 2 ~ 79 */
    srand(time(NULL));
    snake->fx = rand() % (SNAKE_CONSOLE_WIDTH - 2) + 2;

    /* 2 ~ 23 */
    srand(time(NULL));
    snake->fy = rand() % (SNAKE_CONSOLE_HEIGHT - 2) + 2;
    gotoxy(snake->fx, snake->fy);
    printf("%c", SNAKE_FOOD);
}

bool snake_is_kill_by_wall(snake_object_t *snake)
{
    if (snake->sx[0] == 1 || snake->sy[0] == 1 ||
        snake->sx[0] == SNAKE_CONSOLE_WIDTH ||
        snake->sy[0] == SNAKE_CONSOLE_HEIGHT) {
        return true;
    }
    return false;
}

SNAKE_DIR snake_check_keys_pressed(SNAKE_DIR dir)
{
    char press;

    if (kbhit()) {
        press = getch();
        if (dir != press) {
            if (press == DOWN_ARROW && dir != UP_ARROW) {
                dir = press;
            } else if (press == UP_ARROW && dir != DOWN_ARROW) {
                dir = press;
            } else if (press == LEFT_ARROW && dir != RIGHT_ARROW) {
                dir = press;
            } else if (press == RIGHT_ARROW && dir != LEFT_ARROW) {
                dir = press;
            } else if (press == EXIT_BUTTON || press == PAUSE_BUTTON) {
                snake_pause_menu();
            }
        }
    }
    return dir;
}

void snake_start_game(snake_object_t *snake)
{
    snake->game_over = SNAKE_GAME_OVER_NONE;

    int32_t wait = SNAKE_SPEED_CLOCK(snake->speed);
    // first wait user choose to move snake
    char key;
    do {
        key = wait_4_key();
        switch (key) {
        case SNAKE_DIR_UP:
            snake->dir = SNAKE_DIR_UP;
            break;
        case SNAKE_DIR_DOWN:
            snake->dir = SNAKE_DIR_DOWN;
            break;
        case SNAKE_DIR_RIGHT:
            snake->dir = SNAKE_DIR_RIGHT;
            break;
        case SNAKE_DIR_LEFT:
            break;
        default:
            break;
        }
        if (key == SNAKE_DIR_UP || key == SNAKE_DIR_DOWN ||
            key == SNAKE_DIR_RIGHT)
            break;
    } while (1);

    // hide_cursor();
    do {
        snake->dir = snake_check_keys_pressed(snake->dir);
        if (clock() > wait) {
            snake_move(snake);
            if (snake_is_eat_food(snake)) {
                snake->slen++;
                snake->score += snake->speed;
                snake_food(snake);
                snake_refresh_info_bar(snake);
            }
            wait = SNAKE_SPEED_CLOCK(snake->speed);
        }

        if (snake_is_kill_by_wall(snake)) {
            snake->game_over = SNAKE_GAME_OVER_BY_WALL;
        }

    } while (!snake->game_over);
    // show_cursor();
}

void snake_game_over(snake_object_t *snake)
{
    switch (snake->game_over) {
    case SNAKE_GAME_OVER_BY_WALL:
    case SNAKE_GAME_OVER_BY_SNAKE:
        BELL();
        BELL();
        snake_game_overs_screen();
        break;
    case SNAKE_GAME_OVER_BY_YOU_WIN:
        snake_you_win_screen();
        break;
    default:
        assert(0);
        break;
    }
    snake->game_over = SNAKE_GAME_OVER_NONE;

    if (snake->score > snake_get_lowest_score()) {
        snake_input_score(snake);
    }

    snake->score = 0;
}

void snake_load_game(void)
{
    snake_object_t *snake = snake_get_snake_object();
    snake_speed_init(snake);
    snake_enviroment_init(SNAKE_CONSOLE_WIDTH, SNAKE_CONSOLE_HEIGHT);
    snake_len_init(snake);
    snake_body_init(snake);
    snake_food_init(snake);
    snake_refresh_info_bar(snake);
    snake_start_game(snake);
    snake_game_over(snake);
}

void snake_welcome_art(void)
{
    int i;

    clrscr();
    for (i = 0; i < ARRAY_SIZE(wel_art_str); i++) {
        printf("%s", wel_art_str[i]);
    }
    wait_4_key();
}

void snake_tips(void)
{
    int x = 10, y = 5;
    int i;

    clrscr();
    for (i = 0; i < ARRAY_SIZE(tips_str); i++) {
        gotoxy(x, y);
        y++;
        printf("%s", tips_str[i]);
    }
    wait_4_key();
}

void snake_exit_yn(void)
{
    char press;

    clrscr();
    gotoxy(9, 8);
    printf("Are you sure you want to exit Y/N\n");

    do {
        press = wait_4_key();
        press = tolower(press);
    } while (!(press == 'y' || press == 'n'));

    printf("%c", press);

    clrscr();
    exit(1);
}

int32_t snake_main_meun(void)
{
#define MAIN_MENU_INIT_X 10
#define MAIN_MENU_INIT_Y 5
    int32_t x = MAIN_MENU_INIT_X, y = MAIN_MENU_INIT_Y;
    int32_t ystart = y;
    char key;
    int32_t selected = 0;

    clrscr();
    gotoxy(x, y);
    y++;
    for (int i = 0; i < ARRAY_SIZE(menu); i++) {
        printf("%s\n", menu[i].str);
        gotoxy(x, y);
        y++;
    }

    x -= 2;
    gotoxy(x, ystart);
    printf(">");

    do {
        key = wait_4_key();
        switch (key) {
        case UP_ARROW:
            gotoxy(x, ystart + selected);
            printf(" "); // clear ">" sign

            if (ystart >= ystart + selected)
                selected = y - ystart - 2;
            else
                selected--;
            gotoxy(x, ystart + selected);
            printf(">");
            break;
        case DOWN_ARROW:
            gotoxy(x, ystart + selected);
            printf(" "); // clear ">" sign

            if (selected + 2 >= y - ystart)
                selected = 0;
            else
                selected++;
            gotoxy(x, ystart + selected);
            printf(">");
            break;
        default:
            break;
        }
    } while (key != ENTER_KEY);

    return selected;
}

int main(void)
{
    snake_welcome_art();

    do {
        switch (snake_main_meun()) {
        case 0:
            snake_load_game();
            break;
        case 1:
            snake_tips();
            break;
        case 2:
            snake_exit_yn();
            break;
        case 3:
            break;
        default:
            break;
        }
    } while (1);

    return 0;
}
#endif
