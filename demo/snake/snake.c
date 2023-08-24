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

#define SNAKE_HEAD 'X'
#define SNAKE_BODY '#'
#define SNAKE_WALL '#'
#define SNAKE_FOOD '*'
#define SNAKE_BLANK ' '

static const char *str_menu[] = {
    "New Game", "High Scores", "Tips", "Exit", "Test",
};

static const char *str_tips[] = {
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

static const char *str_info_bar[] = {
    "Score: ",
    "Speed: ",
    "Coder  : HZJ",
    "Version: 0.5",
};

// Ascii art reference:
// http://www.chris.com/ascii/index.php?art=animals/reptiles/snakes
static const char *str_wel_art[] = {
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
static const char *str_game_voer_scr[] = {
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

static const char *str_you_win_scr[] = {
    ".##....##..#######..##.....##....##......##.####.##....##.####\n",
    "..##..##..##.....##.##.....##....##..##..##..##..###...##.####\n",
    "...####...##.....##.##.....##....##..##..##..##..####..##.####\n",
    "....##....##.....##.##.....##....##..##..##..##..##.##.##..##.\n",
    "....##....##.....##.##.....##....##..##..##..##..##..####.....\n",
    "....##....##.....##.##.....##....##..##..##..##..##...###.####\n",
    "....##.....#######...#######......###..###..####.##....##.####\n",
};

typedef enum {
    SNAKE_MENU0,
    SNAKE_MENU1,
    SNAKE_MENU2,
    SNAKE_MENU3,
} SNAKE_MENU;

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
    int sx[SNAKE_LEN_MAX];
    int sy[SNAKE_LEN_MAX];
    int slen;
    int fx;
    int fy;
    int speed;
    int score;
    SNAKE_DIR dir;
} snake_object_t;

static snake_object_t g_snake = { .sx = { 0 },
                                  .sy = { 0 },
                                  .slen = 0,
                                  .fx = 0,
                                  .fy = 0,
                                  .dir = SNAKE_DIR_NONE };

snake_object_t *snake_get_snake_object(void)
{
    return &g_snake;
}

void pause_menu(void)
{
    gotoxy(28, 23);
    printf("**Paused**");

    wait_4_key();
    gotoxy(28, 23);
    printf("            ");
}

void snake_create_high_scores(void)
{
    FILE *fp;
    int i;

    fp = fopen("highscores", "w+");

    if (fp == NULL) {
        perror("failed to create highscores");
        exit(0);
    }

    for (i = 0; i < 5; i++) {
        fprintf(fp, "%d", i + 1);
        fprintf(fp, "%s", "\t0\t\t\tEMPTY\n");
    }

    fclose(fp);
}

int snake_get_lowest_score(void)
{
    FILE *fp;
    char str[128];
    int lowest_score = 0;
    int i;
    int len;

    chdir("demo/snake");
    fp = fopen("highscores", "r");
    if (fp == NULL) {
        perror("unable to open ./demo/sanke/highscores");
        snake_create_high_scores();
        fp = fopen("highscores", "r");
        if (fp == NULL) {
            perror("unable to open ./demo/sanke/highscores");
            exit(EXIT_FAILURE);
        }
    }
    /* Read data from fp file and save in str */
    while (!feof(fp)) {
        fgets(str, 126, fp);
    }
    fclose(fp);

    printf("%s\n", str);
    wait_4_key();

    i = 0;
    while (str[2 + i] != '\t') {
        i++;
    }

    len = i;
    for (i = 0; i < len; i++) {
        lowest_score += ((int)str[2 + i] - 48) * pow(10, len - i - 1);
    }

    return lowest_score;
}

void snake_input_score(snake_object_t *snake)
{
    FILE *fp;
    FILE *file;
    char str[20];
    int fScore;
    int i, s, y;
    int intLength;
    int scores[5];
    int x;
    char highScoreName[20];
    char highScoreNames[5][20];

    char name[20];

    int entered = 0;

    clrscr();

    if ((fp = fopen("highscores", "r")) == NULL) {
        snake_create_high_scores();
        if ((fp = fopen("highscores", "r")) == NULL) {
            exit(1);
        }
    }
    gotoxy(10, 5);
    printf("Your Score made it into the top 5!!!");
    gotoxy(10, 6);
    printf("Please enter your name: ");
    fgets(name, 20, stdin);

    x = 0;
    while (!feof(fp)) {
        fgets(str, 126, fp);

        i = 0;

        while (str[2 + i] != '\t') {
            i++;
        }

        s = i;
        intLength = i;
        i = 0;
        while (str[5 + s] != '\n') {
            highScoreName[i] = str[5 + s];
            s++;
            i++;
        }

        fScore = 0;
        for (i = 0; i < intLength; i++) {
            fScore =
                fScore + ((int)str[2 + i] - 48) * pow(10, intLength - i - 1);
        }

        if (snake->score >= fScore && entered != 1) {
            scores[x] = snake->score;
            strcpy(highScoreNames[x], name);

            x++;
            entered = 1;
        }

        strcpy(highScoreNames[x], highScoreName);
        scores[x] = fScore;

        for (y = 0; y < 20; y++) {
            highScoreName[y] = '\0';
        }

        x++;
        if (x >= 5)
            break;
    }

    fclose(fp);

    file = fopen("highscores", "w+");

    for (i = 0; i < 5; i++) {
        fprintf(file, "%d\t%d\t\t\t%s\n", i + 1, scores[i], highScoreNames[i]);
    }

    fclose(file);
}

void snake_display_high_scores(void)
{
    FILE *fp;
    char str[128];
    int x = 10;
    int y = 5;

    clrscr();

    if ((fp = fopen("highscores", "r")) == NULL) {
        snake_create_high_scores();
        if ((fp = fopen("highscores", "r")) == NULL) {
            exit(1);
        }
    }

    gotoxy(x, y);
    y++;
    printf("High Scores");
    gotoxy(x, y);
    y++;
    printf("Rank\tScore\t\t\tName");
    while (!feof(fp)) {
        gotoxy(x, y);
        y++;
        if (fgets(str, 126, fp)) {
            printf("%s", str);
        }
    }

    fclose(fp);
    gotoxy(x, y);
    y++;

    printf("Press any key to continue...");
    wait_4_key();
}

void snake_you_win_screen(void)
{
    int x = 6, y = 7;
    int i;

    for (i = 0; i < ARRAY_SIZE(str_you_win_scr); i++) {
        gotoxy(x, y);
        y++;
        printf("%s", str_you_win_scr[i]);
    }

    wait_4_key();
    clrscr();
}

void snake_game_overs_screen(void)
{
    int x = 17, y = 3;
    int i;

    for (i = 0; i < ARRAY_SIZE(str_game_voer_scr); i++) {
        gotoxy(x, y);
        y++;
        printf("%s", str_game_voer_scr[i]);
    }
    wait_4_key();
    clrscr();
}

void snake_refresh_info_bar(snake_object_t *snake)
{
    gotoxy(5, SNAKE_CONSOLE_HEIGHT + 2);
    printf("%s%d", str_info_bar[0], snake->score);

    gotoxy(5, SNAKE_CONSOLE_HEIGHT + 3);
    printf("%s%d", str_info_bar[1], snake->speed);

    gotoxy(SNAKE_CONSOLE_WIDTH - 15, SNAKE_CONSOLE_HEIGHT + 2);
    printf("%s", str_info_bar[2]);

    gotoxy(SNAKE_CONSOLE_WIDTH - 15, SNAKE_CONSOLE_HEIGHT + 3);
    printf("%s", str_info_bar[3]);
    gotoxy(0, 0);
}

static void snake_enviroment_init(int console_width, int console_height)
{
    int x = 1, y = 1;

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

static void snake_len_init(snake_object_t *snake)
{
    snake->slen = SNAKE_LEN_INIT;
}

static void snake_body_init(snake_object_t *snake)
{
    int i;
    int x, y;

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

static void snake_food_init(snake_object_t *snake)
{
    snake->fx = SNAKE_FOOD_POS_X;
    snake->fy = SNAKE_FOOD_POS_Y;
    gotoxy(snake->fx, snake->fy);
    printf("%c", SNAKE_FOOD);
}

static void snake_speed_init(snake_object_t *snake)
{
    int speed;
    clrscr();

    do {
        gotoxy(10, 5);
        printf("Select The game speed between 1 and 9.");
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
    srand(time(NULL));
    snake->fx = rand() % SNAKE_CONSOLE_WIDTH + 1;
    srand(time(NULL));
    snake->fy = rand() % SNAKE_CONSOLE_HEIGHT + 1;
    gotoxy(snake->fx, snake->fy);
    printf("%c", SNAKE_FOOD);
}

static bool snake_is_kill_by_wall(snake_object_t *snake)
{
    if (snake->sx[0] == 0 || snake->sy[0] == 0 ||
        snake->sx[0] == SNAKE_CONSOLE_WIDTH ||
        snake->sy[0] == SNAKE_CONSOLE_HEIGHT) {
        return true;
    }
    return false;
}

SNAKE_DIR snake_check_keys_pressed(SNAKE_DIR direction)
{
    char pressed;

    if (kbhit()) {
        pressed = getch();
        if (direction != pressed) {
            if (pressed == DOWN_ARROW && direction != UP_ARROW) {
                direction = pressed;
            } else if (pressed == UP_ARROW && direction != DOWN_ARROW) {
                direction = pressed;
            } else if (pressed == LEFT_ARROW && direction != RIGHT_ARROW) {
                direction = pressed;
            } else if (pressed == RIGHT_ARROW && direction != LEFT_ARROW) {
                direction = pressed;
            } else if (pressed == EXIT_BUTTON || pressed == PAUSE_BUTTON) {
                pause_menu();
            }
        }
    }
    return direction;
}

void snake_start_game(snake_object_t *snake)
{
    SNAKE_GAME_OVER game_over = SNAKE_GAME_OVER_NONE;
    int wait =
        clock() + CLOCKS_PER_SEC - (snake->speed) * (CLOCKS_PER_SEC / 10);

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
            wait = clock() + CLOCKS_PER_SEC -
                   (snake->speed) * (CLOCKS_PER_SEC / 10);
        }

        if (snake_is_kill_by_wall(snake)) {
            game_over = SNAKE_GAME_OVER_BY_WALL;
        }

    } while (!game_over);
    // show_cursor();

    switch (game_over) {
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

    if (snake->score >= snake_get_lowest_score() && snake->score != 0) {
        snake_input_score(snake);
        snake_display_high_scores();
    }
}

static void snake_load_game(void)
{
    snake_object_t *snake = snake_get_snake_object();
    snake_speed_init(snake);
    snake_enviroment_init(SNAKE_CONSOLE_WIDTH, SNAKE_CONSOLE_HEIGHT);
    snake_len_init(snake);
    snake_body_init(snake);
    snake_food_init(snake);
    snake_refresh_info_bar(snake);
    snake_start_game(snake);
}

static void snake_welcome_art(void)
{
    int i;

    clrscr();
    for (i = 0; i < ARRAY_SIZE(str_wel_art); i++) {
        printf("%s", str_wel_art[i]);
    }
    wait_4_key();
}

static void snake_tips(void)
{
    int x = 10, y = 5;
    int i;

    clrscr();
    for (i = 0; i < ARRAY_SIZE(str_tips); i++) {
        gotoxy(x, y);
        y++;
        printf("%s", str_tips[i]);
    }
    wait_4_key();
}

static void snake_exit_yn(void)
{
    char pressed;

    clrscr();
    gotoxy(9, 8);
    printf("Are you sure you want to exit(Y/N)\n");

    do {
        pressed = wait_4_key();
        pressed = tolower(pressed);
    } while (!(pressed == 'y' || pressed == 'n'));

    printf("%c", pressed);

    if (pressed == 'y') {
        printf("\t      Press Enter To End...            \n");
        do {
            pressed = wait_4_key();
        } while (pressed != ENTER_KEY);
        clrscr();
        exit(1);
    }
}

int snake_main_meun(void)
{
#define MAIN_MENU_INIT_X 10
#define MAIN_MENU_INIT_Y 5
    int x = MAIN_MENU_INIT_X, y = MAIN_MENU_INIT_Y;
    int ystart = y;
    char key;
    int selected = 0;

    clrscr();
    gotoxy(x, y);
    y++;
    for (int i = 0; i < ARRAY_SIZE(str_menu); i++) {
        printf("%s\n", str_menu[i]);
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
        case SNAKE_MENU0:
            snake_load_game();
            break;
        case SNAKE_MENU1:
            snake_display_high_scores();
            break;
        case SNAKE_MENU2:
            snake_tips();
            break;
        case SNAKE_MENU3:
            snake_exit_yn();
            break;
        default:
            break;
        }
    } while (1);

    return 0;
}
#endif
