#ifdef SNAKE_DEMO

#include "ctype.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "stdbool.h"

#include "assert.h"
#include "utils.h"
#include "string.h"

#define SNAKE_ARRAY_SIZE 310

#define SNAKE_CONSOLE_WIDTH 80
#define SNAKE_CONSOLE_HEIGHT 21

/* snake init */
#define SNAKE_INIT_LEN 8

#define SNAKE_HEAD 'X'
#define SNAKE_BODY '#'
#define SNAKE_WALL '#'
#define SNAKE_FOOD '*'
#define SNAKE_BLANK ' '

static void snake_food(int food_xy[], int width, int height,
                       int snake_xy[][SNAKE_ARRAY_SIZE], int snake_length);
void snake_refresh_info_bar(int score, int speed);

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
    "Coder: HZJ",
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
    SNAKE_DIR_UP,
    SNAKE_DIR_DOWN,
    SNAKE_DIR_LEFT,
    SNAKE_DIR_RIGHT,
} SNAKE_DIR;

int snake_get_game_speed(void)
{
    int speed;
    clrscr();

    do {
        gotoxy(10, 5);
        printf("Select The game speed between 1 and 9.");
        speed = wait_4_key() - '0';
    } while (speed < 1 || speed > 9);
    return speed;
}

void pause_menu(void)
{
    gotoxy(28, 23);
    printf("**Paused**");

    wait_4_key();
    gotoxy(28, 23);
    printf("            ");
}

int collision_snake(int x, int y, int snake_xy[][SNAKE_ARRAY_SIZE],
                    int snake_length, int detect)
{
    int i;
    for (i = detect; i < snake_length; i++) {
        if (x == snake_xy[0][i] && y == snake_xy[1][i]) {
            return 1;
        }
    }
    return 0;
}

void snake_move_snake_array(int snake_xy[][SNAKE_ARRAY_SIZE], int snake_length,
                            int direction)
{
    int i;
    for (i = snake_length - 1; i >= 1; i--) {
        snake_xy[0][i] = snake_xy[0][i - 1];
        snake_xy[1][i] = snake_xy[1][i - 1];
    }

    switch (direction) {
    case DOWN_ARROW:
        snake_xy[1][0]++;
        break;
    case RIGHT_ARROW:
        snake_xy[0][0]++;
        break;
    case UP_ARROW:
        snake_xy[1][0]--;
        break;
    case LEFT_ARROW:
        snake_xy[0][0]--;
        break;
    default:
        break;
    }
}

bool snake_is_eat_food(int snake_xy[][SNAKE_ARRAY_SIZE], int food_xy[])
{
    if (snake_xy[0][0] == food_xy[0] && snake_xy[1][0] == food_xy[1]) {
        food_xy[0] = 0;
        food_xy[1] = 0;

        BELL();
        return true;
    }

    return false;
}

int collision_detection(int snake_xy[][SNAKE_ARRAY_SIZE], int consoleWidth,
                        int consoleHeight, int snake_length)
{
    int colision = 0;
    if ((snake_xy[0][0] == 1) || (snake_xy[1][0] == 1) ||
        (snake_xy[0][0] == consoleWidth) ||
        (snake_xy[1][0] == consoleHeight - 4)) {
        colision = 1;
    } else if (collision_snake(snake_xy[0][0], snake_xy[1][0], snake_xy,
                               snake_length, 1)) {
        colision = 1;
    }

    return colision;
}

void create_high_scores(void)
{
    FILE *file;
    int i;

    file = fopen("highscores", "w+");

    if (file == NULL) {
        printf("FAILED TO CREATE HIGHSCORES!!! EXITING!");
        exit(0);
    }

    for (i = 0; i < 5; i++) {
        fprintf(file, "%d", i + 1);
        fprintf(file, "%s", "\t0\t\t\tEMPTY\n");
    }

    fclose(file);
    return;
}

int get_lowest_score(void)
{
    FILE *fp;
    char str[128];
    int lowestScore = 0;
    int i;
    int intLength;

    if ((fp = fopen("highscores", "r")) == NULL) {
        create_high_scores();
        if ((fp = fopen("highscores", "r")) == NULL) {
            exit(1);
        }
    }

    while (!feof(fp)) {
        fgets(str, 126, fp);
    }
    fclose(fp);

    i = 0;

    while (str[2 + i] != '\t') {
        i++;
    }

    intLength = i;

    for (i = 0; i < intLength; i++) {
        lowestScore =
            lowestScore + ((int)str[2 + i] - 48) * pow(10, intLength - i - 1);
    }

    return lowestScore;
}

void input_score(int score)
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
        create_high_scores();
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

        if (score >= fScore && entered != 1) {
            scores[x] = score;
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

void display_high_scores(void)
{
    FILE *fp;
    char str[128];
    int x = 10;
    int y = 5;

    clrscr();

    if ((fp = fopen("highscores", "r")) == NULL) {
        create_high_scores();
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

    clrscr();
    for (i = 0; i < ARRAY_SIZE(str_game_voer_scr); i++) {
        gotoxy(x, y);
        y++;
        printf("%s", str_game_voer_scr[i]);
    }
    wait_4_key();
    clrscr();
}

static void snake_enviroment_init(int console_width, int console_height)
{
    int x = 1, y = 1;

    clrscr();
    gotoxy(x, y);

    for (; y < console_height; y++) {
        gotoxy(x, y);
        printf("%c", SNAKE_WALL);

        gotoxy(console_width, y);
        printf("%c", SNAKE_WALL);
    }

    y = 1;
    for (; x < console_width + 1; x++) {
        gotoxy(x, y);
        printf("%c", SNAKE_WALL);

        gotoxy(x, console_height);
        printf("%c", SNAKE_WALL);
    }
    wait_4_key();
}

static void snake_position_init(int snake_xy[][SNAKE_ARRAY_SIZE])
{
    snake_xy[0][0] = 40;
    snake_xy[1][0] = 10;
}

static void snake_body_init(int snake_xy[][SNAKE_ARRAY_SIZE], int snake_length)
{
    int i;
    int snake_x = snake_xy[0][0];
    int snake_y = snake_xy[1][0];

    for (i = 1; i <= snake_length; i++) {
        snake_xy[0][i] = snake_x + i;
        snake_xy[1][i] = snake_y;
    }

    for (i = 0; i < snake_length; i++) {
        gotoxy(snake_xy[0][i], snake_xy[1][i]);
        printf("%c", SNAKE_BODY);
    }

    // wait_4_key();
}

static void snake_food(int food_xy[], int width, int height,
                       int snake_xy[][SNAKE_ARRAY_SIZE], int snake_length)
{
    do {
        srand(time(NULL));
        food_xy[0] = rand() % (width - 2) + 2;
        srand(time(NULL));
        food_xy[1] = rand() % (height - 6) + 2;
    } while (
        collision_snake(food_xy[0], food_xy[1], snake_xy, snake_length, 0));

    gotoxy(food_xy[0], food_xy[1]);
    printf("%c", SNAKE_FOOD);

    // wait_4_key();
}

void snake_refresh_info_bar(int score, int speed)
{
    gotoxy(5, 23);
    printf("%s%d", str_info_bar[0], score);

    gotoxy(5, 24);
    printf("%s%d", str_info_bar[1], speed);

    gotoxy(52, 23);
    printf("%s", str_info_bar[2]);

    gotoxy(52, 24);
    printf("%s", str_info_bar[3]);

    // wait_4_key();
}

int snake_check_keys_pressed(int direction)
{
    int pressed;

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

void snake_move(int snake_xy[][SNAKE_ARRAY_SIZE], int snake_length,
                int direction)
{
    int x, y;

    x = snake_xy[0][snake_length - 1];
    y = snake_xy[1][snake_length - 1];

    gotoxy(x, y);
    printf("%c", SNAKE_BLANK);

    gotoxy(snake_xy[0][0], snake_xy[1][0]);
    printf("%c", SNAKE_BODY);

    snake_move_snake_array(snake_xy, snake_length, direction);

    gotoxy(snake_xy[0][0], snake_xy[1][0]);
    printf("%c", SNAKE_HEAD);

    gotoxy(0, 0);
}

static void snake_start_game(int snake_xy[][SNAKE_ARRAY_SIZE], int food_xy[],
                             int consoleWidth, int consoleHeight,
                             int snake_length, int direction, int score,
                             int speed)
{
    int game_over = 0;
    clock_t end_wait;

    // CLOCKS_PER_SEC-(n-1)*(CLOCKS_PER_SEC/10)
    int wait_mili = CLOCKS_PER_SEC - (speed) * (CLOCKS_PER_SEC / 10);
    int temp_score = 10 * speed;
    int old_direction;
    bool can_change_direction = true;

    end_wait = clock() + wait_mili;

    do {
        if (can_change_direction) {
            old_direction = direction;
            direction = snake_check_keys_pressed(direction);
        }

        if (old_direction != direction) {
            can_change_direction = false;
        }

        if (clock() >= end_wait) {
            snake_move(snake_xy, snake_length, direction);
            can_change_direction = true;

            if (snake_is_eat_food(snake_xy, food_xy)) {
                snake_food(food_xy, consoleWidth, consoleHeight, snake_xy,
                           snake_length);
                snake_length++;
                score += speed;
                if (score >= 10 * speed + temp_score) {
                    speed++;
                    temp_score = score;

                    if (speed <= 9) {
                        wait_mili = wait_mili - (CLOCKS_PER_SEC / 10);
                    }

                    else if (wait_mili >= 40) {
                        wait_mili = wait_mili - (CLOCKS_PER_SEC / 200);
                    }
                }

                snake_refresh_info_bar(score, speed);
            }

            end_wait = clock() + wait_mili;
        }

        game_over = collision_detection(snake_xy, consoleWidth, consoleHeight,
                                        snake_length);

        if (snake_length >= SNAKE_ARRAY_SIZE - 5) {
            game_over = 2;
            score += 1500;
        }

    } while (!game_over);

    switch (game_over) {
    case 1:
        BELL();
        BELL();

        snake_game_overs_screen();

        break;
    case 2:
        snake_you_win_screen();
        break;
    }

    if (score >= get_lowest_score() && score != 0) {
        input_score(score);
        display_high_scores();
    }
}

static void snake_load_game(void)
{
    int snake_xy[2][SNAKE_ARRAY_SIZE];
    int food_xy[] = { 5, 5 }; // snake food init position
    int direction = LEFT_ARROW; // snake init direction
    int score = 0;
    int speed = snake_get_game_speed();

    snake_enviroment_init(SNAKE_CONSOLE_WIDTH, SNAKE_CONSOLE_HEIGHT);
    snake_position_init(snake_xy);
    snake_body_init(snake_xy, SNAKE_INIT_LEN);
    snake_food(food_xy, SNAKE_CONSOLE_WIDTH, SNAKE_CONSOLE_HEIGHT, snake_xy,
               SNAKE_INIT_LEN);
    snake_refresh_info_bar(score, speed);
    snake_start_game(snake_xy, food_xy, SNAKE_CONSOLE_WIDTH,
                     SNAKE_CONSOLE_HEIGHT, SNAKE_INIT_LEN, direction, score,
                     speed);
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

static int snake_menu_selector(int x, int y, int ystart)
{
    int i = 0;
    char key;

    x -= 2;
    gotoxy(x, ystart);
    printf(">");
    gotoxy(0, 0); // put cursor at the origin

    do {
        key = wait_4_key();
        if (key == UP_ARROW) {
            gotoxy(x, ystart + i);
            printf(" "); // clear ">" sign

            if (ystart >= ystart + i)
                i = y - ystart - 2;
            else
                i--;
            gotoxy(x, ystart + i);
            printf(">");
        } else if (key == DOWN_ARROW) {
            gotoxy(x, ystart + i);
            printf(" "); // clear ">" sign

            if (i + 2 >= y - ystart)
                i = 0;
            else
                i++;
            gotoxy(x, ystart + i);
            printf(">");
        }
    } while (key != ENTER_KEY);

    return i;
}

static int snake_main_meun(void)
{
    int x = 10, y = 5;
    int ystart = y;
    int selected;
    int i;

    clrscr();
    gotoxy(x, y);
    y++;
    for (i = 0; i < ARRAY_SIZE(str_menu); i++) {
        printf("%s\n", str_menu[i]);
        gotoxy(x, y);
        y++;
    }

    selected = snake_menu_selector(x, y, ystart);

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
            display_high_scores();
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
